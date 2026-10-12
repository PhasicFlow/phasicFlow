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

#include "multiReactiveShrinkingDEMSystem.hpp"
#include "multiReactiveSphereShape.hpp"
#include "vocabs.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

bool multiReactiveShrinkingDEMSystem::loop()
{
    // Runs the steps from the current time up to, but not including,
    // the stop time of this call.
    while (!Control().time().reachedStopAt())
    {
        // Particle injection
        if (!insertion_().insertParticles(
                Control().time().currentIter(),
                Control().time().currentTime(),
                Control().time().dt()))
        {
            fatalError << "Particle insertion failed "
                       << "in multiReactiveShrinkingDEMSystem.\n";
            return false;
        }

        // Reset accumulators
        geometry_->beforeIteration();
        interaction_->beforeIteration();
        multiReactivePrtcl_->beforeIteration();

        // Mechanical collisions
        interaction_->iterate();

        // Q_pp, Q_pfp, Q_rad
        if (thermalInteraction_)
        {
            thermalInteraction_->iterate();
        }

        // Reaction rates, gas sources, reaction heat, composition
        if (multiShrinkingInt_)
        {
            multiShrinkingInt_->iterate();
        }

        // Motion + energy integration
        multiReactivePrtcl_->iterate();

        // Finalize
        geometry_->iterate();
        multiReactivePrtcl_->afterIteration();
        geometry_->afterIteration();

        Control()++;
    }

    return true;
}

//------------------------------- constructors --------------------------------

multiReactiveShrinkingDEMSystem::multiReactiveShrinkingDEMSystem
(
    word                    demSystemName,
    const std::vector<box>& domains,
    int                     argc,
    char*                   argv[],
    bool                    requireRVel
)
:
    thermalSphereDEMSystem(demSystemName, domains, argc, argv, requireRVel)
{
    REPORT(0) << "\nInitializing multi-species reactive DEM components..."
              << END_REPORT;

    // Discards the thermal build of the base class. Its particles are held
    // by a raw pointer and are deleted here, so that the reactive particles
    // can register their fields under the same names.
    thermalInteraction_.reset();
    interaction_.reset();
    insertion_.reset();
    particles_.reset();
    delete thermalParticles_;
    thermalParticles_ = nullptr;
    spheres_.reset();

    multiProp_ = makeUnique<multiSpeciesProperty>(
        propertyFile__,
        Control().caseSetup().path());

    auto* reactiveShape = new multiReactiveSphereShape(
        shapeFile__,
        &Control().caseSetup(),
        *multiProp_);
    spheres_ = uniquePtr<sphereShape>(reactiveShape);

    multiReactivePrtcl_ = new multiReactiveSphereParticles(
        Control(),
        *reactiveShape);
    thermalParticles_ = multiReactivePrtcl_;

    insertion_ = makeUnique<sphereInsertion>(
        *multiReactivePrtcl_,
        multiReactivePrtcl_->spheres());

    interaction_ = interaction::create(
        Control(),
        *multiReactivePrtcl_,
        Geometry());

    thermalInteraction_ = makeUnique<thermalInteraction>(
        Control(),
        *multiReactivePrtcl_,
        multiReactivePrtcl_->extendedDomain().domainBox());

    if (multiProp_->enableMultiReaction())
    {
        multiShrinkingInt_ = makeUnique<multiShrinkingInteraction>(
            Control(),
            *multiReactivePrtcl_,
            *multiProp_);
    }

    real minD, maxD;
    multiReactivePrtcl_->boundingSphereMinMax(minD, maxD);
    particleDistribution_ = makeUnique<domainDistribute>(domains, maxD);
}

//---------------------------- public methods ---------------------------------

bool multiReactiveShrinkingDEMSystem::beforeIteration()
{
    if (!thermalSphereDEMSystem::beforeIteration())
    {
        return false;
    }

    multiReactivePrtcl_->reactionDataHostUpdatedSync();

    return true;
}

bool multiReactiveShrinkingDEMSystem::iterate(
    real                    upToTime,
    real                    timeToWrite,
    word                    timeName)
{
    Control().time().setStopAt(upToTime);
    Control().time().setOutputToFile(timeToWrite, timeName);

    return loop();
}

bool multiReactiveShrinkingDEMSystem::iterate(real upToTime)
{
    Control().time().setStopAt(upToTime);

    return loop();
}

//--- reaction coupling interfaces -------------------------------------------

span<real> multiReactiveShrinkingDEMSystem::gasMassSource()
{
    auto& hVec = multiReactivePrtcl_->gasMassSourceHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> multiReactiveShrinkingDEMSystem::gasMassSourceSp()
{
    auto& hVec = multiReactivePrtcl_->gasMassSourceSpHost();
    return span<real>(hVec.data(), hVec.size());
}

span<real> multiReactiveShrinkingDEMSystem::gasConcentrations()
{
    auto& hVec = multiReactivePrtcl_->gasConcentrationsHost();
    return span<real>(hVec.data(), hVec.size());
}

bool multiReactiveShrinkingDEMSystem::sendGasConcentrationsToDEM()
{
    multiReactivePrtcl_->gasConcentrationsHostUpdatedSync();
    return true;
}

std::vector<std::string>
multiReactiveShrinkingDEMSystem::gasSpeciesNames() const
{
    const auto& names = multiProp_->gasSpeciesNames();

    return std::vector<std::string>(names.begin(), names.end());
}

std::vector<real> multiReactiveShrinkingDEMSystem::gasMolarMasses() const
{
    const auto& mw = multiProp_->gasMw();

    return std::vector<real>(mw.begin(), mw.end());
}

} // pFlow
