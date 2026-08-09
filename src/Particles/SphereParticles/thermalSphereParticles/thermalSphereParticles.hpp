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

#include "sphereFluidParticles.hpp"
#include "thermalSphereShape.hpp"

namespace pFlow
{

/**
 * @brief Manages the thermal state and thermodynamic properties of 
 * spherical particles on the GPU.
 *
 * @details
 * Extends sphereFluidParticles by introducing device (Kokkos) memory for:
 * - Temperatures and integration rates (Explicit Euler).
 * - Thermodynamic properties (heat capacities, conductivities, emissivities).
 * - Multi-mode heat sources (Convection, Radiation, Conduction, PFP).
 * - Host (CPU) mirror arrays used for MPI and OpenFOAM coupling synchronization.
 */
class thermalSphereParticles
:
    public sphereFluidParticles
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

        /// Convective heat source from the fluid phase [W].
        realPointField_D                heatSourceConv_;

        /// Radiative heat source from neighbouring particles [W].
        realPointField_D                heatSourceRad_;

        /// Particle-particle contact conduction heat source, Q_pp [W].
        realPointField_D                heatSourceCondPP_;

        /// Particle-fluid-particle sub-grid heat source, Q_pfp [W].
        realPointField_D                heatSourcePFP_;

        /// Particle surface emissivity [-].
        realPointField_D                emissivity_;

        /// Sum of neighbouring particle temperatures used by the
        /// radiation model [K].
        realPointField_D                radSumTemp_;

        /// Number of radiating neighbours found for each particle [-].
        uint32PointField_D              radNumPrt_;

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

        /// Host mirror of temperature_, used for CPU-side coupling/IO.
        hostViewType1D<real>            temperatureHost_;

        /// Host mirror of heatSourceConv_.
        hostViewType1D<real>            heatSourceConvHost_;

        /// Host mirror of heatSourceRad_.
        hostViewType1D<real>            heatSourceRadHost_;

        /// Host mirror of heatSourceCondPP_.
        hostViewType1D<real>            heatSourceCondPPHost_;

        /// Host mirror of emissivity_.
        hostViewType1D<real>            emissivityHost_;

        /// Host mirror of radSumTemp_.
        hostViewType1D<real>            radSumTempHost_;

        /// Host mirror of radNumPrt_.
        hostViewType1D<uint32>          radNumPrtHost_;

        /// Host mirror of fluidKappa_.
        hostViewType1D<real>            fluidKappaHost_;

        /// Host mirror of fluidAlpha_.
        hostViewType1D<real>            fluidAlphaHost_;

protected:

    //- protected methods

        /**
         * @brief Ensures host arrays are sized to match their corresponding
         * device arrays and initializes newly allocated memory slots.
         */
        void checkHostMemory();

public:

    //- constructors

        thermalSphereParticles(
            systemControl&              control,
            const sphereShape&          shpShape,
            const thermalSphereShape&   thShpShape);

        ~thermalSphereParticles() override = default;

    //- public methods

        /**
         * @brief Scatters per-material thermal properties to individual 
         * particle slots on the GPU.
         * @return True upon successful mapping.
         */
        bool initializeThermalParticles();

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
        const realPointField_D& heatSourceConv() const
        {
            return heatSourceConv_;
        }

        inline
        realPointField_D& heatSourceConv()
        {
            return heatSourceConv_;
        }

        inline
        const realPointField_D& heatSourceRad() const
        {
            return heatSourceRad_;
        }

        inline
        realPointField_D& heatSourceRad()
        {
            return heatSourceRad_;
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

        inline
        auto& temperatureHost()
        {
            return temperatureHost_;
        }

        inline
        auto& heatSourceConvHost()
        {
            return heatSourceConvHost_;
        }

        inline
        auto& heatSourceRadHost()
        {
            return heatSourceRadHost_;
        }

        inline
        auto& heatSourceCondPPHost()
        {
            return heatSourceCondPPHost_;
        }

        inline
        auto& emissivityHost()
        {
            return emissivityHost_;
        }

        inline
        auto& radSumTempHost()
        {
            return radSumTempHost_;
        }

        inline
        auto& radNumPrtHost()
        {
            return radNumPrtHost_;
        }

        inline
        auto& fluidKappaHost()
        {
            return fluidKappaHost_;
        }

        inline
        auto& fluidAlphaHost()
        {
            return fluidAlphaHost_;
        }

        void heatSourcesHostUpdatedSync();
        
        void fluidPropertiesHostUpdatedSync();
        
        void temperatureHostUpdatedSync();
        
        void radiationDataHostUpdatedSync();

}; // thermalSphereParticles

} // pFlow

#endif // pFlow_thermalSphereParticles_hpp
