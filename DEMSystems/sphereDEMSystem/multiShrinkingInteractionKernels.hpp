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

#ifndef pFlow_multiShrinkingInteractionKernels_hpp
#define pFlow_multiShrinkingInteractionKernels_hpp

#include "types.hpp"
#include "pointFlag.hpp"
#include "multiReactionTypes.hpp"

namespace pFlow
{
namespace multiShrinkingKernels
{

/**
 * @brief Evaluates the Arrhenius reactions of all active particles and
 * updates their solid composition, mass, diameter, inertia, reaction heat
 * and gas sources.
 *
 * The rate law is described in clcKineticsDevice. Reaction j is inactive
 * while T < onsetTemperature[j]; a particle without any active reaction is
 * left untouched. The rates of the reactions that draw on a solid species
 * are scaled down together when they would consume more of it within one
 * step than the particle holds; no species goes negative and the products
 * keep their stoichiometric share. The whole reaction heat goes to the
 * particle.
 *
 * @param mask            Active particle flag.
 * @param dt              Time step [s].
 * @param shrinking       Shrinking Particle Model: the diameter follows the
 *                        solid volume; otherwise it is frozen.
 * @param T_p             Particle temperature [K].
 * @param gasConcs        Gas concentrations [kmol/m3], flat [N_p x N_gas].
 * @param kinetics        Kinetics data.
 * @param solidFracs      [IN/OUT] Solid mass fractions, flat [N_p x N_solid].
 * @param totalMass       [IN/OUT] Particle mass [kg].
 * @param diameter        [IN/OUT] Particle diameter [m].
 * @param inertia         [OUT] Moment of inertia [kg m2].
 * @param gasMassSource   [OUT] Gas mass source [kg/s], flat [N_p x N_gas].
 * @param gasMassSourceSp [OUT] Implicit gas sink coefficient, flat
 *                        [N_p x N_gas].
 * @param qrxn_particle   [OUT] Reaction heat of the particles [W].
 */
void calcMultiReaction
(
    const pFlagTypeDevice&         mask,
    const real                     dt,
    const bool                     shrinking,
    const deviceViewType1D<real>&  T_p,
    const deviceViewType1D<real>&  gasConcs,
    const clcKineticsDevice        kinetics,
    deviceViewType1D<real>         solidFracs,
    deviceViewType1D<real>         totalMass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia,
    deviceViewType1D<real>         gasMassSource,
    deviceViewType1D<real>         gasMassSourceSp,
    deviceViewType1D<real>         qrxn_particle
);

/**
 * @brief Minimum temperature of the active particles.
 *
 * @param mask Active particle flag.
 * @param T_p  Particle temperature [K].
 * @return Minimum temperature [K]; the largest representable value when no
 * particle is active.
 */
real minTemperature
(
    const pFlagTypeDevice&         mask,
    const deviceViewType1D<real>&  T_p
);

} // multiShrinkingKernels
} // pFlow

#endif // pFlow_multiShrinkingInteractionKernels_hpp
