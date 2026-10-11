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

#include <algorithm>

#include "multiShrinkingInteraction.hpp"
#include "multiShrinkingInteractionKernels.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

void multiShrinkingInteraction::checkColdParticles()
{
    const real minTp = multiShrinkingKernels::minTemperature
    (
        particles_.getActiveMask(),
        particles_.temperature().deviceViewAll()
    );

    // Below the lowest onset temperature of all reactions, no reaction can
    // be active for the coldest particle.
    const auto& onsetTemps = properties_.onsetTemperature();

    const real minOnset =
        *std::min_element(onsetTemps.begin(), onsetTemps.end());

    if (minTp < minOnset)
    {
        coldWarningIssued_ = true;

        REPORT(0) << Yellow_Text("\n[CLC] Onset cutoff warning: ")
                  << "min(T_particle) = " << minTp << " K, "
                  << "min(onsetTemperature) = " << minOnset << " K. "
                  << "No reaction is active for the coldest particle.\n"
                  << END_REPORT;
    }
}

//------------------------------- constructors --------------------------------

multiShrinkingInteraction::multiShrinkingInteraction
(
    systemControl&                control,
    multiReactiveSphereParticles& prtcl,
    const multiSpeciesProperty&   prop
)
:
    control_(control),
    particles_(prtcl),
    properties_(prop),
    rxnTimer_("multiReaction", &control.timers())
{
    REPORT(0) << "Creating Multi-Species Shrinking Interaction Model . . ."
              << END_REPORT;
}

//----------------------------- public methods --------------------------------

void multiShrinkingInteraction::iterate()
{
    rxnTimer_.start();

    // The check is a device reduction, so it runs every coldCheckInterval_
    // iterations (which also covers particles inserted later) until the
    // warning has been issued.
    if (!coldWarningIssued_ && stepCounter_ % coldCheckInterval_ == 0)
    {
        checkColdParticles();
    }

    stepCounter_++;

    multiShrinkingKernels::calcMultiReaction
    (
        particles_.getActiveMask(),
        control_.time().dt(),
        properties_.enableShrinking(),
        particles_.temperature().deviceViewAll(),
        particles_.gasConcentrations(),
        particles_.kineticsDevice(),
        particles_.solidMassFractions(),
        particles_.mass().deviceViewAll(),
        particles_.diameter().deviceViewAll(),
        particles_.I().deviceViewAll(),
        particles_.gasMassSource(),
        particles_.gasMassSourceSp(),
        particles_.reactionHeat().deviceViewAll()
    );

    rxnTimer_.end();
}

} // pFlow
