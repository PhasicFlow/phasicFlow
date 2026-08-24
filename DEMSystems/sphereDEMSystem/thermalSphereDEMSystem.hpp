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
 * Its inherited particles_ member (declared in sphereDEMSystem as
 * uniquePtr<sphereFluidParticles>) is deliberately left unused (reset
 * to null): thermalSphereFluidParticles does not inherit from
 * sphereFluidParticles (see that class), so it cannot be assigned
 * there. This class instead holds its particles through its own
 * thermalParticles_ member.
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

        /// @brief Host mirror of radiation's device data. Owned here,
        /// not on thermalInteraction, since this class is the
        /// CFD-coupling-only layer and host mirrors are only ever
        /// needed for CFD exchange. The device-side computation stays
        /// on thermalInteraction, since standalone mode needs it too.
        hostViewType1D<real>            radSumTempHost_;

        /// @brief Host mirror of radiation's neighbour-count data.
        /// See radSumTempHost_ above.
        hostViewType1D<uint32>          radNumPrtHost_;

    //- protected methods

        bool loop();

        /// @brief Resizes radSumTempHost_/radNumPrtHost_ to the
        /// current particle count. Safe to call every step.
        void ensureRadiationHostMemory();

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

        //--- overrides for particles_-facing base methods --------------
        // sphereDEMSystem's inherited implementation reads particles_
        // directly, which is unused/null here -- everything below
        // reads thermalParticles_ instead. Methods touching only the
        // protected xxxHost_ mirrors or spheres_/particleDistribution_
        // do not need overriding: diameter(), velocity(), position(),
        // acceleration(), rVelocity(), shapeDiameters(),
        // numParInDomain(s)(), parIndexInDomain().

        bool beforeIteration() override;

        real maxBounndingSphereSize() const override;

        bool updateParticleDistribution(
            real                    extentFraction,
            const std::vector<box>  domains) override;

        span<realx3> parFluidForce() override;

        span<realx3> parFluidTorque() override;

        bool sendFluidForceToDEM() override;

        bool sendFluidTorqueToDEM() override;

        //--- thermal & radiation coupling interfaces --------------------------

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

        /// @brief True if radiation is active and enabled by the user.
        inline
        bool hasRadiation() const override
        {
            return thermalInteraction_ != nullptr &&
                   thermalInteraction_->isRadiationEnabled();
        }

}; // thermalSphereDEMSystem

} // pFlow

#endif // pFlow_thermalSphereDEMSystem_hpp
