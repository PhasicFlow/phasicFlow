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

#ifndef pFlow_thermalPFPKernels_hpp
#define pFlow_thermalPFPKernels_hpp

#include "types.hpp"
#include <Kokkos_Atomic.hpp>

namespace pFlow
{
namespace thermalPFPKernels
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

/**
 * @brief Computes and applies the Particle-Fluid-Particle sub-grid
 * heat transfer rate Q_pfp (Eq. 6.160) for a pair, when the
 * dimensionless gap H/R* <= 0.5, via 5-point Gauss-Legendre
 * quadrature (Rong & Horio, 1999). Applied atomically and
 * symmetrically to both particles.
 *
 * @param r_sij Lower integration limit (Eq. 6.164) -- the Hertzian
 * contact radius when the pair is in contact, 0 otherwise. Callers
 * pass thermalConductionKernels::contactConduction()'s return value
 * when isContact is true, and 0.0 otherwise, matching the original
 * ternary this function replaces.
 */
KOKKOS_INLINE_FUNCTION
void particleFluidParticle(
    real                     R_i,
    real                     R_j,
    real                     dist,
    real                     fluidKappa_i,
    real                     fluidKappa_j,
    real                     fluidAlpha_i,
    real                     fluidAlpha_j,
    real                     K_i,
    real                     K_j,
    real                     T_i,
    real                     T_j,
    real                     r_sij,
    deviceViewType1D<real>   Q_pfp,
    uint32                   i,
    uint32                   j)
{
    real R_star = 0.5 * (R_i + R_j);
    real H = 0.5 * (dist - R_i - R_j);
    real k_f = 0.5 * (fluidKappa_i + fluidKappa_j);

    // Cut-off rule: Ignored if H/R* > 0.5
    if (H / R_star <= 0.5 && k_f > 1e-12)
    {
        // Eq. 6.164: r_ij evaluation based on local porosity
        real eps_avg = 0.5 * (fluidAlpha_i + fluidAlpha_j);

        real solid_frac = 1.0 - eps_avg;

        if (solid_frac < 0.01) 
        {
            // Clamp to avoid inf
            solid_frac = 0.01; 
        }

        real r_ij = 0.56 * R_star * pow(solid_frac, -1.0/3.0);

        // Eq. 6.162: r_sf (Upper limit of integration)
        real R_H = R_star + (H > 0.0 ? H : 0.0);

        real r_sf = (R_star * r_ij) / sqrt(r_ij*r_ij + R_H*R_H);

        if (r_sf > r_sij)
        {
            // Loop-free 5-point Gauss-Legendre Quadrature
            real A = r_sij;
            real B = r_sf;
            real c1 = 0.5 * (B - A);
            real c2 = 0.5 * (A + B);
            real pi = Kokkos::numbers::pi_v<real>;

            real integral = 0.0;

            // Explicit unrolled loop for GPU registers (Thread-safe)
            for (int k = 0; k < 5; ++k)
            {
                real r_pt = c1 * t_GL[k] + c2;
                real r2 = r_pt*r_pt;

                real Ri2 = R_i*R_i;
                real root_i = 0.0;
                if (Ri2 > r2)
                {
                    root_i = sqrt(Ri2 - r2);
                }

                real Rj2 = R_j*R_j;
                real root_j = 0.0;
                if (Rj2 > r2)
                {
                    root_j = sqrt(Rj2 - r2);
                }

                // Lens gap physical thickness
                real gap = dist - root_i - root_j;

                if (gap < 0.0) 
                {
                    gap = 0.0; 
                }

                real term_i = (R_i - root_i) / K_i;
                real term_j = (R_j - root_j) / K_j;
                real term_f = gap / k_f;

                real R_th = term_i + term_j + term_f;

                // Avoid division by zero at perfect rigid contact
                // centers
                if (R_th > 1e-12)
                {
                    real F = (2.0 * pi * r_pt) / R_th;
                    integral += w_GL[k] * F;
                }
            }
            integral *= c1;

            // Apply integrated PFP flux symmetrically
            real Q_pfp_val = integral * (T_j - T_i);

            Kokkos::atomic_add(&Q_pfp[i], Q_pfp_val);
            Kokkos::atomic_add(&Q_pfp[j], -Q_pfp_val);
        }
    }
}

} // thermalPFPKernels
} // pFlow

#endif // pFlow_thermalPFPKernels_hpp







