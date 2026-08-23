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

/**
 * @file heatSphereGranFlow.cpp
 * @brief Standalone DEM solver for granular flow with heat transfer.
 *
 * @details
 * This solver simulates the granular flow of cohesion-less, spherical
 * particles while additionally solving the explicit particle energy
 * equation (contact conduction, radiation, and particle-fluid-particle
 * sub-grid heat transfer) via Kokkos kernels. It carries no chemical
 * reaction capability; for reacting flows, use multiSpeciesGranFlow,
 * which extends this same thermal layer.
 *
 * Note: Q_conv (convection) is always 0.0 in this standalone mode,
 * since Ranz-Marshall needs a local fluid velocity that only a real
 * Eulerian mesh can provide. Q_pfp is 0.0 only by default: if
 * thermoPhysicalInteraction/fluidProperties{kappa, alpha} is set, PFP
 * uses those as a uniform ambient fluid stand-in and does contribute.
 * This mode is ideal for unit-testing conduction (Q_pp), radiation,
 * and PFP against that ambient fallback.
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

// --- Thermal Additions ---
#include "thermalProperty.hpp"
#include "thermalSphereShape.hpp"
#include "thermalSphereParticles.hpp"
#include "thermalInteraction.hpp"

/**
 * @brief Main execution entry point for the standalone thermal DEM
 * solver.
 */
int main(int argc, char* argv[])
{
    //--- initialization & CLI parsing -------------------------------------
    pFlow::commandLine cmds
    (
        "heatSphereGranFlow",
        "DEM solver for non-cohesive spherical particles with heat "
        "transfer, particle insertion mechanism, and moving geometry."
    );

    if (!cmds.parse(argc, argv)) return 0;

    // this should be placed in each main
    pFlow::processors::initProcessors(argc, argv);
    pFlow::initialize_pFlowProcessors();

    #include "initialize_Control.hpp"

    //--- material & geometry setup -----------------------------------------

    /// Read global thermal properties from the case directory.
    auto proprties = pFlow::thermalProperty
    (
        pFlow::propertyFile__,
        Control.caseSetup().path()
    );

    #include "setSurfaceGeometry.hpp"

    #include "createDEMComponents.hpp"

    //--- solver capabilities notice -----------------------------------------
    REPORT(0)
        << "\n[INFO] Standalone Thermal Mode Active.\n"
        << "  Q_pp   (contact conduction) : Computed via Kokkos kernel\n"
        << "  Q_rad  (radiation)          : Computed via Kokkos kernel\n"
        << "  Q_pfp  (fluid bridge)       : 0 by default; computed "
        << "against thermoPhysicalInteraction/fluidProperties (ambient "
        << "kappa/alpha) if that block is set\n"
        << "  Q_conv (convection)         = 0 (Ranz-Marshall needs a "
        << "local fluid velocity; no ambient stand-in exists for it)\n"
        << "  No chemical reaction capability in this solver.\n"
        << "  Use multiSpeciesGranFlow for reacting flows, or\n"
        << "  unresolvedHeatSpherePFPlus for coupled CFD-DEM heat "
        << "transfer.\n"
        << END_REPORT;

    //--- main transient time loop -------------------------------------------
    REPORT(0) << "\nStart of time loop . . .\n" << END_REPORT;

    do
    {
        // Particle insertion phase
        if (!sphInsertion.insertParticles(
                Control.time().currentIter(),
                Control.time().currentTime(),
                Control.time().dt()))
        {
            fatalError
                << "particle insertion failed in heatSphereGranFlow "
                << "solver.\n";
            return 1;
        }

        // Pre-processing updates (reset forces, predict, etc.)
        surfGeometry.beforeIteration();
        sphParticles.beforeIteration();
        sphInteraction.beforeIteration();

        // Evaluate contact interactions (particle-particle, wall)
        sphInteraction.iterate();

        // Thermal physics (conduction, radiation, PFP)
        thermalInt.iterate();

        // Update particle kinematics and integrate temperature. Comes
        // before the geometry update below, matching sphereDEMSystem's
        // own loop() ordering (particles integrate against the current
        // step's geometry state, not next step's).
        sphParticles.iterate();

        // Update boundary kinematics
        surfGeometry.iterate();

        // Post-processing cleanups
        sphInteraction.afterIteration();
        surfGeometry.afterIteration();
        sphParticles.afterIteration();

    } while (Control++);

    REPORT(0) << "\nEnd of time loop.\n" << END_REPORT;

    // this should be placed in each main
    #include "finalize.hpp"
    pFlow::processors::finalizeProcessors();

    return 0;
}
