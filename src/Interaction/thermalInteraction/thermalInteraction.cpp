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

    // Independent of radiation's own radUpdateInterval: gates the
    // shared mapper rebuild, which conduction/PFP rely on too.
    neighborListUpdateInterval_ =
        thermoDict.getValMax<uint32>("neighborListUpdateInterval", 1);
    
    //--- radiation -------------------------------------------------------
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
    Logical enablePP  = thermoDict.getVal<Logical>("enablePP");
    Logical enablePFP = thermoDict.getVal<Logical>("enablePFP");

    if (enablePP)
    {
        REPORT(0) << "Creating Collisional Heat Transfer (Q_p-p) model . . ." 
            << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> Collisional Heat Transfer is disabled.") 
            << END_REPORT;
    }

    if (enablePFP)
    {
        REPORT(0) << "Creating Particle-Fluid-Particle (PFP) "
                  << "sub-grid Heat Transfer model . . ." << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> PFP Heat Transfer is disabled.") 
            << END_REPORT;
    }

    if (enablePP || enablePFP)
    {
        condPfpMech_ = makeUnique<thermalConductionPFPMechanism>(
            thermoDict,
            static_cast<bool>(enablePP),
            static_cast<bool>(enablePFP));
    }

    //--- neighbor search cell size ---------------------------------------
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
    
    // adjustableBox = true: a particle outside domainBox grows/
    // repositions the search box (or trips the fatalErrorInFunction
    // in iterate() below if it cannot), instead of indexing outside
    // the allocated cell arrays.
    mapper_ = makeUnique<mapperNBS>(
        domainBox,
        cellSize,
        particles_.pointPosition().deviceViewAll(),
        particles_.dynPointStruct().activePointsMaskDevice(),
        true,
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

    auto flags = particles_.dynPointStruct().activePointsMaskDevice();
    auto pos   = particles_.pointPosition().deviceViewAll();

    thermalKernelTimer_.start();

    if (radiationMech_)
    {
        radiationMech_->iterate(
            flags,
            pos,
            particles_.temperature().deviceViewAll(),
            *mapper_,
            particles_.radSumTemp().deviceViewAll(),
            particles_.radNumPrt().deviceViewAll());
    }

    if (condPfpMech_)
    {
        condPfpMech_->iterate(
            flags,
            pos,
            particles_.diameter().deviceViewAll(),
            particles_.temperature().deviceViewAll(),
            particles_.conductivity().deviceViewAll(),
            particles_.E0().deviceViewAll(),
            particles_.nu().deviceViewAll(),
            particles_.fluidKappa().deviceViewAll(),
            particles_.fluidAlpha().deviceViewAll(),
            *mapper_,
            particles_.heatSourceCondPP().deviceViewAll(),
            particles_.heatSourcePFP().deviceViewAll());
    }

    thermalKernelTimer_.end();

    thermalTimer_.end();
    stepCounter_++;
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow

