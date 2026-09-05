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

#ifndef pFlow_thermalSphereFluidParticles_hpp
#define pFlow_thermalSphereFluidParticles_hpp

#include "thermalSphereParticles.hpp"
#include "sphereFluidParticlesKernels.hpp"

namespace pFlow
{

/**
 * @brief Thermal sphere particles with fluid-momentum coupling and
 * CFD-exchanged data (heat sources, temperature, emissivity, ambient
 * fluid properties). Does not inherit sphereFluidParticles: that
 * would create two independent dynPointStruct_ instances (see
 * thermalSphereParticles for the shared base). The mechanical
 * acceleration kernel is reused, not duplicated.
 */
class thermalSphereFluidParticles
:
    public thermalSphereParticles
{
public:

    //- Type info

        TypeInfo("thermalSphereFluidParticles");

private:

    //- private members

        // --- Fluid-momentum coupling (mechanical) ---

        /// Force exerted by the fluid phase on each particle [N].
        realx3PointField_D              fluidForce_;

        /// Torque exerted by the fluid phase on each particle [N.m].
        realx3PointField_D              fluidTorque_;

        /// Host mirror of fluidForce_, used for CPU-side coupling with
        /// the CFD solver.
        hostViewType1D<realx3>          fluidForceHost_;

        /// Host mirror of fluidTorque_.
        hostViewType1D<realx3>          fluidTorqueHost_;

        // --- Fluid-momentum coupling (thermal) ---
        // READ_NEVER/WRITE_NEVER (see the .cpp): zeroed and recomputed
        // by the CFD coupling layer on every exchange, so a value in
        // particlesDict.setFields is silently ignored -- intentional.

        /// Convective heat source from the fluid phase [W].
        realPointField_D                heatSourceConv_;

        /// Radiative heat source from neighbouring particles [W].
        realPointField_D                heatSourceRad_;

        // --- Host mirrors: CPU-side exchange with the CFD solver ---

        /// Host mirror of temperature() (inherited).
        hostViewType1D<real>            temperatureHost_;

        /// Host mirror of heatSourceConv_.
        hostViewType1D<real>            heatSourceConvHost_;

        /// Host mirror of heatSourceRad_.
        hostViewType1D<real>            heatSourceRadHost_;

        /// Host mirror of emissivity() (inherited).
        hostViewType1D<real>            emissivityHost_;

        /// Host mirror of fluidKappa() (inherited).
        hostViewType1D<real>            fluidKappaHost_;

        /// Host mirror of fluidAlpha() (inherited).
        hostViewType1D<real>            fluidAlphaHost_;

protected:

    //- protected methods

        /// @brief Sizes every host mirror to match its device field.
        void checkHostMemory();

public:

    //- constructors

        thermalSphereFluidParticles(
            systemControl&              control,
            const thermalSphereShape&   thShpShape);

        ~thermalSphereFluidParticles() override = default;

    //- public methods

        bool beforeIteration() override;

        bool iterate() override;

        // --- Fluid-momentum coupling accessors ---

        inline
        const realx3PointField_D& fluidForce() const
        {
            return fluidForce_;
        }

        inline
        realx3PointField_D& fluidForce()
        {
            return fluidForce_;
        }

        inline
        const realx3PointField_D& fluidTorque() const
        {
            return fluidTorque_;
        }

        inline
        realx3PointField_D& fluidTorque()
        {
            return fluidTorque_;
        }

        inline
        auto& fluidForceHost()
        {
            return fluidForceHost_;
        }

        inline
        auto& fluidTorqueHost()
        {
            return fluidTorqueHost_;
        }

        void fluidForceHostUpdatedSync();

        void fluidTorqueHostUpdatedSync();

        // --- Thermal fluid-coupling accessors ---

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
        auto& emissivityHost()
        {
            return emissivityHost_;
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

        /// @brief Pushes heatSourceConvHost_/heatSourceRadHost_ to
        /// their device fields.
        void heatSourcesHostUpdatedSync();

        /// @brief Pushes fluidKappaHost_/fluidAlphaHost_ to their
        /// device fields.
        void fluidPropertiesHostUpdatedSync();

        /// @brief Pulls temperature() to temperatureHost_.
        void temperatureHostUpdatedSync();

        /// @brief Pulls emissivity() to emissivityHost_ (a material
        /// constant -- only ever needs calling once).
        void emissivityHostUpdatedSync();

}; // thermalSphereFluidParticles

} // pFlow

#endif // pFlow_thermalSphereFluidParticles_hpp
