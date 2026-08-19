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

#ifndef pFlow_thermalSphereDEMSystem_hpp
#define pFlow_thermalSphereDEMSystem_hpp

#include "sphereDEMSystem.hpp"
#include "thermalSphereFluidParticles.hpp"
#include "thermalInteraction.hpp"
#include "thermalProperty.hpp"
#include "thermalSphereShape.hpp"

namespace pFlow
{

/**
 * @brief Extends the base mechanical DEM solver to handle thermodynamic 
 * physics (Conduction, PFP, and Radiation).
 *
 * @details
 * Inherits from sphereDEMSystem to reuse property_/geometry_ construction
 * and the domain-distribution machinery, but its inherited particles_
 * member (declared in sphereDEMSystem as uniquePtr<sphereFluidParticles>)
 * is deliberately left unused (reset to null): thermalSphereFluidParticles
 * does not inherit from sphereFluidParticles (see that class), so it
 * cannot be assigned there. This class instead holds its particles
 * through its own thermalParticles_ member, and overrides every
 * DEMSystem-level virtual method that sphereDEMSystem's inherited
 * implementation would otherwise reach through particles_ directly --
 * verified against sphereDEMSystem.cpp method-by-method, not assumed.
 * Methods that only touch the protected xxxHost_ mirror arrays (already
 * declared in sphereDEMSystem) or spheres_/particleDistribution_ do NOT
 * need overriding: diameter(), velocity(), position(), acceleration(),
 * rVelocity(), shapeDiameters(), numParInDomain(s)(), parIndexInDomain().
 */
class thermalSphereDEMSystem
:
    public sphereDEMSystem
{
protected:

    //- protected members

        /// @brief Direct host-view access for thermal (+ fluid-coupled)
        /// particles. Used in place of the inherited particles_, which
        /// is left unused for this class (see class-level doc comment).
        thermalSphereFluidParticles*    thermalParticles_ = nullptr;

        /// @brief Manages inter-particle conduction, fluid-bridge heat,
        /// radiation.
        uniquePtr<thermalInteraction>   thermalInteraction_ = nullptr;

    //- protected methods

        bool loop();

public:

    //- Type info

        TypeInfo("thermalSphereDEMSystem");

    //- constructors

        thermalSphereDEMSystem(
            word                    demSystemName,
            const std::vector<box>& domains,
            int                     argc,
            char*                   argv[],
            bool                    requireRVel = false);

        ~thermalSphereDEMSystem() override = default;

    //- public methods

        add_vCtor(
            DEMSystem,
            thermalSphereDEMSystem,
            word
        );

        bool iterate(
            real                    upToTime,
            real                    timeToWrite,
            word                    timeName) override;

        bool iterate(real upToTime) override;

        // ================================================================= //
        // Overrides required because sphereDEMSystem's inherited implementation
        // reads particles_ directly (verified line-by-line against
        // sphereDEMSystem.cpp), and particles_ is unused/null in this class.
        // ================================================================= //

        bool beforeIteration() override;

        real maxBounndingSphereSize() const override;

        bool updateParticleDistribution(
            real                    extentFraction,
            const std::vector<box>  domains) override;

        span<realx3> parFluidForce() override;

        span<realx3> parFluidTorque() override;

        bool sendFluidForceToDEM() override;

        bool sendFluidTorqueToDEM() override;

        // ================================================================= //
        // Thermal & radiation coupling interfaces
        // ================================================================= //

        span<real> temperature() override;
        
        span<real> emissivity() override;
        
        span<real> radSumTemp() override;
        
        span<uint32> radNumPrt() override;
        
        span<real> parFluidHeatSourceConv() override;
        
        span<real> parFluidHeatSourceRad() override;

        bool sendFluidHeatSourcesToDEM() override;

        span<real> parFluidKappa() override;
        
        span<real> parFluidAlpha() override;
        
        bool sendFluidPropertiesToDEM() override;

        /**
         * @brief Evaluates radiation module availability.
         * @return True if the module is active and enabled by user dictionary.
         */
        inline
        bool hasRadiation() const override
        {
            return thermalInteraction_ != nullptr &&
                   thermalInteraction_->isRadiationEnabled();
        }

}; // thermalSphereDEMSystem

} // pFlow

#endif // pFlow_thermalSphereDEMSystem_hpp
