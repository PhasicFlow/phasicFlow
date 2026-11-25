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

#ifndef pFlow_particleModelTypes_hpp
#define pFlow_particleModelTypes_hpp

#include "types.hpp"

namespace pFlow
{

/// Particle model of a reaction (dictionary key particleModel).
enum clcParticleModel : int32
{
    /// Rate on the outer surface without a particle model.
    clcModelNone  = 0,

    /// Non-porous particle: unreacted shrinking core.
    clcModelUSCM  = 1,

    /// Porous particle made of grains: grain model.
    clcModelGrain = 2
};

/**
 * @brief Particle-model parameters of one reaction, read from its
 * dictionary and passed to the reaction kernel.
 *
 * A value of zero in D_eKnudsen, D_eTemperature, D_P or k_c means that the
 * key is absent and its term is left out. The rate law that uses these
 * values is in particleModelFactor.hpp.
 */
struct clcParticleModelParams
{
    /// Particle model (clcParticleModel).
    int32 model = clcModelNone;

    /// 1: preExpFactor is per unit grain surface; 0: per unit outer surface.
    int32 grainBasis = 0;

    /// Index of the solid product used for the conversion X.
    int32 solidProductIdx = -1;

    /// Grain radius [m].
    real  grainRadius = 0;

    /// Effective diffusivity of the gas reactant [m2/s]: in the pores
    /// (grain, molecular part) or in the product layer (USCM).
    real  D_e = 0;

    /// Effective Knudsen diffusivity in the pores [m2/s].
    real  D_eKnudsen = 0;

    /// Temperature of D_e and D_eKnudsen [K].
    real  D_eTemperature = 0;

    /// Diffusivity in the product layer of a grain [m2/s].
    real  D_P = 0;

    /// Gas-film mass transfer coefficient [m/s].
    real  k_c = 0;

    /// Molar volume ratio of solid product to solid reactant [-].
    real  Z = 1;

    /// Start of the correction table in the flat table array (-1: none).
    int32 tableOffset = -1;

    /// Number of sigma2 values of the correction table.
    int32 nSigma2 = 0;

    /// Number of g values of the correction table.
    int32 nG = 0;

    /// Number of X values of the correction table.
    int32 nX = 0;
};

} // pFlow

#endif // pFlow_particleModelTypes_hpp
