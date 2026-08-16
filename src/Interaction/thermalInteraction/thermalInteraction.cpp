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
    
    // ---------------------------------------------------------------------- //
    // Radiation
    // ---------------------------------------------------------------------- //
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
        enableRadiation_ = true;

        // radCut determines which particles are treated as radiating
        // neighbours of one another. A silently-defaulted value would
        // switch radiation off in effect while still reporting it as
        // enabled, so it must be supplied explicitly.
        if (!thermoDict.containsDataEntry("radCut"))
        {
            fatalErrorInFunction
                << "Parameter 'radCut' is mandatory when enableRadiation "
                << "is true.\nPlease add it to the thermoPhysicalInteraction "
                << "dictionary." << endl;
            fatalExit;
        }
        radCut_ = thermoDict.getVal<real>("radCut");

        radUpdateInterval_ = thermoDict.getValOrSet<uint32>(
            "radUpdateInterval", 
            1);
            
        if (radUpdateInterval_ == 0)
        {
            fatalErrorInFunction
                << "'radUpdateInterval' must be a positive integer, got 0." 
                << endl;
            fatalExit;
        }

        REPORT(0) << "Creating Radiation interaction model . . ." << END_REPORT;
    }
    else
    {
        enableRadiation_ = false;
        REPORT(0) << Yellow_Text("  -> Radiation is disabled by user.") 
            << END_REPORT;
    }

    // ---------------------------------------------------------------------- //
    // Collisional Heat Conduction (Q_pp)
    // ---------------------------------------------------------------------- //
    if (!thermoDict.containsDataEntry("enableConduction"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'enableConduction' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    Logical enableCond = thermoDict.getVal<Logical>("enableConduction");
    enableConduction_ = enableCond ? true : false;

    if (enableConduction_)
    {
        REPORT(0) << "Creating Collisional Heat Transfer (Q_p-p) model . . ." 
            << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> Collisional Heat Transfer is disabled.") 
            << END_REPORT;
    }

    // ---------------------------------------------------------------------- //
    // Particle-Fluid-Particle (PFP) Sub-grid Heat Transfer
    // ---------------------------------------------------------------------- //
    if (!thermoDict.containsDataEntry("enablePFP"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'enablePFP' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    Logical enablePfpFlag = thermoDict.getVal<Logical>("enablePFP");
    enablePFP_ = enablePfpFlag ? true : false;
    
    if (enablePFP_)
    {
        REPORT(0) << "Creating Particle-Fluid-Particle (PFP) "
                  << "sub-grid Heat Transfer model . . ." << END_REPORT;
    }
    else
    {
        REPORT(0) << Yellow_Text("  -> PFP Heat Transfer is disabled.") 
            << END_REPORT;
    }

    // ---------------------------------------------------------------------- //
    // Hertzian simulation-scale Young's modulus.
    //
    // Used to compute the mechanical contact radius whenever two particles
    // touch. That contact radius feeds both the collisional conduction
    // rate (Q_pp) and the PFP contact-limit radius r_sij, regardless of
    // which of the two mechanisms triggered the calculation, so it must
    // be supplied whenever either is enabled.
    // ---------------------------------------------------------------------- //
    if (enableConduction_ || enablePFP_)
    {
        if (thermoDict.containsDataEntry("simYoungsModulus"))
        {
            simYoungsModulus_ = thermoDict.getVal<real>("simYoungsModulus");
        }
        else
        {
            fatalErrorInFunction
                << "Parameter 'simYoungsModulus' is mandatory when "
                << "enableConduction or enablePFP is true.\n"
                << "Please add it to the thermoPhysicalInteraction dictionary." 
                << endl;
            fatalExit;
        }
    }

    // ---------------------------------------------------------------------- //
    // Determine the neighbor search cell size.
    // ---------------------------------------------------------------------- //
    real pfpCut    = 3.0 * particles_.getShapes().maxBoundingSphere();
    real searchCut = radCut_;
    
    if (enablePFP_ && pfpCut > searchCut) 
    {
        searchCut = pfpCut;
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
    if (!enableRadiation_ && !enableConduction_ && !enablePFP_)
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

    bool calcRad  =
        enableRadiation_ && (stepCounter_ % radUpdateInterval_ == 0);
    bool calcCond = enableConduction_;
    bool calcPFP  = enablePFP_;

    thermalKernelTimer_.start();

    if (calcCond)
    {
        Kokkos::deep_copy(particles_.heatSourceCondPP().deviceViewAll(), 0.0);
    }

    if (calcPFP)
    {
        Kokkos::deep_copy(particles_.heatSourcePFP().deviceViewAll(), 0.0);
    }
    
    thermalInteractionKernels::calcThermalInteractions(
        particles_.dynPointStruct().activePointsMaskDevice(),
        particles_.pointPosition().deviceViewAll(),
        particles_.velocity().deviceViewAll(),
        particles_.rVelocity().deviceViewAll(),
        particles_.diameter().deviceViewAll(),
        particles_.mass().deviceViewAll(),
        particles_.temperature().deviceViewAll(),
        particles_.Cp().deviceViewAll(),
        particles_.conductivity().deviceViewAll(),
        particles_.E0().deviceViewAll(),
        particles_.nu().deviceViewAll(),
        particles_.fluidKappa().deviceViewAll(),
        particles_.fluidAlpha().deviceViewAll(),
        mapper_->getCellIterator(),
        domainMin,
        cellSize,
        numCells,
        radCut_,
        simYoungsModulus_,
        calcRad,
        calcCond,
        calcPFP,
        particles_.heatSourceCondPP().deviceViewAll(),
        particles_.heatSourcePFP().deviceViewAll(),
        particles_.radSumTemp().deviceViewAll(),
        particles_.radNumPrt().deviceViewAll());

    thermalKernelTimer_.end();

    thermalTimer_.end();
    stepCounter_++;
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow




