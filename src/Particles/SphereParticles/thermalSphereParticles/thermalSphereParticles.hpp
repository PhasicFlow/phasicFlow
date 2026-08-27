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

#ifndef pFlow_thermalSphereParticles_hpp
#define pFlow_thermalSphereParticles_hpp

#include "sphereParticles.hpp"
#include "thermalSphereShape.hpp"

namespace pFlow
{

/**
 * @brief Standalone (no CFD mesh) thermal state and properties for
 * spherical particles: temperature, conduction (Q_pp), PFP (Q_pfp),
 * and radiation's neighbourhood sum. For a fluid-coupled particle set
 * that also has convective/radiative CFD sources and fluid-momentum
 * coupling, see thermalSphereFluidParticles.
 */
class thermalSphereParticles
:
    public sphereParticles
{
public:

    //- Type info

        TypeInfo("thermalSphereParticles");

private:

    //- private members

        /// Reference to the thermal shape/material database this particle
        /// set draws its per-material thermal properties from.
        const thermalSphereShape&       thSpheres_;

        /// Particle temperature [K].
        realPointField_D                temperature_;

        /// Particle specific heat capacity [J/(kg.K)].
        realPointField_D                Cp_;

        /// Particle thermal conductivity [W/(m.K)].
        realPointField_D                conductivity_;

        /// Rate of temperature change dT/dt [K/s], computed each step
        /// before time integration.
        realPointField_D                temperatureRate_;

        /// Particle-particle contact conduction heat source, Q_pp [W].
        realPointField_D                heatSourceCondPP_;

        /// Particle-fluid-particle sub-grid heat source, Q_pfp [W].
        realPointField_D                heatSourcePFP_;

        /// Sum of neighbouring particle temperatures, used by the
        /// linearised radiation model (CFD side) [K]. Written by
        /// thermalRadiationMechanism via deviceViewAll(), which does
        /// not own this field -- same ownership split as
        /// heatSourceCondPP_ above. Zero-filled (default) when
        /// radiation is disabled.
        realPointField_D                radSumTemp_;

        /// Number of radiating neighbours found for each particle [-].
        /// See radSumTemp_ above.
        uint32PointField_D              radNumPrt_;

        /// Particle surface emissivity [-].
        realPointField_D                emissivity_;

        /// Real (physical) Young's modulus of the particle material [Pa].
        realPointField_D                E0_;

        /// Poisson's ratio of the particle material [-].
        realPointField_D                nu_;

        /// Local fluid thermal conductivity sampled at the particle's
        /// cell, used by the PFP model [W/(m.K)].
        realPointField_D                fluidKappa_;

        /// Local fluid volume fraction (porosity) sampled at the
        /// particle's cell, used by the PFP model [-].
        realPointField_D                fluidAlpha_;

        /// Performance timer for the heat-transfer-rate calculation.
        Timer                           heatTransferTimer_;

        /// Performance timer for the temperature time-integration step.
        Timer                           temperatureIntegrationTimer_;

protected:

    //- protected methods

        /// Zeroes heatSourceCondPP_. Called from beforeIteration() --
        /// thermalInteraction only adds into this field, it never
        /// zeroes it.
        void zeroHeatSourceCondPP()
        {
            heatSourceCondPP_.fill(0.0);
        }

        /// Zeroes heatSourcePFP_. See zeroHeatSourceCondPP() above.
        void zeroHeatSourcePFP()
        {
            heatSourcePFP_.fill(0.0);
        }

        /// @brief Dispatches the standalone (no Q_conv/Q_rad)
        /// heat-transfer kernel and temperature integration, after
        /// resetting temperatureRate_ to zero.
        void iterateThermal();

        /// Rate of temperature change dT/dt [K/s]. Exposed so
        /// thermalSphereFluidParticles can dispatch its own
        /// fluid-coupled heat-transfer kernel call.
        inline
        realPointField_D& temperatureRate()
        {
            return temperatureRate_;
        }

        inline
        Timer& heatTransferTimer()
        {
            return heatTransferTimer_;
        }

        inline
        Timer& temperatureIntegrationTimer()
        {
            return temperatureIntegrationTimer_;
        }

public:

    //- constructors

        thermalSphereParticles(
            systemControl&              control,
            const thermalSphereShape&   thShpShape);

        ~thermalSphereParticles() override = default;

    //- public methods

        /// @brief Scatters per-material thermal properties to
        /// individual particle slots on the GPU.
        bool initializeThermalParticles();

        /// @brief Zeroes heatSourceCondPP_/heatSourcePFP_, then
        /// defers to sphereParticles::beforeIteration(). Zeroing
        /// happens here rather than in iterateThermal(), since
        /// thermalInteraction accumulates into these fields between
        /// this call and iterate() -- zeroing any later would erase
        /// that step's values before they are read. radSumTemp_/
        /// radNumPrt_ need no such zeroing: thermalRadiationMechanism
        /// assigns them directly (one thread per particle, not an
        /// atomic accumulation), so there is no stale value to clear
        /// first.
        bool beforeIteration() override;

        bool iterate() override;

        bool insertParticles(
            const realx3Vector&         pos,
            const wordVector&           names,
            const anyList&              vars) override;

        inline
        const realPointField_D& temperature() const
        {
            return temperature_;
        }

        inline
        realPointField_D& temperature()
        {
            return temperature_;
        }

        inline
        const realPointField_D& Cp() const
        {
            return Cp_;
        }

        inline
        const realPointField_D& conductivity() const
        {
            return conductivity_;
        }

        inline
        realPointField_D& conductivity()
        {
            return conductivity_;
        }

        inline
        const realPointField_D& heatSourceCondPP() const
        {
            return heatSourceCondPP_;
        }

        inline
        realPointField_D& heatSourceCondPP()
        {
            return heatSourceCondPP_;
        }

        inline
        const realPointField_D& heatSourcePFP() const
        {
            return heatSourcePFP_;
        }

        inline
        realPointField_D& heatSourcePFP()
        {
            return heatSourcePFP_;
        }

        inline
        const realPointField_D& radSumTemp() const
        {
            return radSumTemp_;
        }

        inline
        realPointField_D& radSumTemp()
        {
            return radSumTemp_;
        }

        inline
        const uint32PointField_D& radNumPrt() const
        {
            return radNumPrt_;
        }

        inline
        uint32PointField_D& radNumPrt()
        {
            return radNumPrt_;
        }

        inline
        const realPointField_D& emissivity() const
        {
            return emissivity_;
        }

        inline
        realPointField_D& emissivity()
        {
            return emissivity_;
        }

        inline
        const realPointField_D& E0() const
        {
            return E0_;
        }

        inline
        const realPointField_D& nu() const
        {
            return nu_;
        }

        inline
        const realPointField_D& fluidKappa() const
        {
            return fluidKappa_;
        }

        inline
        realPointField_D& fluidKappa()
        {
            return fluidKappa_;
        }

        inline
        const realPointField_D& fluidAlpha() const
        {
            return fluidAlpha_;
        }

        inline
        realPointField_D& fluidAlpha()
        {
            return fluidAlpha_;
        }

}; // thermalSphereParticles

} // pFlow

#endif // pFlow_thermalSphereParticles_hpp
