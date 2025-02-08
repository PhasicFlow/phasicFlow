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

#include "Time.hpp"
#include "internalPoints.hpp"
#include "domain.hpp"
#include "Vectors.hpp"
#include "anyList.hpp"
#include "internalPointsKernels.hpp"

void pFlow::internalPoints::syncPFlag()const
{
	if(!pFlagSync_)
	{
		pFlagsH_ = pFlagsD_.clone<DefaultHostExecutionSpace>();
		pFlagSync_ = true;
	}
}

bool pFlow::internalPoints::deletePoints(const uint32Vector_D& delPoints)
{
	
	if(delPoints.empty())return true;

	auto oldRange = pFlagsD_.activeRange();

    if(!pFlagsD_.deletePoints(delPoints.deviceView()))
	{
		fatalErrorInFunction<<
		"Error in deleting points from internal points"<<endl;
		return false;
	}
	unSyncFlag();
	
	auto newRange = pFlagsD_.activeRange();

	anyList varList;

	message msg;
	varList.emplaceBack(
		msg.addAndName(message::ITEMS_DELETE), 
		delPoints.deviceView());
	
	if(oldRange!=newRange)
	{
		msg.add(message::RANGE_CHANGED);
		varList.emplaceBack(
			msg.addAndName(message::RANGE_CHANGED),
			newRange);
	}
	
	const auto& ti = time().TimeInfo();
	
	if( !notify(ti , msg, varList) )
	{
		fatalErrorInFunction;
		return false;
	}
	return true;
}

bool pFlow::internalPoints::changePointsFlagPosition
(
	const uint32Vector_D& changePointsIndices,
	realx3 transferVector, 
	uint32 fromBoundaryIndex,
	uint32 toBoundaryIndex
)
{
	if(toBoundaryIndex>5)
	{
		fatalErrorInFunction<<
		"Invalid boundary index "<< toBoundaryIndex<<endl;
		return false;
	}

	// change the flag 
	pFlagsD_.changeFlags(changePointsIndices.deviceView(), toBoundaryIndex);
	unSyncFlag();

	// change the position 
	pFlow::internalPointsKernels::changePosition
	(
		pointPosition_.deviceViewAll(),
		changePointsIndices.deviceView(),
		transferVector
	);

	anyList varList;
	message msg;
	
	varList.emplaceBack(
		msg.addAndName(message::ITEMS_FLAGCHANGED),
		changePointsIndices.deviceView());
	varList.emplaceBack("fromBoundaryIndex", fromBoundaryIndex);
	varList.emplaceBack("toBoundaryIndex", toBoundaryIndex);

	if( !notify(
		time().TimeInfo(),
		msg,
		varList))
	{
		fatalErrorInFunction<<
		"Error in notify for item transfer from "<< fromBoundaryIndex<<
		" to "<<toBoundaryIndex<< " boundary"<<endl;
		return false;
	}

	return true;
}

bool pFlow::internalPoints::sortPoints(const uint32IndexContainer &sortedIndices)
{
	if(!pointPosition_.reorderItems(sortedIndices))
	{
		fatalErrorInFunction;
		return false;
	}
    
	pFlagsD_.resetFlags(pFlagsD_.capacity(), 0u, sortedIndices.size());
	unSyncFlag();

	return true;
}

pFlow::internalPoints::internalPoints()
    : subscriber("internalPoints"),
      pointPosition_("internalPoints", "internalPoints", initialCapacity_, 0, RESERVE()),
      pFlagsD_(initialCapacity_, 0, 0)
{}


pFlow::internalPoints::internalPoints
(
	const realx3Vector& posVec
)
:
	subscriber("internalPoints"),
	pointPosition_
	(
		"internalPoints", 
		"internalPoints", 
		posVec.capacity(), 
		0, 
		RESERVE()
	),
	pFlagsD_
	(
		static_cast<uint32>(posVec.capacity()), 
		static_cast<uint32>(0),
		static_cast<uint32>(posVec.size())
	)
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


const typename pFlow::internalPoints::PointsType& 
    pFlow::internalPoints::pointPosition()const
{
    return pointPosition_;
}


typename pFlow::internalPoints::PointsType&
	pFlow::internalPoints::pointPosition()
{
    return pointPosition_;
}

