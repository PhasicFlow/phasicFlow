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

#include "thermalSphereDEMSystem.hpp"
#include "vocabs.hpp"

namespace pFlow
{

//----------------------------- protected methods -----------------------------

bool thermalSphereDEMSystem::loop()
{
    do 
    {
        // 3.1 Handle particle injection triggers
        if (!insertion_().insertParticles(
                Control().time().currentIter(),
                Control().time().currentTime(),
                Control().time().dt()))
        {
            fatalError << "Particle insertion failed "
                       << "in thermalSphereDEMSystem.\n";
            return false;
        }

        // 3.2 Initialize physics accumulators
        geometry_->beforeIteration();
        interaction_->beforeIteration();
        thermalParticles_->beforeIteration();
        
        // 3.3 Mechanical collision evaluation
        interaction_->iterate();

        // 3.4 Thermodynamic evaluation (Q_pp, Q_pfp, Q_rad)
        if (thermalInteraction_) 
        {
            thermalInteraction_->iterate();
        }

        // 3.5 Equations of motion (with fluid force) and energy integration
        thermalParticles_->iterate();

        // 3.6 Clean up and state finalization
        geometry_->iterate();
        thermalParticles_->afterIteration();
        geometry_->afterIteration();

    } while(Control()++);

    return true;
}

//----------------------------- constructors ----------------------------------

thermalSphereDEMSystem::thermalSphereDEMSystem(
    word                    demSystemName,
    const std::vector<box>& domains,
    int                     argc,
    char*                   argv[],
    bool                    requireRVel)
:
    sphereDEMSystem(demSystemName, domains, argc, argv, requireRVel)
{
    REPORT(0) << "\nInitializing thermal DEM components..." << END_REPORT;

    // Discard the base class's temporary mechanical-only build.
    // particles_ (inherited from sphereDEMSystem, typed
    // uniquePtr<sphereFluidParticles>) is reset to null and deliberately
    // left unused for the rest of this class's lifetime: thermalParticles_
    // (below) is the real, correctly-typed particles object from here on.
    // See the class-level doc comment in the header for why particles_
    // cannot simply be repointed at it.
    interaction_.reset();
    insertion_.reset();
    particles_.reset();
    spheres_.reset();

    auto thermalProps = thermalProperty(
        propertyFile__,
        Control().caseSetup().path());

    auto* combinedShape = new thermalSphereShape(
        shapeFile__,
        &Control().caseSetup(),
        thermalProps);
    spheres_ = uniquePtr<sphereShape>(combinedShape);

    // thermalSphereFluidParticles' constructor already calls
    // initializeThermalParticles() internally (via the inherited
    // thermalSphereParticles constructor body), so no separate
    // initialization call is needed here.
    auto* tp = new thermalSphereFluidParticles(
        Control(),
        *combinedShape,
        *combinedShape);
    thermalParticles_ = tp; 

    insertion_ = makeUnique<sphereInsertion>(
        *thermalParticles_,
        thermalParticles_->spheres());

    interaction_ = interaction::create(
        Control(),
        *thermalParticles_,
        Geometry());

    REPORT(0) << "Creating thermal interactions "
              << "(Conduction, Radiation, PFP)..." << END_REPORT;
    
    box localDomain = domains.empty() ? box() : domains[0];
    
    thermalInteraction_ = makeUnique<thermalInteraction>(
        Control(),
        *thermalParticles_,
        localDomain);

    real minD, maxD;
    thermalParticles_->boundingSphereMinMax(minD, maxD);
    particleDistribution_ = makeUnique<domainDistribute>(domains, maxD);
}

//---------------------------- public methods ---------------------------------

bool thermalSphereDEMSystem::iterate(
    real                    upToTime,
    real                    timeToWrite,
    word                    timeName)
{
    Control().time().setStopAt(upToTime);
    Control().time().setOutputToFile(timeToWrite, timeName);
    
    return loop();
}

bool thermalSphereDEMSystem::iterate(real upToTime)
{
    Control().time().setStopAt(upToTime);
    
    return loop();
}

// ========================================================================= //
// Overrides required because sphereDEMSystem's inherited implementation
// reads particles_ directly. Bodies below are adapted line-for-line from
// the verified sphereDEMSystem.cpp, substituting thermalParticles_ for
// particles_ (a raw pointer here, so no extra dereference).
// ========================================================================= //

bool thermalSphereDEMSystem::beforeIteration()
{
    velocityHost_     = std::as_const(*thermalParticles_).velocity().hostView();
    positionHost_     =
        std::as_const(*thermalParticles_).pointPosition().hostView();
    diameterHost_     = thermalParticles_->diameter().hostView();
    particleIdHost_   = thermalParticles_->particleId().hostView();
    accelerationHost_ = thermalParticles_->acceleration().hostView();

    if (requireRVel_)
    {
        rVelocityHost_ =
            std::as_const(*thermalParticles_).rVelocity().hostView();
    }

    return true;
}

real thermalSphereDEMSystem::maxBounndingSphereSize() const
{
    real minD, maxD;
    thermalParticles_->boundingSphereMinMax(minD, maxD);
    return maxD;
}

bool thermalSphereDEMSystem::updateParticleDistribution(
    real                    extentFraction,
    const std::vector<box>  domains)
{
    if (!particleDistribution_->changeDomainsSize(
            extentFraction,
            maxBounndingSphereSize(),
            domains))
    {
        fatalErrorInFunction << "Cannot change the domain size" << endl;
        return false;
    }

    if (!particleDistribution_->locateParticles(
            positionHost_,
            thermalParticles_->pStruct().activePointsMaskHost()))
    {
        fatalErrorInFunction
            << "Error in locating particles among sub-domains" << endl;
        return false;
    }

    return true;
}

span<realx3> thermalSphereDEMSystem::parFluidForce()
{
    auto& hVec = thermalParticles_->fluidForceHost();
    return span<realx3>(hVec.data(), hVec.size());
}

span<realx3> thermalSphereDEMSystem::parFluidTorque()
{
    auto& hVec = thermalParticles_->fluidTorqueHost();
    return span<realx3>(hVec.data(), hVec.size());
}

bool thermalSphereDEMSystem::sendFluidForceToDEM()
{
    thermalParticles_->fluidForceHostUpdatedSync();
    return true;
}

bool thermalSphereDEMSystem::sendFluidTorqueToDEM()
{
    thermalParticles_->fluidTorqueHostUpdatedSync();
    return true;
}

// ========================================================================= //
// Thermal & radiation coupling interfaces
// ========================================================================= //

span<real> thermalSphereDEMSystem::temperature()
{
    auto& hVec = thermalParticles_->temperatureHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> thermalSphereDEMSystem::emissivity()
{
    auto& hVec = thermalParticles_->emissivityHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> thermalSphereDEMSystem::radSumTemp()
{
    auto& hVec = thermalParticles_->radSumTempHost();
    return span<real>(hVec.data(), hVec.size());
}

span<uint32> thermalSphereDEMSystem::radNumPrt()
{
    auto& hVec = thermalParticles_->radNumPrtHost();
    return span<uint32>(hVec.data(), hVec.size());
}

span<real> thermalSphereDEMSystem::parFluidHeatSourceConv()
{
    auto& hVec = thermalParticles_->heatSourceConvHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> thermalSphereDEMSystem::parFluidHeatSourceRad()
{
    auto& hVec = thermalParticles_->heatSourceRadHost();
    return span<real>(hVec.data(), hVec.size());
}

bool thermalSphereDEMSystem::sendFluidHeatSourcesToDEM()
{
    thermalParticles_->heatSourcesHostUpdatedSync();
    return true;
}

span<real> thermalSphereDEMSystem::parFluidKappa()
{
    auto& hVec = thermalParticles_->fluidKappaHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> thermalSphereDEMSystem::parFluidAlpha()
{
    auto& hVec = thermalParticles_->fluidAlphaHost();
    return span<real>(hVec.data(), hVec.size());
}

bool thermalSphereDEMSystem::sendFluidPropertiesToDEM()
{
    thermalParticles_->fluidPropertiesHostUpdatedSync();
    return true;
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
