
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


bool pFlow::pointStructure::setupPointStructure(const realx3Vector &points)
{
    if(!simulationDomain_->initialUpdateDomains(points.getSpan()))
    {
        fatalErrorInFunction<<
        "error in updating domains"<<endl;
        return false;
    }
    
    uint32 thisN = simulationDomain_->initialNumberInThis();

    Field<Vector, realx3 , vecAllocator<realx3>> internal
    (
        "internalPoints", 
        "internalPoints", 
        thisN, 
        thisN, 
        RESERVE()
    );
    
    auto pSpan  = makeSpan(points); 
    auto iSpan  = internal.getSpan();
    
    if(!simulationDomain_->initialTransferBlockData(pSpan, iSpan))
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

    boundaries_.updateLists();

    return false;
}


bool pFlow::pointStructure::initializePoints(const realx3Vector &points)
{
    pointPosition_.assign(points.vectorField());

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
		simulationDomain::create(control.domainDict())
	),
	boundaries_
	(
		simulationDomain_(),
		*this
	)
{
    REPORT(0)<< "Reading point structure from file"<<
    IOobject::localPath()<<END_REPORT;

    if( !IOobject::read() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::localPath()<<endl;
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
		simulationDomain::create(control.domainDict())
	),
	boundaries_
	(
		simulationDomain_(),
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
    
    Field<Vector, realx3 , vecAllocator<realx3>> 
        fRead("file_internalPoints", "internalPoints");   

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
    
    hostViewType1D<realx3> pointsH;

    if(isAllActive())
    {
        pointsH = pointPositionHost();
    }
    else
    {
        pointsH = activePointsHost();
    }

    auto data = span<realx3>(pointsH.data(), pointsH.size());
    
    if( !writeSpan(os, data, iop) )
    {
        fatalErrorInFunction<<
        "Error in writing pointStructure in stream "<<
        os.name()<<endl;
        return false;
    }

    return true;
}
