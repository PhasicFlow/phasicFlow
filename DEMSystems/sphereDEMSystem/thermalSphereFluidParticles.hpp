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
 * @brief Thermal sphere particles that also carry fluid-momentum
 * coupling (drag/lift force and torque from a CFD mesh).
 *
 * @details
 * Inheritance: particles <- sphereParticles <- thermalSphereParticles
 * <- thermalSphereFluidParticles (linear, per confirmed design).
 *
 * This class deliberately does NOT inherit from sphereFluidParticles.
 * sphereFluidParticles : public sphereParticles is a non-virtual,
 * single-inheritance relationship in a file this project does not
 * permit editing, so it cannot be made virtual. Combining it with
 * thermalSphereParticles (which already carries sphereParticles once)
 * via multiple inheritance would create two independent sphereParticles
 * sub-objects -- in particular two independent dynPointStruct_
 * instances that would silently fall out of sync. The only correct,
 * compiling option under that constraint is single inheritance from
 * thermalSphereParticles, with fluidForce_/fluidTorque_ (and their
 * host mirrors, accessors, and sync methods) duplicated directly here
 * rather than inherited. The mechanical acceleration *kernel*
 * (sphereFluidParticlesKernels::acceleration) is a free function and
 * is reused as-is, not duplicated -- only the few lines that dispatch
 * to it are repeated. Likewise, the thermal kernels are reused via the
 * genuinely-inherited thermalSphereParticles::iterateThermal(), not
 * duplicated.
 *
 * Used wherever both thermal state and fluid-momentum coupling are
 * needed: the CFD-DEM coupled thermal solver (via thermalSphereDEMSystem),
 * and, via multiReactiveSphereParticles, the reactive solvers.
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

        /// Force exerted by the fluid phase on each particle [N].
        realx3PointField_D              fluidForce_;

        /// Torque exerted by the fluid phase on each particle [N.m].
        realx3PointField_D              fluidTorque_;

        /// Host mirror of fluidForce_, used for CPU-side coupling with
        /// the CFD solver.
        hostViewType1D<realx3>          fluidForceHost_;

        /// Host mirror of fluidTorque_.
        hostViewType1D<realx3>          fluidTorqueHost_;

protected:

    //- protected methods

        /**
         * @brief Ensures fluidForce_/fluidTorque_ host mirrors are sized
         * to match their device counterparts. Also invokes the genuinely
         * inherited thermalSphereParticles::checkHostMemory() (calling
         * it here, rather than relying on it being called elsewhere, is
         * necessary because checkHostMemory() is not virtual -- an
         * unqualified call from thermalSphereParticles::beforeIteration()
         * always resolves to thermalSphereParticles::checkHostMemory(),
         * never this override).
         */
        void checkHostMemory();

public:

    //- constructors

        thermalSphereFluidParticles(
            systemControl&              control,
            const sphereShape&          shpShape,
            const thermalSphereShape&   thShpShape);

        ~thermalSphereFluidParticles() override = default;

    //- public methods

        bool beforeIteration() override;

        bool iterate() override;

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

}; // thermalSphereFluidParticles

} // pFlow

#endif // pFlow_thermalSphereFluidParticles_hpp
