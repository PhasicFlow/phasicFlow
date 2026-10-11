/*------------------------------- phasicFlow ---------------------------------
      O        C enter of        O O        E ngineering and
    O   O      M ultiscale modeling of   OOOOOOO
     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.
-----------------------------------------------------------------------------*/

/**
 * @file multiSpeciesGranFlow.cpp
 * @brief Standalone DEM solver for granular flows with thermal and chemical
 * reactions, solving the energy equation and multi-species Arrhenius
 * kinetics via Kokkos kernels.
 *
 * Note: Q_conv is always 0 here (needs a real fluid velocity). Q_pfp is 0
 * unless thermoPhysicalInteraction/fluidProperties{kappa,alpha} is set, in
 * which case it uses those as an ambient stand-in. Radiation's
 * neighbourhood sum computes here too if enabled, but has no effect on
 * temperature standalone -- the actual flux is CFD-side only (see
 * sphereHeatTransfer). Use reactingSpherePFPlus to test radiation's
 * thermal effect.
 */

#include "vocabs.hpp"
#include "phasicFlowKokkos.hpp"
#include "systemControl.hpp"
#include "commandLine.hpp"
#include "property.hpp"
#include "geometry.hpp"
#include "sphereParticles.hpp"
#include "interaction.hpp"
#include "Insertions.hpp"

// --- Multi-Reactive Additions ---
#include "multiSpeciesProperty.hpp"
#include "multiReactiveSphereShape.hpp"
#include "multiReactiveSphereParticles.hpp"
#include "thermalInteraction.hpp"
#include "multiShrinkingInteraction.hpp"

/**
 * @brief Main execution entry point for the standalone multi-reactive DEM
 * solver.
 */
int main(int argc, char* argv[])
{
    pFlow::commandLine cmds
    (
        "multiSpeciesGranFlow",
        "DEM solver for reacting spherical particles with multi-species "
        "kinetics."
    );

    bool isCoupling = false;
    if (!cmds.parse(argc, argv))
    {
        return 0;
    }

    // Initialize parallel processing environments (MPI)
    pFlow::processors::initProcessors(argc, argv);
    pFlow::initialize_pFlowProcessors();

    // Bootstrap Kokkos and system repositories
    #include "initialize_Control.hpp"

    /// @brief Read global multi-species properties from the case directory.
    auto proprties = pFlow::multiSpeciesProperty
    (
        pFlow::propertyFile__,
        Control.caseSetup().path()
    );

    /// @brief Initialize the static/dynamic boundary geometries.
    #include "setSurfaceGeometry.hpp"

    /// @brief Initialize DEM components (thermal and shrinking interactions).
    #include "createDEMComponents.hpp"

    REPORT(0)
        << "\n[INFO] Standalone Thermal-Reactive Mode Active.\n"
        << "  Q_pp   (contact conduction) : Computed via Kokkos kernel\n"
        << "  Q_rad  (radiation)          : Neighbourhood sum computed "
        << "if enabled, but has NO effect on temperature here -- actual "
        << "flux is CFD-side only; use a coupled solver for radiation "
        << "to matter\n"
        << "  Q_pfp  (fluid bridge)       : 0 by default; computed "
        << "against thermoPhysicalInteraction/fluidProperties (ambient "
        << "kappa/alpha) if that block is set\n"
        << "  Q_conv (convection)         = 0 (no Eulerian fluid mesh; "
        << "no ambient stand-in exists for it)\n"
        << "  Q_rxn  (reaction heat)      : Computed via Arrhenius kernel\n"
        << "  Use reactingSpherePFPlus for fully coupled CFD-DEM simulations.\n"
        << END_REPORT;

    REPORT(0) << "\nStart of multi-reactive time loop . . .\n" << END_REPORT;

    do
    {
        // 4.1 Particle Injection
        if (!sphInsertion.insertParticles(
                Control.time().currentIter(),
                Control.time().currentTime(),
                Control.time().dt()))
        {
            fatalError << "Particle insertion failed in solver.\n";
            return 1;
        }

        // 4.2 Pre-Processing (reset forces, zero mass sources)
        surfGeometry.beforeIteration();
        sphParticles.beforeIteration();
        sphInteraction.beforeIteration();

        // 4.3 Physics Iteration
        sphInteraction.iterate();     // Mechanical collisions

        // Thermal physics (Conduction, Radiation, PFP)
        thermalInt.iterate();         

        // Reaction kinetics: Arrhenius GPU kernel updates solidFracs,
        // gasMassSource, and reactionHeat for all active particles.
        if (multiShrinkingInt)
        {
            multiShrinkingInt->iterate();
        }

        surfGeometry.iterate();
        sphParticles.iterate();       // Integrates equations of motion & state

        // 4.4 Post-Processing
        sphInteraction.afterIteration();
        surfGeometry.afterIteration();
        sphParticles.afterIteration();

    } while (Control++);  // Advances time and checks stop criteria

    REPORT(0) << "\nEnd of time loop.\n" << END_REPORT;

    #include "finalize.hpp"
    pFlow::processors::finalizeProcessors();

    return 0;
}
