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
 * @file createDEMComponents.hpp
 * @brief Initialization sequence for the thermal DEM simulation objects.
 *
 * @details
 * Instantiates the thermal shape, particle container, insertion
 * mechanism, and mechanical + thermal interaction models required
 * before the main time loop begins.
 */

// ========================================================================= //
// Section 1: Shape & Material Initialization
// ========================================================================= //

REPORT(0) << "Reading thermal shapes dictionary..." << END_REPORT;

/**
 * @brief Geometric + thermal shape dictionary.
 * Binds per-material thermal data from thermalProperty
 * to per-shape geometry properties.
 */
pFlow::thermalSphereShape spheres
(
    pFlow::shapeFile__,
    &Control.caseSetup(),
    proprties   // thermalProperty instance from the main solver
);

// ========================================================================= //
// Section 2: Particle Container Initialization
// ========================================================================= //

REPORT(0) << "\nReading thermal sphere particles . . ." << END_REPORT;

/**
 * @brief Main GPU-backed thermal particle container.
 */
pFlow::thermalSphereParticles sphParticles
(
    Control,
    spheres,
    spheres
);

// ========================================================================= //
// Section 3: Particle Insertion Mechanism
// ========================================================================= //

REPORT(0) << "\nCreating particle insertion object . . ." << END_REPORT;

/**
 * @brief Time-triggered particle injector.
 */
auto sphInsertion = pFlow::sphereInsertion
(
    sphParticles,
    sphParticles.spheres()
);

// ========================================================================= //
// Section 4: Mechanical Interaction Model
// ========================================================================= //

REPORT(0) << "\nCreating interaction model for sphere-sphere contact . . ."
          << END_REPORT;

/**
 * @brief Factory-instantiated contact-force model (e.g., Hertz-Mindlin).
 * Handles particle-particle and particle-wall collisions.
 */
auto interactionPtr = pFlow::interaction::create
(
    Control,
    sphParticles,
    surfGeometry
);

auto& sphInteraction = interactionPtr();

// ========================================================================= //
// Section 5: Thermal Interaction Model (Fixed for Standalone Mode)
// ========================================================================= //

REPORT(0) << "\nCreating unified thermal interaction model "
          << "(Conduction, PFP, Radiation) . . ." << END_REPORT;

/**
 * @brief Thermal physics dispatcher.
 * Computes Q_pp (Batchelor-O'Brien) and particle-particle radiation.
 */
auto thermalIntPtr = pFlow::makeUnique<pFlow::thermalInteraction>
(
    Control,
    sphParticles,
    pFlow::box()
);

auto& thermalInt = thermalIntPtr();



