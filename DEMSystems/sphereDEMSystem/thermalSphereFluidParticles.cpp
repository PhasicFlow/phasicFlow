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

#include "thermalSphereFluidParticles.hpp"

namespace pFlow
{

//----------------------------- protected methods -----------------------------

void thermalSphereFluidParticles::checkHostMemory()
{
    // Genuine base call: thermalSphereParticles::checkHostMemory() is not
    // virtual, so it will NOT be reached automatically through
    // thermalSphereParticles::beforeIteration()'s own internal
    // (statically-bound) call to checkHostMemory() -- it must be invoked
    // explicitly here too. Calling it twice per iteration (once from the
    // inherited beforeIteration(), once from this override) is harmless:
    // it is a guarded size check, a no-op once already sized correctly.
    thermalSphereParticles::checkHostMemory();

    if (fluidForce_.size() != fluidForceHost_.size())
    {
        size_t oldSize = fluidForceHost_.size();
        size_t newSize = fluidForce_.size();

        resizeNoInit(fluidForceHost_,  newSize);
        resizeNoInit(fluidTorqueHost_, newSize);

        for (size_t i = oldSize; i < newSize; ++i)
        {
            fluidForceHost_[i]  = fluidForce_.field()[i];
            fluidTorqueHost_[i] = fluidTorque_.field()[i];
        }
    }
}

//----------------------------- constructors ----------------------------------

thermalSphereFluidParticles::thermalSphereFluidParticles(
    systemControl&              control,
    const sphereShape&          shpShape,
    const thermalSphereShape&   thShpShape)
:
    thermalSphereParticles(control, shpShape, thShpShape),
    fluidForce_(
        objectFile(
            "fluidForce",
            "",
            objectFile::READ_IF_PRESENT,
            objectFile::WRITE_ALWAYS),
        dynPointStruct(),
        realx3(0, 0, 0)),
    fluidTorque_(
        objectFile(
            "fluidTorque",
            "",
            objectFile::READ_IF_PRESENT,
            objectFile::WRITE_ALWAYS),
        dynPointStruct(),
        realx3(0, 0, 0))
{
    checkHostMemory();
}

//---------------------------- public methods ---------------------------------

bool thermalSphereFluidParticles::beforeIteration()
{
    thermalSphereParticles::beforeIteration();
    checkHostMemory();

    return true;
}

bool thermalSphereFluidParticles::iterate()
{
    // Mechanical: acceleration (WITH fluid force/torque) + correct.
    // This dispatch is duplicated rather than inherited, since this
    // class does not derive from sphereFluidParticles (see class-level
    // doc comment in the header for why). The kernel itself
    // (sphereFluidParticlesKernels::acceleration) is NOT duplicated --
    // only these few lines that call it are.
    accelerationTimer().start();

    sphereFluidParticlesKernels::acceleration(
        control().g(),
        mass().deviceViewAll(),
        contactForce().deviceViewAll(),
        fluidForce_.deviceViewAll(),
        I().deviceViewAll(),
        contactTorque().deviceViewAll(),
        fluidTorque_.deviceViewAll(),
        pStruct().activePointsMaskDevice(),
        acceleration().deviceViewAll(),
        rAcceleration().deviceViewAll());

    accelerationTimer().end();

    intCorrectTimer().start();

    dynPointStruct().correct(control().time().dt());
    rVelIntegration().correct(
        control().time().dt(), rVelocity(), rAcceleration());

    intCorrectTimer().end();

    // Thermal: reuse the genuinely-inherited kernel dispatch, unchanged.
    iterateThermal();

    return true;
}

void thermalSphereFluidParticles::fluidForceHostUpdatedSync()
{
    checkHostMemory();

    if (fluidForceHost_.size() == fluidForce_.deviceView().size())
    {
        Kokkos::deep_copy(fluidForce_.deviceView(), fluidForceHost_);
    }
}

void thermalSphereFluidParticles::fluidTorqueHostUpdatedSync()
{
    checkHostMemory();

    if (fluidTorqueHost_.size() == fluidTorque_.deviceView().size())
    {
        Kokkos::deep_copy(fluidTorque_.deviceView(), fluidTorqueHost_);
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
