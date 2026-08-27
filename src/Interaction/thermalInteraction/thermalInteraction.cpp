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

#include "thermalInteraction.hpp"

namespace pFlow 
{

//----------------------------- constructors ----------------------------------

thermalInteraction::thermalInteraction(
    systemControl&                  control, 
    const thermalSphereParticles&   particles, 
    const box&                      domainBox)
: 
    control_(control),
    particles_(particles),
    thermalTimer_("thermalInteraction", &control.timers()),
    neighborSearchTimer_("thermalInteractionSearch", &control.timers()),
    thermalKernelTimer_("thermalInteractionKernel", &control.timers())
{
    dictionary thermoDict(
        "thermoPhysicalInteraction", 
        control_.caseSetup().path() + "thermoPhysicalInteraction");

    // Mandatory regardless of which mechanisms end up enabled: gates
    // the shared mapper rebuild below, which conduction/PFP rely on
    // too, not just radiation. Independent of radiation's own
    // radUpdateInterval (read separately, inside
    // thermalRadiationMechanism's own constructor) -- these two
    // intervals serve different purposes (search-rebuild cost vs.
    // radiation's own physical update cadence) even though they
    // sound similar, so they are two separate dictionary entries.
    if (!thermoDict.containsDataEntry("neighborListUpdateInterval"))
    {
        fatalErrorInFunction
            << "Missing MANDATORY entry 'neighborListUpdateInterval' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }
    neighborListUpdateInterval_ =
        thermoDict.getVal<uint32>("neighborListUpdateInterval");

    if (neighborListUpdateInterval_ == 0)
    {
        fatalErrorInFunction
            << "'neighborListUpdateInterval' must be >= 1." << endl;
        fatalExit;
    }
    
    //--- radiation -------------------------------------------------------
    if (!thermoDict.containsDataEntry("enableRadiation"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'enableRadiation' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }
    
    Logical enableRad = thermoDict.getVal<Logical>("enableRadiation");
    
    if (enableRad)
    {
        REPORT(0) << "Creating Radiation interaction model . . ." << END_REPORT;
        radiationMech_ = makeUnique<thermalRadiationMechanism>(thermoDict);
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> Radiation is disabled by user.") 
            << END_REPORT;
    }

    //--- collisional heat conduction (Q_pp) / PFP -------------------------
    if (!thermoDict.containsDataEntry("enableConduction"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'enableConduction' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    Logical enableCond = thermoDict.getVal<Logical>("enableConduction");
    bool condOn = enableCond ? true : false;

    if (!thermoDict.containsDataEntry("enablePFP"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'enablePFP' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    Logical enablePfpFlag = thermoDict.getVal<Logical>("enablePFP");
    bool pfpOn = enablePfpFlag ? true : false;

    if (condOn)
    {
        REPORT(0) << "Creating Collisional Heat Transfer (Q_p-p) model . . ." 
            << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> Collisional Heat Transfer is disabled.") 
            << END_REPORT;
    }

    if (pfpOn)
    {
        REPORT(0) << "Creating Particle-Fluid-Particle (PFP) "
                  << "sub-grid Heat Transfer model . . ." << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> PFP Heat Transfer is disabled.") 
            << END_REPORT;
    }

    if (condOn || pfpOn)
    {
        condPfpMech_ = makeUnique<thermalConductionPFPMechanism>(
            thermoDict, condOn, pfpOn);
    }

    //--- neighbor search cell size ------------------------------------------
    // Must cover whichever constructed mechanism needs the longest
    // search radius, so mapperNBS's 27-cell sweep finds every
    // neighbour any active mechanism cares about.
    real searchCut = 0.0;

    if (radiationMech_)
    {
        searchCut = max(searchCut, radiationMech_->requiredSearchCut());
    }

    if (condPfpMech_)
    {
        searchCut = max(
            searchCut,
            condPfpMech_->requiredSearchCut(
                particles_.getShapes().maxBoundingSphere()));
    }
    
    real cellSize = (searchCut > 1e-12) 
                    ? searchCut 
                    : 3.0 * particles_.getShapes().maxBoundingSphere();

    REPORT(1) << "  Thermal interaction search cell size: " << cellSize << " m" 
        << END_REPORT;
    
    mapper_ = makeUnique<mapperNBS>(
        domainBox,
        cellSize,
        particles_.pointPosition().deviceViewAll(),
        particles_.dynPointStruct().activePointsMaskDevice(),
        false,  
        true);
}

//---------------------------- public methods ---------------------------------

void thermalInteraction::iterate()
{
    if (!radiationMech_ && !condPfpMech_)
    {
        return;
    }
    
    thermalTimer_.start();

    bool rebuildThisStep =
        (stepCounter_ % neighborListUpdateInterval_ == 0);

    if (rebuildThisStep)
    {
        bool boxChanged = false;

        neighborSearchTimer_.start();
        bool mapperBuiltOk = mapper_->build(
            particles_.pointPosition().deviceViewAll(),
            particles_.dynPointStruct().activePointsMaskDevice(),
            boxChanged);
        neighborSearchTimer_.end();

        if (!mapperBuiltOk)
        {
            fatalErrorInFunction
                << "[thermalInteraction] mapperNBS failed to build "
                << "at step " << stepCounter_ << ".\n"
                << "Likely cause: a burned-out ghost particle has an "
                << "extreme position that forces the search box "
                << "beyond allocatable limits." << endl;
            fatalExit;
        }
    }

    auto searchBox = mapper_->getSearchCells();
    auto domainMin = searchBox.domainBox().minPoint();
    auto cellSize  = searchBox.cellSize();
    int32x3 numCells(searchBox.nx(), searchBox.ny(), searchBox.nz());
    auto cellIter  = mapper_->getCellIterator();

    auto mask = particles_.dynPointStruct().activePointsMaskDevice();
    auto pos  = particles_.pointPosition().deviceViewAll();

    thermalKernelTimer_.start();

    if (radiationMech_)
    {
        radiationMech_->iterate(
            mask,
            pos,
            particles_.temperature().deviceViewAll(),
            cellIter,
            domainMin,
            cellSize,
            numCells,
            particles_.radSumTemp().deviceViewAll(),
            particles_.radNumPrt().deviceViewAll());
    }

    if (condPfpMech_)
    {
        condPfpMech_->iterate(
            mask,
            pos,
            particles_.diameter().deviceViewAll(),
            particles_.temperature().deviceViewAll(),
            particles_.conductivity().deviceViewAll(),
            particles_.E0().deviceViewAll(),
            particles_.nu().deviceViewAll(),
            particles_.fluidKappa().deviceViewAll(),
            particles_.fluidAlpha().deviceViewAll(),
            cellIter,
            domainMin,
            cellSize,
            numCells,
            particles_.heatSourceCondPP().deviceViewAll(),
            particles_.heatSourcePFP().deviceViewAll());
    }

    thermalKernelTimer_.end();

    thermalTimer_.end();
    stepCounter_++;
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
