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

#include "pointStructure.hpp"
#include "simulationDomain.hpp"

#include "streams.hpp"

pFlow::pointStructure::pointStructure
(
  const dictionary& domainDict
)
:
  internalPoints(),
  simulationDomain_
  (
    simulationDomain::create(domainDict)
  ),
  boundaries_
  (
    simulationDomain_(),
    *this
  )
{}

bool pFlow::pointStructure::distributePoints
(
	const realx3Vector &points
)
{
	uint32 thisN = simulationDomain_->thisNumPoints();

    Field<Vector, realx3 , vecAllocator<realx3>> internal(
        "internalPoints", 
        "internalPoints", 
        thisN, 
        thisN, 
        RESERVE());
    
    
    auto l = sizeof(realx3);
    auto pointsSpan = span<char>(
        reinterpret_cast<char*>
		(
			const_cast<realx3*>(points.data())
		), 
        points.size()*l );

    auto internalSpan = span<char>(reinterpret_cast<char*>(internal.data()), internal.size()*l);

    if(!simulationDomain_->transferBlockData(pointsSpan, internalSpan, l))
    {
        fatalErrorInFunction<<
        "Error in transfering the block data "<<endl;
        return false;
    }
    
    pointPosition_.assign(internal.vectorField());

    
    pFlagsD_ = pFlagTypeDevice(pointPosition_.capacity(), 0, pointPosition_.size());
    pFlagSync_ = false;
    syncPFlag();
    
    

    return true;

}

pFlow::pointStructure::pointStructure(
    const dictionary &domainDict,
    const realx3Vector &posVec)
    : internalPoints(),
      simulationDomain_(
          simulationDomain::create(domainDict)),
      boundaries_(
          simulationDomain_(),
          *this)
{
    if(!simulationDomain_->updateDomains(posVec) )
    {
        fatalErrorInFunction<<
        "Failed to update domains."<<endl;
        fatalExit;
    }

    boundaries_.updateLists();

    if( !distributePoints(posVec) )
    {
        fatalErrorInFunction<<"Faild to distributes poinst"<<endl;
        fatalExit;
    }

    
}

FUNCTION_H
bool pFlow::pointStructure::read
(
    iIstream& is, 
    IOPattern::IOType iotype
)
{
    Field<Vector, realx3 , vecAllocator<realx3>> fRead("file_internalPoints", "internalPoints");
    
    

	if( !fRead.read(is, iotype))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

    if(!simulationDomain_->updateDomains(fRead))
    {
        fatalErrorInFunction<<
        "error in updating domains"<<endl;
        return false;
    }

    boundaries_.updateLists();

	return distributePoints(fRead);
    
}