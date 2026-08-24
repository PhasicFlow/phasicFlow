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

//--- shape & material initialization -------------------------------------

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

//--- particle container initialization -----------------------------------

REPORT(0) << "\nReading thermal sphere particles . . ." << END_REPORT;

/**
 * @brief Main GPU-backed thermal particle container.
 *
 * thermalSphereParticles, not thermalSphereFluidParticles: no CFD
 * mesh here, so fluid-momentum coupling is unneeded. spheres is
 * passed once since thermalSphereShape already IS a sphereShape.
 */
pFlow::thermalSphereParticles sphParticles
(
    Control,
    spheres
);

//--- particle insertion mechanism -----------------------------------------

REPORT(0) << "\nCreating particle insertion object . . ." << END_REPORT;

/**
 * @brief Time-triggered particle injector.
 */
auto sphInsertion = pFlow::sphereInsertion
(
    sphParticles,
    sphParticles.spheres()
);

//--- mechanical interaction model ------------------------------------------

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

//--- thermal interaction model (fixed for standalone mode) ----------------

REPORT(0) << "\nCreating unified thermal interaction model "
          << "(Conduction, PFP, Radiation) . . ." << END_REPORT;

/**
 * @brief Thermal physics dispatcher (conduction, PFP, radiation).
 * Takes a const thermalSphereParticles&, so sphParticles binds
 * directly.
 */
auto thermalIntPtr = 
    pFlow::makeUnique<pFlow::thermalInteraction>
    (
        Control,
        sphParticles,
        pFlow::box()
    );

auto& thermalInt = thermalIntPtr();
