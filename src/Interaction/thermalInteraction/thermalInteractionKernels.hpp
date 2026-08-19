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

#ifndef pFlow_thermalInteractionKernels_hpp
#define pFlow_thermalInteractionKernels_hpp

#include "types.hpp"
#include "pointFlag.hpp"
#include "mapperNBS.hpp"

namespace pFlow
{
namespace thermalInteractionKernels
{

/**
 * @brief Sweeps the 27-cell neighbourhood of every active particle,
 * dispatching each pair found to the three per-mechanism kernels:
 *
 * - Radiation (temperature-sum accumulation): thermalRadiationKernels.hpp
 * - Static-contact conduction (Q_pp):         thermalConductionKernels.hpp
 * - Particle-Fluid-Particle sub-grid (Q_pfp): thermalPFPKernels.hpp
 *
 * Radiation runs on every (i,j) pair within radCut. Conduction and PFP
 * run once per pair (i < j); conduction's Hertzian-corrected contact
 * radius is reused as PFP's lower integration limit r_sij -- see
 * thermalConductionKernels::contactConduction().
 *
 * @param calcRad   Enable radiation neighbourhood accumulation.
 * @param calcCond  Enable particle-particle static-contact conduction (Q_pp).
 * @param calcPFP   Enable particle-fluid-particle sub-grid transfer (Q_pfp).
 */
void calcThermalInteractions(
    const pFlagTypeDevice&          m,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<realx3>& tvel,
    const deviceViewType1D<realx3>& rvel,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   mass,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   Cp,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    // local fluid kappa at particle cell [W/(m.K)]
    const deviceViewType1D<real>&   fluidKappa,
    // local fluid porosity alpha at particle cell [-]
    const deviceViewType1D<real>&   fluidAlpha,
    const mapperNBS::CellIterator&  cellIter,
    const realx3&                   domainMin,
    const real&                     cellSize,
    const int32x3&                  numCells,
    const real                      radCut,
    const real                      simYoungsModulus,
    const bool                      calcRad,
    const bool                      calcCond,
    const bool                      calcPFP,
    // static-contact conduction output [W]
    deviceViewType1D<real>          Q_pp,
    // PFP sub-grid output [W]
    deviceViewType1D<real>          Q_pfp,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt);

} // thermalInteractionKernels
} // pFlow

#endif // pFlow_thermalInteractionKernels_hpp
