
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
#include "systemControl.hpp"
#include "vocabs.hpp"

bool pFlow::pointStructure::setupPointStructure(const realx3Vector& points)
{
    PointsTypeHost hPoints
    (
        pointPosition_.name(), 
        pointPosition_.fieldKey()
    );

    hPoints.assign(points);
    
    return setupPointStructure(hPoints);
}

bool pFlow::pointStructure::setupPointStructure(const PointsTypeHost &points)
{
    if(!simulationDomain_->initialUpdateDomains(points.getSpan()))
    {
        fatalErrorInFunction<<
        "error in updating domains"<<endl;
        return false;
    }
    
    uint32 thisN = simulationDomain_->initialNumberInThis();

    PointsTypeHost internal
    (
        pointPosition_.name(), 
        pointPosition_.fieldKey(), 
        thisN, 
        thisN, 
        RESERVE()
    );
    
    auto pSpan  = points.getSpan();
    
    if(auto iSpan  = internal.getSpan(); 
        !simulationDomain_->initialTransferBlockData(pSpan, iSpan))
    {
        fatalErrorInFunction<<
        "Error in transfering the block data "<<endl;
        return false;
    }
    
    if( !initializePoints(internal) )
    {
        fatalErrorInFunction;
        return false;
    }

    boundaries_.setLists();

    return true;
}


bool pFlow::pointStructure::initializePoints(const PointsTypeHost &points)
{
    pointPosition_.assign(points);

    pFlagsD_ = pFlagTypeDevice(pointPosition_.capacity(), 0, pointPosition_.size());
    pFlagSync_ = false;
    syncPFlag();

    return true;
}

pFlow::pointStructure::pointStructure
(
    systemControl& control
)
:
    IOobject
    (
        objectFile
        (
            pointStructureFile__,
            "",
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS
        ),
        IOPattern::MasterProcessorDistribute,
        &control.time()
    ),
	demComponent("particlesCenterMass", control),
  	internalPoints(),
	simulationDomain_
	(
		simulationDomain::create(control)
	),
	boundaries_
	(
        *this
	)
{
    REPORT(0)<< "Reading "<< Green_Text("point structure")<<
    " from "<<IOobject::path()<<END_REPORT;

    if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
}


pFlow::pointStructure::pointStructure(
    systemControl& control,
    const realx3Vector &posVec)
:
	IOobject
    (
        objectFile
        (
            pointStructureFile__,
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_ALWAYS
        ),
        IOPattern::MasterProcessorDistribute,
        &control.time()
    ),
	demComponent("particlesCenterMass", control),
  	internalPoints(),
	simulationDomain_
	(
		simulationDomain::create(control)
	),
	boundaries_
	(
		*this
	)
{
    if(!setupPointStructure(posVec))
    {
        fatalErrorInFunction<<
        "Error in seting up pointStructure"<<endl;
        fatalExit;
    }
}

bool pFlow::pointStructure::beforeIteration()
{
    if( !boundaries_.beforeIteration(currentIter(), currentTime(), dt()) )
    {
        fatalErrorInFunction<<
        "Unable to perform beforeIteration for boundaries"<<endl;
        return false;
    }
    return true;
}

bool pFlow::pointStructure::iterate()
{
    return true;
}

bool pFlow::pointStructure::afterIteration()
{
    return true;
}

bool pFlow::pointStructure::read(
    iIstream &is,
    const IOPattern& iop)
{
    
    PointsTypeHost fRead
    (
        this->pointPosition_.name(),
        this->pointPosition_.fieldKey()
    );

	if( !fRead.read(is, iop))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

    return setupPointStructure(fRead);
}

bool pFlow::pointStructure::write
(
    iOstream& os, 
    const IOPattern& iop
)const
{
    output<<"bool pFlow::pointStructure::write bool pFlow::pointStructure::write bool pFlow::pointStructure::write"<<endl;
    return internalPoints::write(os, iop);
}
