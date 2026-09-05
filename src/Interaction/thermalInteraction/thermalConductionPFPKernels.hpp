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

#ifndef pFlow_thermalConductionPFPKernels_hpp
#define pFlow_thermalConductionPFPKernels_hpp

#include "types.hpp"
#include <Kokkos_Atomic.hpp>

namespace pFlow
{
namespace thermalConductionPFPKernels
{

// Conduction (Q_pp) and PFP (Q_pfp) share this file: contactConduction()
// always computes the Hertzian contact radius, which particleFluidParticle()
// needs as its lower integration limit even when Q_pp itself is disabled.

/**
 * @brief Hertzian-corrected contact radius for a contacting pair;
 * also applies the static-contact conduction rate Q_pp when calcCond
 * is true.
 *
 * @return Corrected contact radius -- always computed and returned,
 * since PFP needs it even when calcCond is false.
 */
KOKKOS_INLINE_FUNCTION
real contactConduction(
    real                     R_i,
    real                     R_j,
    real                     dist,
    real                     nu_i,
    real                     nu_j,
    real                     E0_i,
    real                     E0_j,
    real                     simYoungsModulus,
    real                     K_i,
    real                     K_j,
    real                     T_i,
    real                     T_j,
    bool                     calcCond,
    deviceViewType1D<real>   Q_pp,
    uint32                   i,
    uint32                   j)
{
    real R_eff = (R_i * R_j) / (R_i + R_j);

    // Inverse-modulus terms for the simulated vs. real Young's modulus.
    real term_E_sim = 
        (1.0 - nu_i*nu_i) / simYoungsModulus + 
        (1.0 - nu_j*nu_j) / simYoungsModulus;
        
    real term_E_real = 
        (1.0 - nu_i*nu_i) / E0_i + 
        (1.0 - nu_j*nu_j) / E0_j;

    // Geometric contact radius from normal overlap (Hertzian).
    real overlap = (R_i + R_j) - dist;
    real rc_geom = sqrt(R_eff * overlap);

    // A softened simulated Young's modulus overstates the real
    // contact radius; scale back down by (E_sim/E_real)^0.2.
    real c_corr = pow(term_E_real / term_E_sim, 0.2);

    real rc_real = c_corr * rc_geom;

    if (calcCond)
    {
        real tempDiff = T_j - T_i;
        real num = 4.0 * rc_real * tempDiff;
        real den = (1.0 / K_i) + (1.0 / K_j);
        real Q_rate = num / den;

        Kokkos::atomic_add(&Q_pp[i], Q_rate);
        Kokkos::atomic_add(&Q_pp[j], -Q_rate);
    }

    return rc_real;
}

// Gauss-Legendre quadrature points/weights for the PFP flux integral.
// File scope to avoid rebuilding per thread.
KOKKOS_INLINE_FUNCTION 
constexpr real t_GL[5] = {
    -0.9061798459, -0.5384693101, 0.0, 0.5384693101, 0.9061798459
};

KOKKOS_INLINE_FUNCTION 
constexpr real w_GL[5] = {
    0.2369268850,  0.4786286705, 0.5688888889, 0.4786286705, 0.2369268850
};

/**
 * @brief Particle-Fluid-Particle sub-grid heat transfer Q_pfp, when
 * the dimensionless gap H/R* <= 0.5, via 5-point Gauss-Legendre
 * quadrature (Rong & Horio, 1999).
 *
 * @param r_sij Lower integration limit -- the Hertzian contact
 * radius when in contact, 0 otherwise.
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

    if (H / R_star <= 0.5 && k_f > 1e-12)
    {
        // Porosity-weighted radial reach of the fluid bridge.
        real eps_avg = 0.5 * (fluidAlpha_i + fluidAlpha_j);
        real solid_frac = 1.0 - eps_avg;

        if (solid_frac < 0.01) 
        {
            solid_frac = 0.01; 
        }

        real r_ij = 0.56 * R_star * pow(solid_frac, -1.0/3.0);

        // Upper integration limit.
        real R_H = R_star + (H > 0.0 ? H : 0.0);
        real r_sf = (R_star * r_ij) / sqrt(r_ij*r_ij + R_H*R_H);

        if (r_sf > r_sij)
        {
            real A = r_sij;
            real B = r_sf;
            real c1 = 0.5 * (B - A);
            real c2 = 0.5 * (A + B);
            real pi = Kokkos::numbers::pi_v<real>;

            real integral = 0.0;

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

                real gap = dist - root_i - root_j;

                if (gap < 0.0) 
                {
                    gap = 0.0; 
                }

                real term_i = (R_i - root_i) / K_i;
                real term_j = (R_j - root_j) / K_j;
                real term_f = gap / k_f;

                real R_th = term_i + term_j + term_f;

                // Avoid division by zero at perfect rigid contact.
                if (R_th > 1e-12)
                {
                    real F = (2.0 * pi * r_pt) / R_th;
                    integral += w_GL[k] * F;
                }
            }
            integral *= c1;

            real Q_pfp_val = integral * (T_j - T_i);

            Kokkos::atomic_add(&Q_pfp[i], Q_pfp_val);
            Kokkos::atomic_add(&Q_pfp[j], -Q_pfp_val);
        }
    }
}

} // thermalConductionPFPKernels
} // pFlow

#endif // pFlow_thermalConductionPFPKernels_hpp
