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

#ifndef pFlow_grainCorrectionTable_hpp
#define pFlow_grainCorrectionTable_hpp

#include "types.hpp"
#include "Vectors.hpp"

namespace pFlow
{

/**
 * @brief Builds the correction table c(sigma2, g, X) of the grain model
 * for a reaction of the given order in the gas concentration.
 *
 * The table is the ratio of the closed-form resistance of
 * particleModelFactor.hpp to the resistance found by solving the grain
 * model in a spherical particle: quasi-steady diffusion and reaction of
 * the gas in the pores, and a shrinking front in every grain.
 *
 * In dimensionless form, with rho = r/R, psi = C/C_bulk and xi the front
 * position of the grains at rho:
 *
 *   (1/rho^2) d/drho(rho^2 dpsi/drho) = 18 sigma2 xi^2 psi_s^n
 *   dxi/dt = -psi_s^n
 *   6 g xi (1 - xi) psi_s^n + psi_s = psi      (product layer of the grain)
 *   psi = 1 at rho = 1,  dpsi/drho = 0 at rho = 0,  xi = 1 at t = 0
 *
 * psi_s is the gas concentration at the front of a grain. For n = 1 the
 * rate of a grain is psi / (1 + 6 g xi (1 - xi)). The gas film is not in
 * the table; it is added in series in the rate.
 *
 * @param order   reaction order n in the gas concentration
 * @param sigma2  [out] ascending sigma2 values
 * @param g       [out] ascending g values
 * @param X       [out] ascending conversion values
 * @param lnc     [out] ln c with the index (i*size(g) + j)*size(X) + k
 */
void buildGrainCorrectionTable(
    real        order,
    realVector& sigma2,
    realVector& g,
    realVector& X,
    realVector& lnc);

} // pFlow

#endif // pFlow_grainCorrectionTable_hpp
