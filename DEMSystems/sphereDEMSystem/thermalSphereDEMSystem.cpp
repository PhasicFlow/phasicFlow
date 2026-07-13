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
-----------------------------------------------------------------------------*/

#include "thermalSphereDEMSystem.hpp"
#include "vocabs.hpp"

namespace pFlow
{

// ========================================================================= //
// Section 1: Constructors
// ========================================================================= //

pFlow::thermalSphereDEMSystem::thermalSphereDEMSystem
(
    word                    demSystemName,
    const std::vector<box>& domains,
    int                     argc, 
    char* argv[],
    bool                    requireRVel
)
:
    sphereDEMSystem(demSystemName, domains, argc, argv, requireRVel)
{
    REPORT(0) << "\nInitializing thermal DEM components..." << END_REPORT;

    // Reset base instances for thermal override
    interaction_.reset();
    insertion_.reset();
    particles_.reset();
    spheres_.reset();

    // 1.1 Load thermal properties
    auto thermalProps = thermalProperty
    (
        propertyFile__, 
        Control().caseSetup().path()
    );

    // 1.2 Initialize thermal shapes
    auto* combinedShape = new thermalSphereShape
    (
        pFlow::shapeFile__, 
        &Control().caseSetup(), 
        thermalProps
    );
    
    spheres_ = uniquePtr<sphereShape>(combinedShape);

    // 1.3 Initialize thermal particles on GPU
    auto* tp = new thermalSphereParticles
    (
        Control(), 
        *combinedShape, 
        *combinedShape
    );
    
    particles_ = uniquePtr<sphereFluidParticles>(tp);
    thermalParticles_ = tp; 

    // 1.4 Reconstruct insertion mechanism
    insertion_ = makeUnique<sphereInsertion>
    ( 
        particles_(), 
        particles_().spheres()
    );

    if (!thermalParticles_->initializeThermalParticles())
    {
        fatalError << "Failed to initialize thermal properties for particles.\n";
    }

    // 1.5 Reconstruct mechanical interactions
    interaction_ = interaction::create
    (
        Control(), 
        Particles(), 
        Geometry()
    );

    // 1.6 Initialize Unified Thermal Interaction Model
    REPORT(0) << "Creating thermal interactions "
              << "(Conduction, Radiation, PFP)..." << END_REPORT;
    
    box localDomain = domains.empty() ? box() : domains[0];
    
    thermalInteraction_ = makeUnique<thermalInteraction>
    (
        Control(), 
        *thermalParticles_, 
        localDomain
    );

    // 1.7 Update distribution boundaries
    real minD, maxD;
    particles_->boundingSphereMinMax(minD, maxD);
    particleDistribution_ = makeUnique<domainDistribute>(domains, maxD);
}

// ========================================================================= //
// Section 2: Time Integration Constraints
// ========================================================================= //

bool pFlow::thermalSphereDEMSystem::iterate
(
    real upToTime, 
    real timeToWrite, 
    word timeName
)
{
    Control().time().setStopAt(upToTime);
    Control().time().setOutputToFile(timeToWrite, timeName);
    
    return loop();
}

bool pFlow::thermalSphereDEMSystem::iterate(real upToTime)
{
    Control().time().setStopAt(upToTime);
    
    return loop();
}

// ========================================================================= //
// Section 3: Core Physics Loop
// ========================================================================= //

bool pFlow::thermalSphereDEMSystem::loop()
{
    do 
    {
        // 3.1 Handle particle injection triggers
        if (!insertion_().insertParticles
            ( 
                Control().time().currentIter(),
                Control().time().currentTime(),
                Control().time().dt() 
            ))
        {
            fatalError << "Particle insertion failed in thermalSphereDEMSystem.\n";
            return false;
        }    

        // 3.2 Initialize physics accumulators
        geometry_->beforeIteration();
        interaction_->beforeIteration();
        particles_->beforeIteration();
        
        // 3.3 Mechanical collision evaluation
        interaction_->iterate();

        // 3.4 Thermodynamic evaluation (Q_pp, Q_pfp, Q_rad)
        if (thermalInteraction_) 
        {
            thermalInteraction_->iterate();
        }

        // 3.5 Equations of motion and explicit energy integration
        particles_->iterate();

        // 3.6 Clean up and state finalization
        geometry_->iterate();
        particles_->afterIteration();
        geometry_->afterIteration();

    } while(Control()++);

    return true;
}

// ========================================================================= //
// Section 4: Data Exchange Interfaces (CFD-DEM Coupling)
// ========================================================================= //

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::temperature()
{
    auto& hVec = thermalParticles_->temperatureHost();
    return span<real>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::emissivity()
{
    auto& hVec = thermalParticles_->emissivityHost();
    return span<real>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::radSumTemp()
{
    auto& hVec = thermalParticles_->radSumTempHost();
    return span<real>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::uint32> pFlow::thermalSphereDEMSystem::radNumPrt()
{
    auto& hVec = thermalParticles_->radNumPrtHost();
    return span<uint32>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::parFluidHeatSourceConv()
{
    auto& hVec = thermalParticles_->heatSourceConvHost();
    return span<real>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::parFluidHeatSourceRad()
{
    auto& hVec = thermalParticles_->heatSourceRadHost();
    return span<real>(hVec.data(), hVec.size());
}

bool pFlow::thermalSphereDEMSystem::sendFluidHeatSourcesToDEM()
{
    thermalParticles_->heatSourcesHostUpdatedSync();
    return true;
}

// ========================================================================= //
// Section 5: PFP Pipeline Exchange
// ========================================================================= //

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::parFluidKappa()
{
    auto& hVec = thermalParticles_->fluidKappaHost();
    return span<real>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::real> pFlow::thermalSphereDEMSystem::parFluidAlpha()
{
    auto& hVec = thermalParticles_->fluidAlphaHost();
    return span<real>(hVec.data(), hVec.size());
}

bool pFlow::thermalSphereDEMSystem::sendFluidPropertiesToDEM()
{
    thermalParticles_->fluidPropertiesHostUpdatedSync();
    return true;
}

} // namespace pFlow



