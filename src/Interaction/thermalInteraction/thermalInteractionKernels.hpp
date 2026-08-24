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
#include "thermalRadiationKernels.hpp"
#include "thermalConductionPFPKernels.hpp"
#include <Kokkos_Atomic.hpp>

namespace pFlow
{
namespace thermalInteractionKernels
{

// Dynamic scheduling: neighbour counts vary strongly between dense and
// dilute regions of the particle bed, so static chunking would leave some
// threads idle while others are still sweeping crowded cells.
using policy = Kokkos::RangePolicy<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<pFlow::uint32>>;

/**
 * @brief Sweeps the 27-cell neighbourhood of every active particle
 * once, dispatching each pair to whichever mechanisms are present.
 * One shared search, not one per mechanism.
 *
 * Header-only, no matching .cpp -- same as thermalRadiationKernels.hpp
 * and thermalConductionPFPKernels.hpp.
 *
 * @tparam CalcRad  Radiation exists (compile-time) -- its branch
 *   compiles out entirely when false.
 * @tparam CalcCond Conduction (Q_pp) enabled (compile-time).
 * @tparam CalcPFP  PFP (Q_pfp) enabled (compile-time).
 * @param doRadThisStep Runtime: radiation only accumulates on steps
 *   satisfying radUpdateInterval_.
 */
template<bool CalcRad, bool CalcCond, bool CalcPFP>
void calcThermalInteractions(
    const pFlagTypeDevice&          m,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   temperature,
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
    // --- radiation group ---
    const bool                      doRadThisStep,
    const real                      radCutSq,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt,
    // --- conduction + PFP group ---
    const real                      simYoungsModulus,
    // static-contact conduction output [W]
    deviceViewType1D<real>          Q_pp,
    // PFP sub-grid output [W]
    deviceViewType1D<real>          Q_pfp)
{
    using namespace thermalRadiationKernels;
    using namespace thermalConductionPFPKernels;

    auto r = m.activeRange();
    
    Kokkos::parallel_for(
        "calcThermalInteractions", 
        policy(r.start(), r.end()), 
        KOKKOS_LAMBDA(uint32 i) 
        {
            if (m(i)) 
            {
                realx3 p_i = pos[i];
                real R_i   = 0.5 * diameter[i];
                real T_i   = temperature[i];
                
                real sumT = 0.0;
                uint32 count = 0;

                int32 c_x = static_cast<int32>(
                    (p_i.x() - domainMin.x()) / cellSize);
                int32 c_y = static_cast<int32>(
                    (p_i.y() - domainMin.y()) / cellSize);
                int32 c_z = static_cast<int32>(
                    (p_i.z() - domainMin.z()) / cellSize);

                // Sweep the immediate 27-cell neighborhood
                for (int32 cx = c_x - 1; cx <= c_x + 1; ++cx) 
                {
                    for (int32 cy = c_y - 1; cy <= c_y + 1; ++cy) 
                    {
                        for (int32 cz = c_z - 1; cz <= c_z + 1; ++cz) 
                        {
                            if (cx >= 0 && cx < numCells.x() && 
                                cy >= 0 && cy < numCells.y() && 
                                cz >= 0 && cz < numCells.z()) 
                            {
                                uint32 j = cellIter.start(cx, cy, cz);
                                
                                while (j != mapperNBS::CellIterator::NoPos) 
                                {
                                    if (i != j && m(j)) 
                                    {
                                        real dx = p_i.x() - pos[j].x();
                                        real dy = p_i.y() - pos[j].y();
                                        real dz = p_i.z() - pos[j].z();
                                        
                                        real distSq = dx*dx + dy*dy + dz*dz;
                                        
                                        // Radiation: all i-j pairs.
                                        if constexpr (CalcRad)
                                        {
                                            if (doRadThisStep &&
                                                distSq <= radCutSq) 
                                            {
                                                accumulateNeighborTemperature(
                                                    temperature[j],
                                                    sumT,
                                                    count);
                                            }
                                        }

                                        // Conduction & PFP: i < j only.
                                        if constexpr (CalcCond || CalcPFP)
                                        {
                                            if (i < j)
                                            {
                                                real R_j = 0.5*diameter[j];
                                                real sumRadiiSq = 
                                                    (R_i+R_j)*(R_i+R_j);
                                                
                                                bool isContact = 
                                                    (distSq < sumRadiiSq);
                                                real dist = sqrt(distSq);
                                                real rc_real = 0.0;

                                                // Runs whenever in contact
                                                // even if CalcCond is
                                                // false -- PFP needs
                                                // rc_real as r_sij; the
                                                // Q_pp write itself is
                                                // gated by CalcCond.
                                                if (isContact &&
                                                    dist > 1e-12)
                                                {
                                                    rc_real =
                                                        contactConduction(
                                                            R_i, R_j, dist,
                                                            nu[i], nu[j],
                                                            E0[i], E0[j],
                                                            simYoungsModulus,
                                                            K[i], K[j],
                                                            T_i,
                                                            temperature[j],
                                                            CalcCond,
                                                            Q_pp, i, j);
                                                }

                                                if constexpr (CalcPFP)
                                                {
                                                    if (dist > 1e-12)
                                                    {
                                                        real r_sij =
                                                            isContact
                                                                ? rc_real
                                                                : 0.0;

                                                        particleFluidParticle(
                                                            R_i, R_j, dist,
                                                            fluidKappa[i],
                                                            fluidKappa[j],
                                                            fluidAlpha[i],
                                                            fluidAlpha[j],
                                                            K[i], K[j],
                                                            T_i,
                                                            temperature[j],
                                                            r_sij,
                                                            Q_pfp, i, j);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    j = cellIter.next(j);
                                }
                            }
                        }
                    }
                }
                
                if constexpr (CalcRad) 
                {
                    if (doRadThisStep)
                    {
                        radSumTemp[i] = sumT;
                        radNumPrt[i]  = count;
                    }
                }
            }
        });
    
    Kokkos::fence();
}

} // thermalInteractionKernels
} // pFlow

#endif // pFlow_thermalInteractionKernels_hpp
