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

#ifndef pFlow_thermalInteraction_hpp
#define pFlow_thermalInteraction_hpp

#include "systemControl.hpp"
#include "thermalSphereParticles.hpp"
#include "mapperNBS.hpp"
#include "Timer.hpp"

namespace pFlow 
{

/**
 * @brief Dispatcher for intra-phase thermodynamic interactions.
 *
 * Manages the calculation of particle-particle conduction (Q_pp), 
 * sub-grid Particle-Fluid-Particle heat transfer (Q_pfp), and 
 * local radiation neighbourhood sums.
 */
class thermalInteraction 
{
public:

    //- Type info

        TypeInfo("thermalInteraction");

private:

    //- private members

        // --- System references ---

            systemControl&                  control_;
            
            const thermalSphereParticles&   particles_;
            
            uniquePtr<mapperNBS>            mapper_ = nullptr;

        // --- Physics control flags ---

            /// @brief Toggles radiation neighbourhood calculations.
            bool                            enableRadiation_ = false;
            
            uint32                          radUpdateInterval_ = 1;
            
            real                            radCut_ = 0.0;

            /// @brief Toggles direct particle-particle contact conduction.
            bool                            enableConduction_ = false;
            
            real                            simYoungsModulus_ = 1e7;

            /// @brief Toggles sub-grid fluid bridge heat transfer (PFP).
            bool                            enablePFP_ = false;

        // --- Radiation neighbourhood output ---
        // Plain Kokkos views, not registered PointFields: particles_
        // is a const reference, so a PointField (needing the
        // protected, non-const dynPointStruct()) cannot be registered
        // here. Sized explicitly via ensureRadiationMemory().

            /// Sum of neighbouring particle temperatures per particle,
            /// used by the linearised radiation model in
            /// sphereHeatTransfer (CFD side) [K].
            deviceViewType1D<real>          radSumTemp_;

            /// Number of radiating neighbours found for each particle [-].
            deviceViewType1D<uint32>        radNumPrt_;

            /// Host mirror of radSumTemp_, used for CPU-side coupling
            /// with the CFD solver.
            hostViewType1D<real>            radSumTempHost_;

            /// Host mirror of radNumPrt_.
            hostViewType1D<uint32>          radNumPrtHost_;

        // --- Performance & tracking ---

            uint32                          stepCounter_ = 0;
            
            /// Overall time for iterate() -- search + kernel combined.
            Timer                           thermalTimer_;

            /// Time spent rebuilding the neighbor-search mapper
            /// (mapper_->build()) specifically, isolated from the
            /// physics kernel below it.
            Timer                           neighborSearchTimer_;

            /// Time spent in the physics kernel itself
            /// (thermalInteractionKernels::calcThermalInteractions),
            /// isolated from the neighbor search above it.
            Timer                           thermalKernelTimer_;

    //- private methods

        /**
         * @brief Sizes radSumTemp_/radNumPrt_ to the current particle
         * count. See the "Radiation neighbourhood output" comment
         * above for why.
         */
        void ensureRadiationMemory();

public:

    //- constructors

        /**
         * @brief Constructs the thermal interaction manager.
         */
        thermalInteraction(
            systemControl&                  control, 
            const thermalSphereParticles&   prtcl, 
            const box&                      domainBox);

        ~thermalInteraction() = default;

    //- public methods

        /**
         * @brief Checks if radiation physics is actively executing.
         * @return True if radiation is globally enabled by the user.
         */
        inline
        bool isRadiationEnabled() const
        {
            return enableRadiation_;
        }

        /**
         * @brief Executes the neighbor-search and thermodynamic kernels.
         */
        void iterate();

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

        /**
         * @brief Copies radSumTemp_/radNumPrt_ to their host mirrors.
         * Called automatically at the end of iterate(); safe to call
         * again from outside since it is idempotent.
         */
        void radiationDataHostUpdatedSync();

}; // thermalInteraction

} // pFlow

#endif // pFlow_thermalInteraction_hpp
