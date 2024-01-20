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
#include "internalPoints.hpp"
#include "simulationDomain.hpp"

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

	
	internal_.updateFlag(
			simDomain_.thisDomain(),
			dist);
	const auto& maskD = internal_.activePointsMaskDevice();
	boundary(0).setSize( maskD.leftSize() );
	boundary(1).setSize( maskD.rightSize() );
	boundary(2).setSize( maskD.bottomSize() );
	boundary(3).setSize( maskD.topSize() );
	boundary(4).setSize( maskD.rearSize() );
	boundary(5).setSize( maskD.frontSize() );

	internal_.fillNeighborsLists(
		boundary(0).indexList().deviceVectorAll(),
		boundary(1).indexList().deviceVectorAll(),
		boundary(2).indexList().deviceVectorAll(),
		boundary(3).indexList().deviceVectorAll(),
		boundary(4).indexList().deviceVectorAll(),
		boundary(5).indexList().deviceVectorAll());

	return true;	
}

pFlow::boundaryList::boundaryList
(
    const simulationDomain &simD,
    internalPoints &internal
)
: 
	ListPtr<boundaryBase>(simD.sizeOfBoundaries()),
	internal_(internal),
	simDomain_(simD),
	timeControl_
	(
		simDomain_.subDict("boundaries"), 
		"update"
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

	for(auto i=0; i<simDomain_.sizeOfBoundaries();i++)
	{
		this->set
		(
			i,
			boundaryBase::create
			(
				simDomain_.boundaryDict(i),
				simDomain_.boundaryPlane(i),
				internal_
			)
		);
	}
	listSet_ = true;

	return true;
}