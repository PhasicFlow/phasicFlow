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

REPORT(0) << "Reading shapes dictionary..." << END_REPORT;

pFlow::multiReactiveSphereShape spheres
(
    pFlow::shapeFile__,
    &Control.caseSetup(),
    proprties
);

REPORT(0) << "\nReading multi-reactive sphere particles . . ." << END_REPORT;

pFlow::multiReactiveSphereParticles sphParticles(Control, spheres);

REPORT(0) << "\nCreating particle insertion object . . ." << END_REPORT;

auto sphInsertion = pFlow::sphereInsertion
(
    sphParticles,
    sphParticles.spheres()
);

REPORT(0) << "\nCreating interaction model for sphere-sphere contact and "
          << "sphere-wall contact . . ." << END_REPORT;

auto interactionPtr = pFlow::interaction::create
(
    Control,
    sphParticles,
    surfGeometry
);

auto& sphInteraction = interactionPtr();

REPORT(0) << "\nCreating thermal interaction model "
          << "(Conduction, PFP, Radiation) . . ." << END_REPORT;

// The domain box of thermalInteraction must contain the particles, so it is
// the extended domain of the particles.
auto thermalIntPtr = pFlow::makeUnique<pFlow::thermalInteraction>
(
    Control,
    sphParticles,
    sphParticles.extendedDomain().domainBox()
);

auto& thermalInt = thermalIntPtr();

// Null when the multi-species reaction model is disabled
pFlow::uniquePtr<pFlow::multiShrinkingInteraction> multiShrinkingInt = nullptr;

if (proprties.enableMultiReaction())
{
    REPORT(0) << "\nCreating multi-species shrinking reaction model . . ."
              << END_REPORT;

    multiShrinkingInt = pFlow::makeUnique<pFlow::multiShrinkingInteraction>
    (
        Control,
        sphParticles,
        proprties
    );
}
