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
#include "thermalRadiationMechanism.hpp"
#include "thermalConductionPFPMechanism.hpp"
#include "Timer.hpp"

namespace pFlow 
{

/**
 * @brief Dispatcher for particle-particle conduction (Q_pp), PFP
 * sub-grid heat transfer (Q_pfp), and radiation neighbourhood sums.
 *
 * Structured like momentumInteraction on the CFD side (independent
 * uniquePtr sub-mechanisms), not like drag/interaction (one class
 * templated on a closure): radiation and conduction+PFP are
 * independent mechanisms, not alternative ways to compute the same
 * thing. A disabled mechanism's uniquePtr stays null -- no memory
 * allocated, no dictionary read.
 */
class thermalInteraction 
{
public:

    //- Type info

        TypeInfo("thermalInteraction");

private:

    //- private members

        systemControl&                              control_;
        
        const thermalSphereParticles&                particles_;
        
        uniquePtr<mapperNBS>                         mapper_ = nullptr;

        /// Null when radiation is disabled.
        uniquePtr<thermalRadiationMechanism>          radiationMech_ = nullptr;

        /// Null when both conduction and PFP are disabled.
        uniquePtr<thermalConductionPFPMechanism>      condPfpMech_ = nullptr;

        uint32                                        stepCounter_ = 0;
        
        /// Overall time for iterate() -- search + kernel combined.
        Timer                                         thermalTimer_;

        /// Time spent rebuilding the neighbor-search mapper.
        Timer                                         neighborSearchTimer_;

        /// Time spent in the physics kernel itself.
        Timer                                         thermalKernelTimer_;

public:

    //- constructors

        /// @brief Reads which mechanisms are enabled and constructs
        /// only those.
        thermalInteraction(
            systemControl&                  control, 
            const thermalSphereParticles&   prtcl, 
            const box&                      domainBox);

        ~thermalInteraction() = default;

    //- public methods

        inline
        bool isRadiationEnabled() const
        {
            return radiationMech_ != nullptr;
        }

        /// @brief Runs the shared neighbor search and dispatches to
        /// every active mechanism.
        void iterate();

        /// @brief Radiation's device-side sum-of-neighbour-temperatures,
        /// for thermalSphereDEMSystem to sync to its host mirror.
        /// Empty when radiation is disabled.
        inline
        const deviceViewType1D<real>& radSumTempDevice() const
        {
            static deviceViewType1D<real> empty;
            return radiationMech_ ? radiationMech_->radSumTemp() : empty;
        }

        /// @brief Radiation's device-side neighbour count. Empty when
        /// radiation is disabled.
        inline
        const deviceViewType1D<uint32>& radNumPrtDevice() const
        {
            static deviceViewType1D<uint32> empty;
            return radiationMech_ ? radiationMech_->radNumPrt() : empty;
        }

}; // thermalInteraction

} // pFlow

#endif // pFlow_thermalInteraction_hpp
