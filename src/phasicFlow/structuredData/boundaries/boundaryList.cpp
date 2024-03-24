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

#include "boundaryList.hpp"
#include "pointStructure.hpp"


bool pFlow::boundaryList::resetLists()
{
	clear();
	listSet_ = false;
    return true;
}

bool pFlow::boundaryList::updateLists()
{
    
	std::array<real,6> dist;
	dist[0] = boundary(0).neighborLength();
	dist[1] = boundary(1).neighborLength();
	dist[2] = boundary(2).neighborLength();
	dist[3] = boundary(3).neighborLength();
	dist[4] = boundary(4).neighborLength();
	dist[5] = boundary(5).neighborLength();

	realx3 lowerExt = 
		boundary(0).boundaryExtensionLength() + 
		boundary(2).boundaryExtensionLength() + 
		boundary(4).boundaryExtensionLength();
	
	realx3 upperExt = 
		boundary(1).boundaryExtensionLength()+
		boundary(3).boundaryExtensionLength()+
		boundary(5).boundaryExtensionLength();

	auto extDomain = pStruct_.simDomain().extendThisDomain(lowerExt, upperExt);
	pStruct_.updateFlag(
			extDomain,
			dist);
	const auto& maskD = pStruct_.activePointsMaskDevice();
	boundary(0).setSize( maskD.leftSize() );
	boundary(1).setSize( maskD.rightSize() );
	boundary(2).setSize( maskD.bottomSize() );
	boundary(3).setSize( maskD.topSize() );
	boundary(4).setSize( maskD.rearSize() );
	boundary(5).setSize( maskD.frontSize() );

	pStruct_.fillNeighborsLists(
		boundary(0).indexList().deviceViewAll(),
		boundary(1).indexList().deviceViewAll(),
		boundary(2).indexList().deviceViewAll(),
		boundary(3).indexList().deviceViewAll(),
		boundary(4).indexList().deviceViewAll(),
		boundary(5).indexList().deviceViewAll());

	return true;	
}

pFlow::boundaryList::boundaryList
(
    pointStructure& pStruct
)
: 
	ListPtr<boundaryBase>(pStruct.simDomain().sizeOfBoundaries()),
	pStruct_(pStruct),
	timeControl_
	(
		pStruct.simDomain().subDict("boundaries"), 
		"update",
		pStruct.currentTime()
	)
{}

bool pFlow::boundaryList::updateLists(uint32 iter, real t, real dt)
{
	if( timeControl_.timeEvent(iter, t, dt))
	{
		return updateLists();
	}
	return true;
}

bool pFlow::boundaryList::setLists()
{
	if(listSet_)return true;

	for(auto i=0; i<pStruct_.simDomain().sizeOfBoundaries();i++)
	{
		this->set
		(
			i,
			boundaryBase::create
			(
				pStruct_.simDomain().boundaryDict(i),
				pStruct_.simDomain().boundaryPlane(i),
				pStruct_,
				*this,
				i
			)
		);
	}
	listSet_ = true;

	return true;
}

bool pFlow::boundaryList::beforeIteration
(
	uint32 iter, 
	real t, 
	real dt
)
{
	// it is time to update lists 
	if(timeControl_.timeEvent(iter, t, dt))
	{
		if(!updateLists())
		{
			fatalErrorInFunction;
			return false;
		}
	}

	for(auto& bdry:*this)
	{
		if( !bdry->beforeIteration(iter, t, dt))
		{
			fatalErrorInFunction<<
			"Error in beforeIteration in boundary "<<bdry->name()<<endl;
			return false;
		}
		
	}

    return true;
}

bool pFlow::boundaryList::iterate
(
	uint32 iter, 
	real t, 
	real dt
)
{
	for(auto& bdry:*this)
	{
		if( !bdry->iterate(iter, t, dt))
		{
			fatalErrorInFunction<<
			"Error in iterate in boundary "<<bdry->name()<<endl;
			return false;
		}
	}
    return true;
}

bool pFlow::boundaryList::afterIteration
(
	uint32 iter, 
	real t, 
	real dt
)
{
    for(auto& bdry:*this)
	{
		if( !bdry->afterIteration(iter, t, dt))
		{
			fatalErrorInFunction<<
			"Error in afterIteration in boundary "<<bdry->name()<<endl;
			return false;
		}
	}
	return true;
}
