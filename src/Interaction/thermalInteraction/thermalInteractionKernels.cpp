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
#include <Kokkos_Atomic.hpp>

namespace pFlow
{
namespace thermalInteractionKernels
{

// Gauss-Legendre quadrature abscissae/weights for the PFP flux integral.
// Kept at file scope (rather than re-built per thread) to reduce
// per-thread register pressure on the GPU.
KOKKOS_INLINE_FUNCTION 
constexpr real t_GL[5] = {
    -0.9061798459, -0.5384693101, 0.0, 0.5384693101, 0.9061798459
};

KOKKOS_INLINE_FUNCTION 
constexpr real w_GL[5] = {
    0.2369268850,  0.4786286705, 0.5688888889, 0.4786286705, 0.2369268850
};

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
                
                real pi = Kokkos::numbers::pi_v<real>;
                
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
                                        
                                        // =====================================
                                        // 1. Radiation Check (Asymmetric 
                                        //    execution, all i-j pairs)
                                        // =====================================
                                        if (calcRad && distSq <= radCutSq) 
                                        {
                                            sumT += temperature[j];
                                            count++;
                                        }

                                        // =====================================
                                        // 2. Conduction & PFP Checks (Symmetric
                                        //    execution: i < j only). Computing 
                                        //    both forces here halves the 
                                        //    computational load.
                                        // =====================================
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
                                                real R_eff = 
                                                    (R_i * R_j) / (R_i + R_j);

                                                // Inverse-modulus quantities 
                                                // for the simulation's Young's 
                                                // modulus and the material's 
                                                // real Young's modulus.
                                                real term_E_sim = 
                                                    (1.0 - nu[i]*nu[i]) / 
                                                    simYoungsModulus + 
                                                    (1.0 - nu[j]*nu[j]) / 
                                                    simYoungsModulus;
                                                    
                                                real term_E_real = 
                                                    (1.0 - nu[i]*nu[i]) / E0[i]+ 
                                                    (1.0 - nu[j]*nu[j]) / E0[j];

                                                // Geometric contact radius from
                                                // the normal overlap of the two
                                                // spheres: a^2 = R_eff*overlap,
                                                // the standard Hertzian 
                                                // relationship between 
                                                // interpenetration and 
                                                // contact-patch radius. Purely 
                                                // geometric - no relative 
                                                // velocity or contact-time 
                                                // model involved, matching the 
                                                // static/sustained contact 
                                                // regime of Eq. 6.159.
                                                real overlap = 
                                                    (R_i + R_j) - dist;
                                                real rc_geom = 
                                                    sqrt(R_eff * overlap);

                                                // Contact radius correction 
                                                // (Eq. 6.166 & 6.167): the 
                                                // overlap above was produced 
                                                // using the simulation's 
                                                // Young's modulus, so rc_geom 
                                                // overstates the contact radius
                                                // a real, stiffer material 
                                                // would give for the same 
                                                // geometric interpenetration. 
                                                // c = (E_sim/E_real)^0.2; in 
                                                // terms of the inverse-modulus 
                                                // quantities above (term_E=1/E)
                                                // this is:
                                                // c=(term_E_real/term_E_sim)^.2
                                                real c_corr = 
                                                    pow(term_E_real / 
                                                        term_E_sim, 
                                                        0.2);
                                                        
                                                rc_real = c_corr * rc_geom;

                                                if (calcCond)
                                                {
                                                    real tempDiff = 
                                                        temperature[j] - T_i;
                                                    real num = 
                                                        4.0 * rc_real * 
                                                        tempDiff;
                                                    real den = 
                                                        (1.0 / K[i]) + 
                                                        (1.0 / K[j]);
                                                    real Q_rate = num / den;

                                                    // Atomic accumulation for 
                                                    // thread safety
                                                    Kokkos::atomic_add(
                                                        &Q_pp[i], Q_rate);
                                                    Kokkos::atomic_add(
                                                        &Q_pp[j], -Q_rate);
                                                }
                                            }

