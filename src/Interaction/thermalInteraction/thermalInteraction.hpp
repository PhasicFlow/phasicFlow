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
#include "thermalWallMechanism.hpp"
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
 *
 * Holds no physics logic of its own: each active mechanism runs its
 * own complete neighbour sweep, self-contained in its own file. This
 * class only rebuilds the shared cell-list mapper and dispatches to
 * whichever mechanisms exist.
 *
 * The mapper is rebuilt on every call and never reused over several
 * steps. A sweep takes the cell of particle i from its current
 * position but the members of every cell from the lists, so lists
 * that are even one step old give pairs that are missed or counted
 * twice (and, in radiation, a particle paired with itself) as soon as
 * a particle crosses a cell face. Radiation's own radUpdateInterval
 * (owned by thermalRadiationMechanism) is a separate matter: it
 * reflects radiation's physically slower update cadence.
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

        /// Null without walls, or when no wall path is active.
        uniquePtr<thermalWallMechanism>               wallMech_ = nullptr;

        uint32                                        stepCounter_ = 0;
        
        /// Overall time for iterate() -- search + both mechanisms.
        Timer                                         thermalTimer_;

        /// Time spent rebuilding the neighbor-search mapper.
        Timer                                         neighborSearchTimer_;

        /// Time spent across both mechanisms' iterate() calls.
        Timer                                         thermalKernelTimer_;

public:

    //- constructors

        /// @brief Reads which mechanisms are enabled and constructs
        /// only those.
        /// @param walls Wall geometry for the particle-wall heat
        /// transfer; nullptr leaves it off.
        /// @param fluidCoupled True in a CFD-coupled run, where the CFD
        /// side exchanges the particle radiation with its environment.
        thermalInteraction(
            systemControl&                  control,
            const thermalSphereParticles&   prtcl,
            const box&                      domainBox,
            const geometry*                 walls = nullptr,
            bool                            fluidCoupled = false);

        ~thermalInteraction() = default;

    //- public methods

        inline
        bool isRadiationEnabled() const
        {
            return radiationMech_ != nullptr;
        }

        /// @brief Rebuilds the shared mapper and dispatches to every
        /// active mechanism's own iterate().
        void iterate();

}; // thermalInteraction

} // pFlow

#endif // pFlow_thermalInteraction_hpp
