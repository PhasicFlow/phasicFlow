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

#include "thermalInteractionKernels.hpp"
#include "thermalRadiationKernels.hpp"
#include "thermalConductionKernels.hpp"
#include "thermalPFPKernels.hpp"
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
    const deviceViewType1D<real>&   fluidKappa,
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
    deviceViewType1D<real>          Q_pp,
    deviceViewType1D<real>          Q_pfp,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt)
{
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
                
                real radCutSq = radCut * radCut;
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
                                        
                                        //--- radiation check (asymmetric
                                        // execution, all i-j pairs) ------
                                        if (calcRad && distSq <= radCutSq) 
                                        {
                                            thermalRadiationKernels::
                                                accumulateNeighborTemperature(
                                                    temperature[j],
                                                    sumT,
                                                    count);
                                        }

                                        //--- conduction & PFP checks
                                        // (symmetric execution: i < j
                                        // only). Computing both forces
                                        // here halves the computational
                                        // load. -------------------------
                                        if ((calcCond || calcPFP) && i < j)
                                        {
                                            real R_j = 0.5 * diameter[j];
                                            real sumRadiiSq = 
                                                (R_i + R_j) * (R_i + R_j);
                                            
                                            bool isContact = 
                                                (distSq < sumRadiiSq);
                                            real dist = sqrt(distSq);
                                            real rc_real = 0.0;

                                            // --- 2.A: Static Contact 
                                            //          Conduction 
                                            //          (Eq. 6.159) ---
                                            if (isContact && dist > 1e-12)
                                            {
                                                rc_real = 
                                                    thermalConductionKernels::
                                                        contactConduction(
                                                            R_i, R_j, dist,
                                                            nu[i], nu[j],
                                                            E0[i], E0[j],
                                                            simYoungsModulus,
                                                            K[i], K[j],
                                                            T_i, temperature[j],
                                                            calcCond,
                                                            Q_pp, i, j);
                                            }

                                            // --- 2.B: Particle-Fluid-Particle 
                                            //          Sub-grid Heat Transfer 
                                            //          (Eq. 6.160) ---
                                            if (calcPFP && dist > 1e-12)
                                            {
                                                real r_sij = 
                                                    isContact ? rc_real : 0.0;

                                                thermalPFPKernels::
                                                    particleFluidParticle(
                                                        R_i, R_j, dist,
                                                        fluidKappa[i],
                                                        fluidKappa[j],
                                                        fluidAlpha[i],
                                                        fluidAlpha[j],
                                                        K[i], K[j],
                                                        T_i, temperature[j],
                                                        r_sij,
                                                        Q_pfp, i, j);
                                            }
                                        }
                                    }
                                    j = cellIter.next(j);
                                }
                            }
                        }
                    }
                }
                
                // Finalize Radiation (Saves sum to memory)
                if (calcRad) 
                {
                    radSumTemp[i] = sumT;
                    radNumPrt[i]  = count;
                }
            }
        });
    
    Kokkos::fence();
}

} // thermalInteractionKernels
} // pFlow
