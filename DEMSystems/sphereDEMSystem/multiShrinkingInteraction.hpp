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

#ifndef pFlow_multiShrinkingInteraction_hpp
#define pFlow_multiShrinkingInteraction_hpp

#include "systemControl.hpp"
#include "Timer.hpp"
#include "multiReactiveSphereParticles.hpp"
#include "multiSpeciesProperty.hpp"

namespace pFlow
{

/**
 * @brief Dispatches the multi-species Arrhenius reaction kernel on the
 * reactive particles.
 *
 * Like thermalInteraction, it is a plain class that the DEM loop calls
 * through iterate(). It lives in DEMSystems because it references
 * multiReactiveSphereParticles, a tier that the Interaction library cannot
 * see.
 */
class multiShrinkingInteraction
{
public:

    TypeInfo("multiShrinkingInteraction");

private:

    /// Simulation control, for the time step and the timers.
    systemControl&                control_;

    /// Reactive particles updated by the reaction kernel.
    multiReactiveSphereParticles& particles_;

    /// Multi-species property database.
    const multiSpeciesProperty&   properties_;

    /// Iterations between two checks for particles colder than every
    /// onset temperature.
    static constexpr uint32       coldCheckInterval_ = 1000;

    /// Set once the cold-particle warning has been issued.
    bool                          coldWarningIssued_ = false;

    /// Number of iterate() calls so far.
    uint32                        stepCounter_       = 0;

    /// Timer of the reaction kernel.
    Timer                         rxnTimer_;

    /// Warns when the coldest particle is below every onset temperature.
    void checkColdParticles();

public:

    multiShrinkingInteraction
    (
        systemControl&                control,
        multiReactiveSphereParticles& prtcl,
        const multiSpeciesProperty&   prop
    );

    ~multiShrinkingInteraction() = default;

    /// Evaluates the reactions of all active particles.
    void iterate();
};

} // pFlow

#endif // pFlow_multiShrinkingInteraction_hpp
