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
	auto oldSize  = size();

    if(!pFlagsD_.deletePoints(delPoints.deviceView()))
	{
		fatalErrorInFunction<<
		"Error in deleting points from internal points"<<endl;
		return false;
	}
	unSyncFlag();
	
	auto newRange = pFlagsD_.activeRange();
	auto newSize  = size();

	anyList varList;

	varList.emplaceBack(
		message::eventName(message::ITEM_DELETE), 
		delPoints);
	message msg(message::ITEM_DELETE);

	if(oldSize!= newSize)
	{
		msg.add(message::SIZE_CHANGED); 
		varList.emplaceBack(
			message::eventName(message::SIZE_CHANGED),
			newSize);
	}
	
	if(oldRange!=newRange)
	{
		msg.add(message::RANGE_CHANGED);
		varList.emplaceBack(
			message::eventName(message::RANGE_CHANGED),
			newRange);
	}
	auto iter = time().currentIter();
	auto t = time().currentTime();
	auto dt = time().dt();

	if( !notify(iter, t, dt, msg, varList) )
	{
		fatalErrorInFunction;
		return false;
	}

	return true;
}

bool pFlow::internalPoints::changePointsFlagPosition
(
	const uint32Vector_D& changePoints,
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
	pFlagsD_.changeFlags(changePoints.deviceView(), toBoundaryIndex);
	unSyncFlag();

	// change the position 
	pFlow::internalPointsKernels::changePosition
	(
		pointPosition_.deviceViewAll(),
		changePoints.deviceView(),
		transferVector
	);

	anyList varList;
	message msg;
	
	varList.emplaceBack(
		message::eventName(message::ITEM_FLAGCHANGED),
		changePoints);
	varList.emplaceBack("fromBoundaryIndex", fromBoundaryIndex);
	varList.emplaceBack("toBoundaryIndex", toBoundaryIndex);

	msg.add(message::ITEM_FLAGCHANGED);

	if( !notify(
		time().currentIter(), 
		time().currentTime(), 
		time().dt(),
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

FUNCTION_H
const typename pFlow::internalPoints::PointsType& 
    pFlow::internalPoints::pointPosition()const
{
    return pointPosition_;
}

FUNCTION_H
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

FUNCTION_H
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

FUNCTION_H
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


FUNCTION_H
bool pFlow::internalPoints::write
(
	iOstream& os
)const
{
	auto aPoints = this->activePointsHost();
	return aPoints.write(os);
}

FUNCTION_H
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
		


FUNCTION_H
bool pFlow::internalPoints::write
(
	iOstream& os, 
	const IOPattern& iop
)const
{
	auto aPoints = activePointsHost();
	return aPoints.write(os,iop);	
}
