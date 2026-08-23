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

//----------------------------- protected methods ------------------------------

void thermalInteraction::ensureRadiationMemory()
{
    // Plain views, not a registered PointField (see the class-level
    // doc comment on the "Radiation neighbourhood output" members in
    // the header for why): sizing must be checked explicitly rather
    // than relying on an automatic insert/delete hook.
    size_t newSize = particles_.size();

    if (radSumTemp_.extent(0) != newSize)
    {
        Kokkos::resize(radSumTemp_, newSize);
        Kokkos::resize(radNumPrt_,  newSize);
        resizeNoInit(radSumTempHost_, newSize);
        resizeNoInit(radNumPrtHost_,  newSize);
    }
}

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

    //--- collisional heat conduction (Q_pp) -------------------------------
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

    //--- particle-fluid-particle (PFP) sub-grid heat transfer -------------
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

    //--- Hertzian simulation-scale Young's modulus -------------------------
    // Used to compute the mechanical contact radius whenever two particles
    // touch. That contact radius feeds both the collisional conduction
    // rate (Q_pp) and the PFP contact-limit radius r_sij, regardless of
    // which of the two mechanisms triggered the calculation, so it must
    // be supplied whenever either is enabled.
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

    //--- neighbor search cell size ------------------------------------------
    // Must be at least as large as whichever enabled mechanism needs the
    // longest search radius: mapperNBS's 27-cell sweep around a
    // particle's own cell only finds every neighbour within a radius R
    // when cellSize >= R (the particle can sit anywhere in its own
    // cell, and any neighbour within R of it then falls, at most, one
    // cell away in each direction -- the standard cell-list
    // neighbour-search guarantee). Each enabled mechanism contributes
    // its own required radius via max(), one line each, so a future
    // fourth mechanism follows the same template instead of relying on
    // remembering to add a separate branch (conduction's radius was
    // missing here for exactly that reason until this fix).
    real searchCut = 0.0;

    if (enableRadiation_)
    {
        searchCut = max(searchCut, radCut_);
    }

    if (enableConduction_)
    {
        // Largest possible contact distance R_i+R_j between any two
        // particles in the case.
        searchCut = max(
            searchCut, 
            2.0 * particles_.getShapes().maxBoundingSphere());
    }

    if (enablePFP_)
    {
        searchCut = max(
            searchCut, 
            3.0 * particles_.getShapes().maxBoundingSphere());
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

    // Size radSumTemp_/radNumPrt_ (device + host) now, so they are
    // correctly sized as soon as this object exists, rather than
    // leaving them at their default zero size until the first
    // iterate() call.
    ensureRadiationMemory();
}

//---------------------------- public methods ---------------------------------

void thermalInteraction::iterate()
{
    // Checked unconditionally, every call, regardless of the
    // enable-flags early return below: keeps radSumTemp_/radNumPrt_
    // correctly sized irrespective of which thermal mechanisms are on.
    ensureRadiationMemory();

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
        // radSumTemp_/radNumPrt_ are now this class's own members
        // (moved from thermalSphereParticles -- see the "Radiation
        // neighbourhood output" comment in the header), not
        // particles_'s.
        radSumTemp_,
        radNumPrt_);

    thermalKernelTimer_.end();

    // Sync radSumTemp_/radNumPrt_ to host right after computing them,
    // so the host mirror is never stale. Harmless to call again from
    // outside afterwards (e.g. at a CFD-exchange boundary) -- see the
    // method's doc comment.
    radiationDataHostUpdatedSync();

    thermalTimer_.end();
    stepCounter_++;
}

void thermalInteraction::radiationDataHostUpdatedSync()
{
    ensureRadiationMemory();

    if (radSumTempHost_.size() == radSumTemp_.size() &&
        radNumPrtHost_.size()  == radNumPrt_.size())
    {
        Kokkos::deep_copy(radSumTempHost_, radSumTemp_);
        Kokkos::deep_copy(radNumPrtHost_,  radNumPrt_);
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
