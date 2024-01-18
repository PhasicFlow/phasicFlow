/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/


#include "internalPoints.hpp"
#include "domain.hpp"
#include "Vectors.hpp"

void pFlow::internalPoints::syncPFlag()const
{
	if(!pFlagSync_)
	{
		pFlagsH_ = pFlagsD_.clone<DefaultHostExecutionSpace>();
		pFlagSync_ = true;
	}
}

pFlow::internalPoints::internalPoints()
:
	subscriber("internalPoints"),
	pointPosition_("internalPoints", "internalPoints", initialCapacity_, 0, RESERVE()),
	pFlagsD_(initialCapacity_, 0 , 0)
{}


pFlow::internalPoints::internalPoints
(
	const realx3Vector& posVec
)
:
	subscriber("internalPoints"),
	pointPosition_("internalPoints", "internalPoints", posVec.capacity(), 0, RESERVE()),
	pFlagsD_(posVec.capacity(), 0, posVec.size())
{

	pointPosition_.assign(posVec);

}


const pFlow::pFlagTypeDevice& 
	pFlow::internalPoints::activePointsMaskDevice() const 
{
	return pFlagsD_;
}


const pFlow::pFlagTypeHost&
	pFlow::internalPoints::activePointsMaskHost() const
{
	syncPFlag();
	return pFlagsH_;
}

FUNCTION_H
const pFlow::realx3Field_D& 
    pFlow::internalPoints::pointPosition()const
{
    return pointPosition_;
}

pFlow::hostViewType1D<pFlow::realx3> 
    pFlow::internalPoints::activePointsHost() const
{
    auto maskH = activePointsMaskHost();
    auto pointsH = pointPositionHost();

    hostViewType1D<realx3> aPoints("Active pointst", maskH.numActive());
    auto aRange = maskH.activeRange();
    uint32 n = 0;
    for(auto i=aRange.start(); i<aRange.end(); i++)
    {
        if( maskH.isActive(i) )
        {
            aPoints[n] = pointsH[i];
            n++;
        }
    }

    return aPoints;
}

FUNCTION_H
pFlow::realx3Field_D &pFlow::internalPoints::pointPosition()
{
    return pointPosition_;
}

FUNCTION_H
pFlow::uint32 pFlow::internalPoints::updateFlag
(
	const domain& dm, 
	const std::array<real,6>& dist
)
{
	return pFlagsD_.markPointRegions
	(
		dm,
		pointPosition_.deviceVectorAll(),
		dist[0],
		dist[1],
		dist[2],
		dist[3],
		dist[4],
		dist[5]
	);

	pFlagSync_ = false;
}

void pFlow::internalPoints::fillNeighborsLists
(
	ViewType1D<uint32, memory_space> leftList, 
	ViewType1D<uint32, memory_space> rightList, 
	ViewType1D<uint32, memory_space> bottomList, 
	ViewType1D<uint32, memory_space> topList, 
	ViewType1D<uint32, memory_space> rearList, 
	ViewType1D<uint32, memory_space> frontList
)
{
	pFlagsD_.fillNeighborsLists(
		leftList,
		rightList,
		bottomList,
		topList,
		rearList,
		frontList);
		
	pFlagSync_ = false;
}

FUNCTION_H
bool pFlow::internalPoints::read
(
	iIstream& is, 
	const IOPattern& iop
)
{
	
	Field<Vector, realx3 , vecAllocator<realx3>> fRead("internalPoints", "internalPoints");

	if( !fRead.read(is, iop))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	/// here there should be some mechanism for field distribution between procesors
	

	pointPosition_.assign(fRead.vectorField());

	pFlagsD_ = pFlagTypeDevice(pointPosition_.capacity(), 0, pointPosition_.size());
	pFlagSync_ = false;
	syncPFlag();

	return true;
}
		


FUNCTION_H
bool pFlow::internalPoints::write
(
	iOstream& os, 
	const IOPattern& iop
)const
{
	if( pFlagsD_.isAllActive())
	{
		return pointPosition_.write(os, iop);
	}
	else
	{
		auto aPoints = this->activePointsHost();
		auto spanPoints = makeSpan(aPoints); //span<realx3>(aPoints.data(), aPoints.size());
		return writeSpan(os, spanPoints, iop);
	}
}


/*FUNCTION_H
bool pFlow::internalPoints::evaluateinternalPoints()
{
	if(pointFlag_.size() != pointPosition_.size())
	{
		fatalErrorInFunction<<
		"number of elements in pointFlag and pointPosition is not equal \n";
		return false;
	}

	setNumMaxPoints();

	int32 minActive, maxActive;
	numActivePoints_ = pFlow::internalPointsKernels::scanPointFlag(
		0,
		numPoints_,
		static_cast<int8>(internalPoints::ACTIVE),
		pointFlag_.deviceVectorAll(),
		minActive,
		maxActive
		);
	
	activeRange_ = {minActive, maxActive};	
	
	return true;
}

FUNCTION_H
void pFlow::internalPoints::setNumMaxPoints()
{
	maxPoints_ = pointFlag_.capacity();
	numPoints_ = pointFlag_.size();
}

FUNCTION_H
pFlow::realx3Field_D& pFlow::internalPoints::pointPosition()
{
	return pointPosition_;
}

FUNCTION_H
pFlow::int8Field_HD& pFlow::internalPoints::pointFlag()
{
	return pointFlag_;
}
		
pFlow::uniquePtr<pFlow::int32IndexContainer> 
	pFlow::internalPoints::getNewPointsIndices(int32 numNewPoints)const
{


	if( capacity() - activeRange_.second >=  numNewPoints )
	{
		// fill the sequence starting from activeRange_.second-1
		return makeUnique<int32IndexContainer>(
			activeRange_.second,
			activeRange_.second+numNewPoints);

	}

	// second, check if there is space at the beginning
	if( activeRange_.first >= numNewPoints)
	{
		return makeUnique<int32IndexContainer>(
			0,
			numNewPoints);	
	}

	// otherwise scan the points from first to the end to find empty spaces 
	int32Vector newPoints(
		numNewPoints,
		RESERVE());

	newPoints.clear();
	int32 numAdded = 0;
	ForAll(i, pointFlag_)
	{
		if(!isActive(i))
		{
			newPoints.push_back(static_cast<int32>(i));
			numAdded++;
		}

		if(numAdded == numNewPoints)
		{
			return makeUnique<int32IndexContainer>(
				newPoints.data(),
				numNewPoints);
		}
	}

	// check if there is space at the end for the remaining of points
	if( numAdded <numNewPoints && capacity() - size() >= numNewPoints - numAdded )
	{
		int32 ind = size();
		for(int32 i=numAdded; i<numNewPoints; i++)
		{
			newPoints.push_back(ind);
			ind++;
		}
		
		return makeUnique<int32IndexContainer>(
			newPoints.data(),
			numNewPoints);
	}
	else
	{
		fatalErrorInFunction<<"not enough capacity for inserting particles into the point structure\n";
		return nullptr;
	}

	return nullptr;
}*/