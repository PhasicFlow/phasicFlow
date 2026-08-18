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

#ifndef pFlow_thermalConductionKernels_hpp
#define pFlow_thermalConductionKernels_hpp

#include "types.hpp"
#include <Kokkos_Atomic.hpp>

namespace pFlow
{
namespace thermalConductionKernels
{

/**
 * @brief Computes the Hertzian-corrected contact radius for a
 * particle pair already known to be in contact (dist > 1e-12), and,
 * when calcCond is true, the static-contact conduction rate Q_pp
 * (Eq. 6.159), applied atomically and symmetrically to both particles.
 *
 * @details
 * The contact radius is computed purely from geometry:
 * rc = sqrt(R_eff * overlap), the standard Hertzian relationship
 * between the normal interpenetration of two spheres and their
 * circular contact-patch radius. Because the simulation typically
 * uses a softened Young's modulus to allow a larger DEM timestep, the
 * resulting overlap -- and hence rc -- overstates what a real,
 * stiffer material would produce; a correction factor derived from
 * the ratio of simulated to real Young's modulus scales rc back down
 * (Eq. 6.166 & 6.167) before it is used in the heat transfer rate.
 *
 * @return The corrected contact radius rc_real -- needed by the PFP
 * model too (as its lower integration limit r_sij), even when
 * calcCond itself is off, so it is always computed and returned
 * whenever this function is called for a contacting pair.
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

    // Inverse-modulus quantities for the simulation's Young's
    // modulus and the material's real Young's modulus.
    real term_E_sim = 
        (1.0 - nu_i*nu_i) / simYoungsModulus + 
        (1.0 - nu_j*nu_j) / simYoungsModulus;
        
    real term_E_real = 
        (1.0 - nu_i*nu_i) / E0_i + 
        (1.0 - nu_j*nu_j) / E0_j;

    // Geometric contact radius from the normal overlap of the two
    // spheres: a^2 = R_eff*overlap, the standard Hertzian
    // relationship between interpenetration and contact-patch
    // radius. Purely geometric - no relative velocity or
    // contact-time model involved, matching the static/sustained
    // contact regime of Eq. 6.159.
    real overlap = (R_i + R_j) - dist;
    real rc_geom = sqrt(R_eff * overlap);

    // Contact radius correction: the overlap
    // above was produced using the simulation's Young's modulus, so
    // rc_geom overstates the contact radius a real, stiffer material
    // would give for the same geometric interpenetration.
    // c = (E_sim/E_real)^0.2; in terms of the inverse-modulus
    // quantities above (term_E=1/E) this is:
    // c = (term_E_real/term_E_sim)^.2
    real c_corr = pow(term_E_real / term_E_sim, 0.2);

    real rc_real = c_corr * rc_geom;

    if (calcCond)
    {
        real tempDiff = T_j - T_i;
        real num = 4.0 * rc_real * tempDiff;
        real den = (1.0 / K_i) + (1.0 / K_j);
        real Q_rate = num / den;

        // Atomic accumulation for thread safety
        Kokkos::atomic_add(&Q_pp[i], Q_rate);
        Kokkos::atomic_add(&Q_pp[j], -Q_rate);
    }

    return rc_real;
}

} // thermalConductionKernels
} // pFlow

#endif // pFlow_thermalConductionKernels_hpp
