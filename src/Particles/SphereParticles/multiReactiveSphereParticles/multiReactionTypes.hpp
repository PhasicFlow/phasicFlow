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

#ifndef pFlow_multiReactionTypes_hpp
#define pFlow_multiReactionTypes_hpp

#include "types.hpp"

namespace pFlow
{

/**
 * @brief Upper bound on the number of solid species, gas species and
 * reactions handled by the fixed-size per-thread arrays of the reaction
 * kernel.
 *
 * multiReactiveSphereParticles checks the reaction network against this
 * constant, and the kernel (multiShrinkingInteractionKernels.cpp) sizes its
 * arrays with it.
 */
constexpr int32 clcMaxSpecies = 10;

/**
 * @brief Arrhenius kinetics data passed by value to the reaction kernel.
 *
 * Pointer members refer to device views owned by multiReactiveSphereParticles.
 * Matrices are row-major: stoicSolid[rxn * N_solid + s] and
 * stoicGas[rxn * N_gas + g].
 *
 * For reaction j of a particle at temperature T:
 *
 *   k     = preExpFactor[j] * exp(-activationTemperature[j] / T)
 *   omega = k * C_gas^gasOrder[j] * Y_solid^solidOrder[j] * A_p   [kmol/s]
 *
 * where C_gas [kmol/m3] is the concentration of the gas species driving the
 * reaction, Y_solid the mass fraction of the reaction's tracked solid
 * reactant and A_p the particle surface area. Reaction j is inactive while
 * T < onsetTemperature[j]. The whole reaction heat, -sum(omega * dH_rxn),
 * is deposited in the particle.
 *
 * Molar quantities follow the kmol basis: solidMw and gasMw are [kg/kmol]
 * and dH_rxn is [J/kmol] (negative for exothermic reactions). preExpFactor
 * has units kmol^(1-gasOrder) m^(3 gasOrder-2) / s, i.e. m/s for
 * gasOrder = 1; Y_solid is dimensionless, so solidOrder does not affect it.
 */
struct clcKineticsDevice
{
    /// Number of solid species.
    int32 N_solid = 0;

    /// Number of gas species.
    int32 N_gas = 0;

    /// Number of reactions.
    int32 N_rxn = 0;

    /// Solid stoichiometry [N_rxn x N_solid] (negative: consumed).
    const real* stoicSolid = nullptr;

    /// Gas stoichiometry [N_rxn x N_gas] (negative: consumed).
    const real* stoicGas = nullptr;

    /// Pre-exponential factor of each reaction.
    const real* preExpFactor = nullptr;

    /// Activation temperature E_a/R [K] of each reaction.
    const real* activationTemperature = nullptr;

    /// Reaction order in the gas concentration, per reaction.
    const real* gasOrder = nullptr;

    /// Reaction order in the solid reactant mass fraction, per reaction.
    const real* solidOrder = nullptr;

    /// Temperature [K] below which each reaction is inactive.
    const real* onsetTemperature = nullptr;

    /// Index of the gas species driving each reaction.
    const int32* gasSpeciesIdx = nullptr;

    /// Index of the tracked solid reactant of each reaction.
    const int32* solidReactantIdx = nullptr;

    /// Enthalpy of reaction [J/kmol] of each reaction (IUPAC sign).
    const real* dH_rxn = nullptr;

    /// Molar masses of the solid species [kg/kmol].
    const real* solidMw = nullptr;

    /// Densities of the solid species [kg/m3].
    const real* solidDensities = nullptr;

    /// Molar masses of the gas species [kg/kmol].
    const real* gasMw = nullptr;
};

} // pFlow

#endif // pFlow_multiReactionTypes_hpp
