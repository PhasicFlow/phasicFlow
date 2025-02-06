
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
#include "anyList.hpp"

bool pFlow::pointStructure::setupPointStructure(const realx3Vector& points)
{
    PointsTypeHost hPoints
    (
        pointPosition().name(), 
        pointPosition().fieldKey()
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
        pointPosition().name(), 
        pointPosition().fieldKey(), 
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

    boundaries_.createBoundaries();
    boundaries_.updateNeighborLists(0u, true);

    return true;
}


bool pFlow::pointStructure::initializePoints(const PointsTypeHost &points)
{
    pointPosition().assignFromHost(points);

    createDeviceFlag(pointPosition().capacity(), 0, pointPosition().size());
    syncPFlag();

    return true;
}

pFlow::pointStructure::pointStructure
(
    systemControl& control, 
    real maxBSphere
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
		simulationDomain::create(control, maxBSphere)
	),
    //pointSorting_(simulationDomain_->subDictOrCreate("pointSorting")),
	boundaries_
	(
        *this
	),
    boundaryUpdateTimer_("boundaryUpdate", &timers()),
    boundaryDataTransferTimer_("boundaryDataTransferTimer", &timers())
{
    REPORT(1)<< "Reading "<< Green_Text("pointStructure")<<
    " from "<<IOobject::path()<<END_REPORT;

    if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }

    pointSorting_ = makeUnique<pointSorting>(simulationDomain_->subDictOrCreate("pointSorting"));
}


pFlow::pointStructure::pointStructure(
    systemControl& control,
    real maxBSphere, 
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
		simulationDomain::create(control, maxBSphere)
	),
    //pointSorting_(simulationDomain_->subDictOrCreate("pointSorting")),
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
    pointSorting_ = makeUnique<pointSorting>(simulationDomain_->subDictOrCreate("pointSorting"));
}

bool pFlow::pointStructure::beforeIteration()
{
    const timeInfo ti = TimeInfo();

    if(pointSorting_().sortTime(ti.iter(), ti.t(), ti.dt()))
    {
        auto sortedIndices = pointSorting_().getSortedIndices(
            simulationDomain_().globalBox(),
            pointPositionDevice(),
            activePointsMaskDevice()
        );
        
        if( !sortPoints(sortedIndices) )
        {
            fatalErrorInFunction;
            return false;
        }
        
        boundaryUpdateTimer_.start();
        boundaries_.beforeIteration(ti, true); 
        boundaryUpdateTimer_.end();

        INFORMATION<<"Reordering of particles has been done. New active range for particles is "<<
        activeRange()<<END_INFO; 


        message msg;
        anyList varList;

        varList.emplaceBack(
            msg.addAndName(message::ITEMS_REARRANGE),
            sortedIndices);

        if(!notify(ti, msg, varList))
        {
            fatalErrorInFunction<<
            "cannot notify for reordering items."<<endl;
            return false;
        }
        return true;
        // notify others about this change 
    }
    else
    {
        boundaryUpdateTimer_.start();
        if( !boundaries_.beforeIteration(ti) )
        {
            fatalErrorInFunction<<
            "Unable to perform beforeIteration for boundaries"<<endl;
            return false;
        }
        boundaryUpdateTimer_.end();
    }
    
    return true;
}

bool pFlow::pointStructure::iterate()
{
    if( !boundaries_.iterate(TimeInfo()) )
    {
        fatalErrorInFunction<<
        "Unable to perform iterate for boundaries"<<endl;
        return false;
    }
    
    return true;
}

bool pFlow::pointStructure::afterIteration()
{
    boundaryDataTransferTimer_.start();
    if( !boundaries_.afterIteration(TimeInfo()) )
    {
        fatalErrorInFunction<<
        "Unable to perform afterIteration for boundaries"<<endl;
        return false;
    }
    boundaryDataTransferTimer_.end();
    return true;
}

bool pFlow::pointStructure::read(
    iIstream &is,
    const IOPattern& iop)
{
    
    PointsTypeHost fRead
    (
        this->pointPosition().name(),
        this->pointPosition().fieldKey()
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
    return internalPoints::write(os, iop);
}
