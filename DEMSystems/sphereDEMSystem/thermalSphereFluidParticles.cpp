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
#include "thermalSphereParticlesKernels.hpp"

namespace pFlow
{

//----------------------------- protected methods -----------------------------

void thermalSphereFluidParticles::checkHostMemory()
{
    if (fluidForce_.size() != fluidForceHost_.size())
    {
        resizeNoInit(fluidForceHost_,  fluidForce_.size());
        resizeNoInit(fluidTorqueHost_, fluidTorque_.size());
    }

    if (temperature().size() != temperatureHost_.size())
    {
        resizeNoInit(temperatureHost_,    temperature().size());
        resizeNoInit(heatSourceConvHost_, heatSourceConv_.size());
        resizeNoInit(heatSourceRadHost_,  heatSourceRad_.size());
        resizeNoInit(emissivityHost_,     emissivity().size());
        resizeNoInit(fluidKappaHost_,     fluidKappa().size());
        resizeNoInit(fluidAlphaHost_,     fluidAlpha().size());
    }
}

//----------------------------- constructors ----------------------------------

thermalSphereFluidParticles::thermalSphereFluidParticles(
    systemControl&              control,
    const thermalSphereShape&   thShpShape)
:
    thermalSphereParticles(control, thShpShape),
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
        realx3(0, 0, 0)),
    // heatSourceConv_/heatSourceRad_ are READ_NEVER/WRITE_NEVER by
    // design: both are zeroed and fully recomputed by the CFD
    // coupling layer on every data exchange (see calculateHeatCoupling()
    // in thermalMomentumSphereUnresolvedCouplingSystem and
    // sphereHeatTransfer::calculateHeatTransfer()), so there is no
    // persistent state here for a dictionary read to seed. A value
    // given for either field in particlesDict.setFields is silently
    // ignored -- this is intentional, not a bug.
    heatSourceConv_(
        objectFile(
            "heatSourceConv", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0),
    heatSourceRad_(
        objectFile(
            "heatSourceRad", 
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER),
        dynPointStruct(), 
        0.0)
{
    checkHostMemory();

    // Initial host sync so temperature/emissivity are valid immediately
    // after construction. temperature is re-synced every CFD exchange
    // afterwards (see thermalSphereDEMSystem::beforeIteration());
    // emissivity never changes again after this, so this is its only
    // sync ever.
    temperatureHostUpdatedSync();
    emissivityHostUpdatedSync();
}

//---------------------------- public methods ---------------------------------

bool thermalSphereFluidParticles::beforeIteration()
{
    sphereParticles::beforeIteration();
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

    // Thermal (fluid-coupled): dispatches the fluid-coupled kernel
    // overload directly. Only the kernel itself is shared with the
    // standalone tier (thermalSphereParticles::iterateThermal()); this
    // dispatch wrapper is specific to the fluid-coupled tier, so it
    // lives here rather than on the base.
    auto mask = dynPointStruct().activePointsMaskDevice();

    temperatureRate().field().fill(0.0);

    heatTransferTimer().start();

    thermalSphereParticlesKernels::calcFluidParticleHeatTransfer(
        mask,
        diameter().deviceViewAll(),
        mass().deviceViewAll(),
        Cp().deviceViewAll(),
        temperature().deviceViewAll(),
        heatSourceConv_.deviceViewAll(),
        heatSourceRad_.deviceViewAll(),
        heatSourceCondPP().deviceViewAll(),
        heatSourcePFP().deviceViewAll(),
        temperatureRate().deviceViewAll());

    heatTransferTimer().end();

    temperatureIntegrationTimer().start();

    thermalSphereParticlesKernels::integrateTemperature(
        mask,
        control().time().dt(),
        temperature().deviceViewAll(),
        temperatureRate().deviceViewAll());

    temperatureIntegrationTimer().end();

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

void thermalSphereFluidParticles::heatSourcesHostUpdatedSync()
{
    checkHostMemory();

    bool sizeConv = 
        (heatSourceConvHost_.size() == heatSourceConv_.deviceView().size());
    bool sizeRad = 
        (heatSourceRadHost_.size() == heatSourceRad_.deviceView().size());

    if (sizeConv && sizeRad)
    {
        Kokkos::deep_copy(heatSourceConv_.deviceView(), heatSourceConvHost_);
        Kokkos::deep_copy(heatSourceRad_.deviceView(),  heatSourceRadHost_);
    }
}

void thermalSphereFluidParticles::fluidPropertiesHostUpdatedSync()
{
    checkHostMemory();

    bool sizeKappa = 
        (fluidKappaHost_.size() == fluidKappa().deviceView().size());
    bool sizeAlpha = 
        (fluidAlphaHost_.size() == fluidAlpha().deviceView().size());

    if (sizeKappa && sizeAlpha)
    {
        Kokkos::deep_copy(fluidKappa().deviceView(), fluidKappaHost_);
        Kokkos::deep_copy(fluidAlpha().deviceView(), fluidAlphaHost_);
    }
}

void thermalSphereFluidParticles::temperatureHostUpdatedSync()
{
    checkHostMemory();

    if (temperatureHost_.size() == temperature().deviceView().size())
    {
        Kokkos::deep_copy(temperatureHost_, temperature().deviceView());
    }
}

void thermalSphereFluidParticles::emissivityHostUpdatedSync()
{
    checkHostMemory();

    if (emissivityHost_.size() == emissivity().deviceView().size())
    {
        Kokkos::deep_copy(emissivityHost_, emissivity().deviceView());
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