                                            // --- 2.B: Particle-Fluid-Particle 
                                            //          Sub-grid Heat Transfer 
                                            //          (Eq. 6.160) ---
                                            if (calcPFP && dist > 1e-12)
                                            {
                                                real R_star = 0.5 * (R_i + R_j);
                                                real H = 0.5 * 
                                                    (dist - R_i - R_j);
                                                real k_f = 0.5 * 
                                                    (fluidKappa[i] + 
                                                     fluidKappa[j]);

                                                // Cut-off rule: Ignored if 
                                                // H/R* > 0.5
                                                if (H / R_star <= 0.5 && 
                                                    k_f > 1e-12)
                                                {
                                                    // Eq. 6.164: r_ij 
                                                    // evaluation based on 
                                                    // local porosity
                                                    real r_sij = 
                                                        isContact ? 
                                                        rc_real : 0.0;
                                                        
                                                    real eps_avg = 0.5 * 
                                                        (fluidAlpha[i] + 
                                                         fluidAlpha[j]);
                                                         
                                                    real solid_frac = 
                                                        1.0 - eps_avg;
                                                        
                                                    if (solid_frac < 0.01) 
                                                    {
                                                        // Clamp to avoid inf
                                                        solid_frac = 0.01; 
                                                    }
                                                    
                                                    real r_ij = 0.56 * R_star * 
                                                        pow(solid_frac, 
                                                            -1.0/3.0);
                                                    
                                                    // Eq. 6.162: r_sf (Upper 
                                                    // limit of integration)
                                                    real R_H = R_star + 
                                                        (H > 0.0 ? H : 0.0);
                                                        
                                                    real r_sf = (R_star * r_ij)/ 
                                                        sqrt(r_ij*r_ij + 
                                                             R_H*R_H);

                                                    if (r_sf > r_sij)
                                                    {
                                                        // Loop-free 5-point 
                                                        // Gauss-Legendre 
                                                        // Quadrature
                                                        real A = r_sij;
                                                        real B = r_sf;
                                                        real c1 = 0.5 * (B - A);
                                                        real c2 = 0.5 * (A + B);

                                                        real integral = 0.0;
                                                        
                                                        // Explicit unrolled 
                                                        // loop for GPU 
                                                        // registers 
                                                        // (Thread-safe)
                                                        for (int k=0; k<5; ++k)
                                                        {
                                                            real r_pt = 
                                                                c1 * t_GL[k] + 
                                                                c2;
                                                            real r2 = r_pt*r_pt;
                                                            
                                                            real Ri2 = R_i*R_i;
                                                            real root_i = 0.0;
                                                            if (Ri2 > r2)
                                                            {
                                                                root_i = 
                                                                    sqrt(Ri2 - 
                                                                         r2);
                                                            }
                                                            
                                                            real Rj2 = R_j*R_j;
                                                            real root_j = 0.0;
                                                            if (Rj2 > r2)
                                                            {
                                                                root_j = 
                                                                    sqrt(Rj2 - 
                                                                         r2);
                                                            }

                                                            // Lens gap physical
                                                            // thickness
                                                            real gap = dist - 
                                                                root_i - root_j;
                                                                
                                                            if (gap < 0.0) 
                                                            {
                                                                gap = 0.0; 
                                                            }

                                                            real term_i = 
                                                                (R_i - root_i) / 
                                                                K[i];
                                                            real term_j = 
                                                                (R_j - root_j) / 
                                                                K[j];
                                                            real term_f = 
                                                                gap / k_f;

                                                            real R_th = term_i + 
                                                                term_j + term_f;

                                                            // Avoid division by
                                                            // zero at perfect 
                                                            // rigid contact 
                                                            // centers
                                                            if (R_th > 1e-12)
                                                            {
                                                                real F = 
                                                                    (2.0 * pi * 
                                                                    r_pt)/R_th;
                                                                integral += 
                                                                    w_GL[k] * F;
                                                            }
                                                        }
                                                        integral *= c1;

                                                        // Apply integrated PFP 
                                                        // flux symmetrically
                                                        real Q_pfp_val = 
                                                            integral * 
                                                            (temperature[j] - 
                                                             T_i);
                                                             
                                                        Kokkos::atomic_add(
                                                            &Q_pfp[i], 
                                                            Q_pfp_val);
                                                        Kokkos::atomic_add(
                                                            &Q_pfp[j], 
                                                            -Q_pfp_val);
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



