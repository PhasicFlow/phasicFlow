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

#ifndef pFlow_thermalRadiationKernels_hpp
#define pFlow_thermalRadiationKernels_hpp

#include "types.hpp"

namespace pFlow
{
namespace thermalRadiationKernels
{

/**
 * @brief Accumulates one radiating neighbour's temperature into the
 * running sum and count used by the linearised radiation model in
 * sphereHeatTransfer (CFD side). Called once per (i,j) pair found
 * within radCut by the neighbour sweep in thermalInteractionKernels.
 *
 * @param T_j    Temperature of the neighbouring particle j [K].
 * @param sumT   [IN/OUT] Running sum of neighbour temperatures for i.
 * @param count  [IN/OUT] Running count of radiating neighbours for i.
 */
KOKKOS_INLINE_FUNCTION
void accumulateNeighborTemperature(
    real     T_j,
    real&    sumT,
    uint32&  count)
{
    sumT += T_j;
    count++;
}

} // thermalRadiationKernels
} // pFlow

#endif // pFlow_thermalRadiationKernels_hpp







