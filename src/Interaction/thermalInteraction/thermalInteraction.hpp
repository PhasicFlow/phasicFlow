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
 * * Manages the calculation of particle-particle conduction (Q_pp), 
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

        // --- Section 1: System References ---

            systemControl&                  control_;
            
            const thermalSphereParticles&   particles_;
            
            uniquePtr<mapperNBS>            mapper_ = nullptr;

        // --- Section 2: Physics Control Flags ---

            /// @brief Toggles radiation neighbourhood calculations.
            bool                            enableRadiation_ = false;
            
            uint32                          radUpdateInterval_ = 1;
            
            real                            radCut_ = 0.0;

            /// @brief Toggles direct particle-particle contact conduction.
            bool                            enableConduction_ = false;
            
            real                            simYoungsModulus_ = 1e7;

            /// @brief Toggles sub-grid fluid bridge heat transfer (PFP).
            bool                            enablePFP_ = false;

        // --- Section 3: Performance & Tracking ---

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

        // --- Section 4: Public Interface ---

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

}; // thermalInteraction

} // pFlow

#endif // pFlow_thermalInteraction_hpp
