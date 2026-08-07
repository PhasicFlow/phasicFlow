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
 * Note: Q_conv (convection) and Q_pfp (fluid bridge) remain 0.0 since
 * the Eulerian fluid mesh does not exist in standalone mode. This mode
 * is ideal for unit-testing conduction (Q_pp) and radiation.
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
    // ===================================================================== //
    // Section 1: Initialization & CLI Parsing
    // ===================================================================== //
    pFlow::commandLine cmds
    (
        "heatSphereGranFlow",
        "DEM solver for non-cohesive spherical particles with heat "
        "transfer, particle insertion mechanism, and moving geometry."
    );

    bool isCoupling = false;

    if (!cmds.parse(argc, argv)) return 0;

    // this should be palced in each main
    pFlow::processors::initProcessors(argc, argv);
    pFlow::initialize_pFlowProcessors();

    #include "initialize_Control.hpp"

    // ===================================================================== //
    // Section 2: Material & Geometry Setup
    // ===================================================================== //

    /// Read global thermal properties from the case directory.
    auto proprties = pFlow::thermalProperty
    (
        pFlow::propertyFile__,
        Control.caseSetup().path()
    );

    #include "setSurfaceGeometry.hpp"

    #include "createDEMComponents.hpp"

    // ===================================================================== //
    // Section 3: Solver Capabilities Notice
    // ===================================================================== //
    REPORT(0)
        << "\n[INFO] Standalone Thermal Mode Active.\n"
        << "  Q_pp  (contact conduction) : Computed via Kokkos kernel\n"
        << "  Q_rad (radiation)          : Computed via Kokkos kernel\n"
        << "  Q_conv / Q_pfp             = 0 (No Eulerian fluid mesh "
        << "present)\n"
        << "  No chemical reaction capability in this solver.\n"
        << "  Use multiSpeciesGranFlow for reacting flows, or\n"
        << "  unresolvedHeatSpherePFPlus for coupled CFD-DEM heat "
        << "transfer.\n"
        << END_REPORT;

    // ===================================================================== //
    // Section 4: Main Transient Time Loop
    // ===================================================================== //
    REPORT(0) << "\nStart of time loop . . .\n" << END_REPORT;

    do
    {
        // 4.1 Particle insertion phase
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

        // 4.2 Pre-processing updates (reset forces, predict, etc.)
        surfGeometry.beforeIteration();
        sphParticles.beforeIteration();
        sphInteraction.beforeIteration();

        // 4.3 Evaluate contact interactions (particle-particle, wall)
        sphInteraction.iterate();

        // 4.4 Thermal physics (conduction, radiation, PFP)
        thermalInt.iterate();

        // 4.5 Update boundary kinematics
        surfGeometry.iterate();

        // 4.6 Update particle kinematics and integrate temperature
        sphParticles.iterate();

        // 4.7 Post-processing cleanups
        sphInteraction.afterIteration();
        surfGeometry.afterIteration();
        sphParticles.afterIteration();

    } while (Control++);

    REPORT(0) << "\nEnd of time loop.\n" << END_REPORT;

    // this should be palced in each main
    #include "finalize.hpp"
    pFlow::processors::finalizeProcessors();

    return 0;
}