typename pFlow::internalPoints::PointsTypeHost
    pFlow::internalPoints::activePointsHost() const
{
    auto maskH = activePointsMaskHost();
    auto pointsH = pointPositionHost();

	PointsTypeHost aPoints
	(
		pointPosition_.name(),
		pointPosition_.fieldKey(),
		maskH.numActive(), 
		maskH.numActive(),
		RESERVE()
	);

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

bool pFlow::internalPoints::deletePoints
(
	scatteredFieldAccess<uint32, memory_space> delPoints
)
{
	if(!pFlagsD_.deletePoints(delPoints))
	{
		fatalErrorInFunction<<
		"Error in deleting points from internal points"<<endl;
		return false;
	}
	unSyncFlag();
	WARNING<<"Notify the observersin in internalPoints"<<END_WARNING;

    return true;
}


pFlow::uint32 pFlow::internalPoints::updateFlag
(
	const domain& dm, 
	const std::array<real,6>& dist
)
{
	unSyncFlag();
	return pFlagsD_.markPointRegions
	(
		dm,
		pointPosition_.deviceViewAll(),
		dist[0],
		dist[1],
		dist[2],
		dist[3],
		dist[4],
		dist[5]
	);	
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
		
}

bool
pFlow::internalPoints::insertPoints(
  const realx3Vector& points,
  anyList&            varList
)
{
	
	message msg;
	realx3Vector_D points_D(points.name(), points);
	if(!insertPointsOnly(points_D, msg, varList))
	{
		return false;
	}
	//output<<" points \n"<<points<<endl;
	//output<<pointPosition_<<endl;
	if(!notify(
		time().TimeInfo(),
		msg, 
		varList))
	{
		fatalErrorInFunction<<
		"Error in notifying observers of "<< subscriberName()<<endl;
		return false;
	}

	return true;
}

bool pFlow::internalPoints::insertPointsOnly(
	const realx3Vector_D &points,
	message& msg, 
	anyList &varList
)
{

	uint32 numNew = static_cast<uint32>(points.size());

	auto oldActiveRange = pFlagsD_.activeRange();
	
	uint32 emptySpots = pFlagsD_.capacity() - pFlagsD_.numActive();

    if(emptySpots!= 0) emptySpots--;

	if( numNew > emptySpots )
	{
		// increase the capacity to hold new points 
		pFlagsD_.changeCapacity(numNew);
		unSyncFlag();
	}
	
	// first check if it is possible to add to the beggining of list
	if(numNew <= oldActiveRange.start())
	{
		varList.emplaceBack<uint32IndexContainer>(
			msg.addAndName(message::ITEMS_INSERT),
			0u, numNew);
	}
	// check if it is possible to add to the end of the list  
	else if( numNew <= pFlagsD_.capacity() - oldActiveRange.end() )
	{	 	
		varList.emplaceBack<uint32IndexContainer>(
			msg.addAndName(message::ITEMS_INSERT),
			oldActiveRange.end(), oldActiveRange.end()+numNew);
	}
	// we should fill the scattered empty spots
	else
	{
		auto newIndices = pFlagsD_.getEmptyPoints(numNew);
		if(numNew != newIndices.size())
		{
			fatalErrorInFunction<<"not enough empty points in pointFlag"<<
			numNew<< " "<<newIndices.size() <<endl;
			pOutput<< pFlagsD_.capacity()<<endl;
			pOutput<< pFlagsD_.numActive()<<endl;
			return false;
		}
		
		varList.emplaceBack<uint32IndexContainer>(
			msg.addAndName(message::ITEMS_INSERT),
			newIndices
		);
	}

	const auto& indices = varList.getObject<uint32IndexContainer>(
		message::eventName(message::ITEMS_INSERT));

	auto nAdded = pFlagsD_.addInternalPoints(indices.deviceView());
	unSyncFlag();

	if(nAdded != numNew )
	{
		fatalErrorInFunction;
		return false;
	}

	pointPosition_.reserve( pFlagsD_.capacity() );
	if(!pointPosition_.insertSetElement(indices, points.deviceView()))
	{
		fatalErrorInFunction<<
		"Error in inserting new positions into pointPosition"<<
		" internalPoints field"<<endl;
		return false;
	}

	auto newActiveRange = pFlagsD_.activeRange();

	if(oldActiveRange != newActiveRange)
	{
		varList.emplaceBack(
			msg.addAndName(message::RANGE_CHANGED),
			newActiveRange);
	}
	
	return true;
}

bool pFlow::internalPoints::read
(
	iIstream& is
)
{
	
	PointsTypeHost fRead(pointPosition_.name(), pointPosition_.fieldKey());

	if( !fRead.read(is))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	pointPosition_.assignFromHost(fRead);

	pFlagsD_ = pFlagTypeDevice(pointPosition_.capacity(), 0, pointPosition_.size());
	unSyncFlag();
	syncPFlag();

	return true;
}



bool pFlow::internalPoints::write
(
	iOstream& os
)const
{
	auto aPoints = this->activePointsHost();
	return aPoints.write(os);
}


bool pFlow::internalPoints::read
(
	iIstream& is, 
	const IOPattern& iop
)
{
	
	PointsTypeHost fRead(pointPosition_.name(), pointPosition_.fieldKey());

	if( !fRead.read(is, iop))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	pointPosition_.assignFromHost(fRead);

	createDeviceFlag(pointPosition_.capacity(), 0, pointPosition_.size());
	syncPFlag();

	return true;
}

bool pFlow::internalPoints::write
(
	iOstream& os, 
	const IOPattern& iop
)const
{
	auto aPoints = activePointsHost();
	return aPoints.write(os,iop);	
}
