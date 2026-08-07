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

        // --- Section 1: Shape Reference ---

        const thermalSphereShape&       thSpheres_;

        // --- Section 2: Device Fields (GPU/Kokkos) ---

        realPointField_D                temperature_;
        realPointField_D                Cp_;
        realPointField_D                conductivity_;
        realPointField_D                temperatureRate_;

        realPointField_D                heatSourceConv_;
        realPointField_D                heatSourceRad_;
        realPointField_D                heatSourceCondPP_;
        realPointField_D                heatSourcePFP_;

        realPointField_D                emissivity_;
        realPointField_D                radSumTemp_;
        uint32PointField_D              radNumPrt_;

        realPointField_D                E0_;
        realPointField_D                nu_;

        realPointField_D                fluidKappa_;
        realPointField_D                fluidAlpha_;

        // --- Section 3: Performance Timers ---

        Timer                           heatTransferTimer_;
        Timer                           temperatureIntegrationTimer_;

        // --- Section 4: Host Mirror Fields (CPU RAM) ---

        hostViewType1D<real>            temperatureHost_;
        hostViewType1D<real>            heatSourceConvHost_;
        hostViewType1D<real>            heatSourceRadHost_;
        hostViewType1D<real>            heatSourceCondPPHost_;
        hostViewType1D<real>            emissivityHost_;
        hostViewType1D<real>            radSumTempHost_;
        hostViewType1D<uint32>          radNumPrtHost_;
        hostViewType1D<real>            fluidKappaHost_;
        hostViewType1D<real>            fluidAlphaHost_;

protected:

    //- protected methods

        // --- Section 5: Memory Management ---

        /**
         * @brief Ensures host arrays are sized to match their corresponding
         * device arrays and initializes newly allocated memory slots.
         */
        void checkHostMemory();

public:

    //- constructors

        // --- Section 6: Constructor and Initialization ---

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

        // --- Section 7: Core Iteration Hooks ---

        bool beforeIteration() override;
        
        bool iterate() override;

        bool insertParticles(
            const realx3Vector&         pos,
            const wordVector&           names,
            const anyList&              vars) override;

        // --- Section 8: Device Accessors ---

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

        // --- Section 9: Host Accessors ---

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

        // --- Section 10: Synchronisation Routines (Host <-> Device) ---

        void heatSourcesHostUpdatedSync();
        
        void fluidPropertiesHostUpdatedSync();
        
        void temperatureHostUpdatedSync();
        
        void radiationDataHostUpdatedSync();

}; // thermalSphereParticles

} // pFlow

#endif // pFlow_thermalSphereParticles_hpp


