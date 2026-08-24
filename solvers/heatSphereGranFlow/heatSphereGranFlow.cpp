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
 * @brief Standalone DEM solver for granular flow with heat transfer
 * (conduction, radiation, PFP) via Kokkos kernels. No chemical
 * reaction capability -- see multiSpeciesGranFlow for that.
 *
 * Note: Q_conv is always 0 here (needs a real fluid velocity). Q_pfp
 * is 0 unless thermoPhysicalInteraction/fluidProperties{kappa,alpha}
 * is set, in which case it uses those as an ambient stand-in.
 * Radiation's neighbourhood sum computes here too if enabled, but has
 * no effect on temperature standalone -- the actual flux is CFD-side
 * only (see sphereHeatTransfer). Use a coupled solver to test
 * radiation's thermal effect.
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
        << "  Q_rad  (radiation)          : Neighbourhood sum computed "
        << "if enabled, but has NO effect on temperature here -- actual "
        << "flux is CFD-side only; use a coupled solver for radiation "
        << "to matter\n"
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

        // Particles integrate before geometry updates, matching
        // sphereDEMSystem's own loop() ordering.
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
