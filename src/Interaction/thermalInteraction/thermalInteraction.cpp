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
#include "thermalInteractionKernels.hpp"

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

    if (radiationMech_)
    {
        radiationMech_->ensureMemory(particles_.size());
    }
}

//---------------------------- public methods ---------------------------------

void thermalInteraction::iterate()
{
    // Kept correctly sized every call, regardless of update interval.
    if (radiationMech_)
    {
        radiationMech_->ensureMemory(particles_.size());
    }

    if (!radiationMech_ && !condPfpMech_)
    {
        return;
    }
    
    thermalTimer_.start();
    bool boxChanged = false;

    neighborSearchTimer_.start();
    bool mapperBuiltOk = mapper_->build(
        particles_.pointPosition().deviceViewAll(),
        particles_.dynPointStruct().activePointsMaskDevice(),
        boxChanged);
    neighborSearchTimer_.end();

    if (!mapperBuiltOk)
    {
        output
            << "\n"
            << Yellow_Text("[thermalInteraction] WARNING — mapperNBS failed "
                           "to build")
            << " at step " << stepCounter_ << ".\n"
            << "  Likely cause: a burned-out ghost particle has an extreme "
            << "position\n"
            << "  that forces the search box beyond allocatable limits.\n"
            << "  Thermal interactions (Q_pp, Q_pfp, radiation) are SKIPPED "
            << "this step.\n"
            << endl;

        thermalTimer_.end();
        stepCounter_++;
        return;
    }

    auto searchBox = mapper_->getSearchCells();
    auto domainMin = searchBox.domainBox().minPoint();
    auto cellSize  = searchBox.cellSize();
    int32x3 numCells(searchBox.nx(), searchBox.ny(), searchBox.nz());

    bool hasRad   = (radiationMech_ != nullptr);
    bool calcCond = condPfpMech_ && condPfpMech_->conductionEnabled();
    bool calcPFP  = condPfpMech_ && condPfpMech_->pfpEnabled();

    bool doRadThisStep = hasRad &&
        (stepCounter_ % radiationMech_->updateInterval() == 0);

    real radCut   = hasRad ? radiationMech_->requiredSearchCut() : 0.0;
    real radCutSq = radCut * radCut;
    real simYM    = condPfpMech_ ? condPfpMech_->simYoungsModulus() : 0.0;

    deviceViewType1D<real>   radSumTempView;
    deviceViewType1D<uint32> radNumPrtView;
    if (hasRad)
    {
        radSumTempView = radiationMech_->radSumTemp();
        radNumPrtView  = radiationMech_->radNumPrt();
    }

    thermalKernelTimer_.start();

    // heatSourceCondPP_/heatSourcePFP_ are zeroed by
    // thermalSphereParticles itself, not here.

    // Dispatches to one of 8 template instantiations so every
    // disabled mechanism's branches compile out of the per-pair
    // kernel entirely. Runs once per iterate(), never per particle.
#define CALL_THERMAL_KERNEL(RAD, COND, PFP)                                 \
    thermalInteractionKernels::calcThermalInteractions<RAD, COND, PFP>(     \
        particles_.dynPointStruct().activePointsMaskDevice(),               \
        particles_.pointPosition().deviceViewAll(),                         \
        particles_.diameter().deviceViewAll(),                              \
        particles_.temperature().deviceViewAll(),                           \
        particles_.conductivity().deviceViewAll(),                          \
        particles_.E0().deviceViewAll(),                                    \
        particles_.nu().deviceViewAll(),                                    \
        particles_.fluidKappa().deviceViewAll(),                            \
        particles_.fluidAlpha().deviceViewAll(),                            \
        mapper_->getCellIterator(),                                        \
        domainMin,                                                         \
        cellSize,                                                          \
        numCells,                                                          \
        doRadThisStep,                                                     \
        radCutSq,                                                          \
        radSumTempView,                                                    \
        radNumPrtView,                                                     \
        simYM,                                                             \
        particles_.heatSourceCondPP().deviceViewAll(),                      \
        particles_.heatSourcePFP().deviceViewAll())

    if (hasRad)
    {
        if (calcCond)
        {
            if (calcPFP) { CALL_THERMAL_KERNEL(true, true, true);  }
            else         { CALL_THERMAL_KERNEL(true, true, false); }
        }
        else
        {
            if (calcPFP) { CALL_THERMAL_KERNEL(true, false, true);  }
            else         { CALL_THERMAL_KERNEL(true, false, false); }
        }
    }
    else
    {
        if (calcCond)
        {
            if (calcPFP) { CALL_THERMAL_KERNEL(false, true, true);  }
            else         { CALL_THERMAL_KERNEL(false, true, false); }
        }
        else
        {
            if (calcPFP) { CALL_THERMAL_KERNEL(false, false, true);  }
            else         { CALL_THERMAL_KERNEL(false, false, false); }
        }
    }

#undef CALL_THERMAL_KERNEL

    thermalKernelTimer_.end();

    thermalTimer_.end();
    stepCounter_++;
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
