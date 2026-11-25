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

#ifndef pFlow_thermalWallKernels_hpp
#define pFlow_thermalWallKernels_hpp

#include "types.hpp"

namespace pFlow
{

/**
 * @brief Device functions of the particle-wall heat transfer
 * (thermalWallMechanism).
 *
 * @details
 * The wall is taken as a particle with an infinite radius and mass, as
 * in Section 6.3.2.2 of Norouzi et al. (2016), so the contact
 * conduction and its contact-radius correction are those of the
 * particle-particle pair with R_eff = R_p. The gas path cannot use the
 * Voronoi-cone form of the particle pairs (Eq. 6.165 needs a centre
 * distance), so it uses the static gas lens of Rong and Horio (1999)
 * between a sphere and a plane, in the form of Liu et al. (2021),
 * Eqs. 31-36. Radiation uses the particle-wall distribution factors of
 * Johnson et al. (2021).
 *
 * @cite Norouzi, H.R., Zarghami, R., Sotudeh-Gharebagh, R., Mostoufi,
 * N., 2016. Coupled CFD-DEM Modeling. Wiley, Sections 6.3.2.1-6.3.2.2.
 * @cite Liu, X., Deen, N.G., Tang, Y., 2021. On the treatment of
 * bed-to-wall heat transfer in CFD-DEM simulations of gas-fluidized
 * beds. Chem. Eng. Sci. 236, 116492.
 * @cite Zhang, L., Lu, H., 2021. Chem. Eng. J. 406, 126880 (lens
 * radius 1.4 R_p, minimum conduction distance 2.75e-8 m).
 * @cite Johnson, E., Tari, I., Baker, D., 2021. Powder Technol. 380,
 * 164-182.
 * @cite Ericson, C., 2005. Real-Time Collision Detection. Morgan
 * Kaufmann, Section 5.1.5 (closest point on a triangle).
 */
namespace thermalWallKernels
{

/// Closest point to p on the triangle (a, b, c), Ericson (2005).
INLINE_FUNCTION_HD
realx3 closestPointOnTriangle(
    const realx3&   p,
    const realx3&   a,
    const realx3&   b,
    const realx3&   c)
{
    const realx3 ab = b - a;
    const realx3 ac = c - a;
    const realx3 ap = p - a;

    const real d1 = dot(ab, ap);
    const real d2 = dot(ac, ap);

    if (d1 <= 0 && d2 <= 0) return a;

    const realx3 bp = p - b;
    const real d3 = dot(ab, bp);
    const real d4 = dot(ac, bp);

    if (d3 >= 0 && d4 <= d3) return b;

    const real vc = d1*d4 - d3*d2;

    if (vc <= 0 && d1 >= 0 && d3 <= 0)
    {
        const real v = d1 / (d1 - d3);
        return a + v*ab;
    }

    const realx3 cp = p - c;
    const real d5 = dot(ab, cp);
    const real d6 = dot(ac, cp);

    if (d6 >= 0 && d5 <= d6) return c;

    const real vb = d5*d2 - d1*d6;

    if (vb <= 0 && d2 >= 0 && d6 <= 0)
    {
        const real w = d2 / (d2 - d6);
        return a + w*ac;
    }

    const real va = d3*d6 - d5*d4;

    if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0)
    {
        const real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w*(c - b);
    }

    const real denom = real(1) / (va + vb + vc);
    const real v = vb*denom;
    const real w = vc*denom;

    return a + v*ab + w*ac;
}

/// Distance from p to the triangle (a, b, c).
INLINE_FUNCTION_HD
real distanceToTriangle(
    const realx3&   p,
    const realx3&   a,
    const realx3&   b,
    const realx3&   c)
{
    const realx3 d = p - closestPointOnTriangle(p, a, b, c);
    return Kokkos::sqrt(dot(d, d));
}

/**
 * @brief Contact radius of a particle on the wall, corrected for the
 * softened Young's modulus (Norouzi et al., Eqs. 6.166-6.167, wall as a
 * particle of infinite radius: R_eff = R_p).
 */
INLINE_FUNCTION_HD
real wallContactRadius(
    real    R,
    real    overlap,
    real    nuP,
    real    nuW,
    real    E0P,
    real    E0W,
    real    simYoungsModulus)
{
    const real termSim =
        (real(1) - nuP*nuP) / simYoungsModulus
        + (real(1) - nuW*nuW) / simYoungsModulus;

    const real termReal =
        (real(1) - nuP*nuP) / E0P + (real(1) - nuW*nuW) / E0W;

    const real c = Kokkos::pow(termReal / termSim, real(0.2));

    return c * Kokkos::sqrt(R * overlap);
}

/**
 * @brief Integral of r / l(r) dr from r1 to r2, with the local gap
 * l(r) = a - sqrt(R^2 - r^2) (Liu et al., Eq. 36, a = R + delta).
 *
 * Closed form with u = sqrt(R^2 - r^2):
 * (u2 - u1) + a ln[(a - u2)/(a - u1)]. Needs l > 0 on [r1, r2].
 */
INLINE_FUNCTION_HD
real lensIntegral(real R, real a, real r1, real r2)
{
    const real u1 = Kokkos::sqrt(Kokkos::max(R*R - r1*r1, real(0)));
    const real u2 = Kokkos::sqrt(Kokkos::max(R*R - r2*r2, real(0)));

    return (u2 - u1) + a * Kokkos::log((a - u2) / (a - u1));
}

/**
 * @brief Geometric conductance G [m] of the gas lens between a sphere
 * and a plane: Q_pfw = 2 pi k_f G (T_w - T_p).
 *
 * @details
 * Liu et al. (2021), Eq. 32, with delta the gap between the particle
 * surface and the wall (negative when they overlap):
 *   - s <= delta <= r_lens - R:  G = int_0^r_out r/l dr
 *   - 0 <= delta < s:            G = r_s^2/(2s) + int_r_s^r_out r/l dr
 *   - delta < 0:                 G = (r_s^2 - r_in^2)/(2s)
 *                                    + int_r_s^r_out r/l dr
 * with r_out = sqrt(r_lens^2 - (R + delta)^2) (Eq. 35), r_s where
 * l = s, and r_in where l = 0. For an overlap, delta is the overlap of
 * the corrected contact radius (the thermal overlap of Liu et al.).
 */
INLINE_FUNCTION_HD
real lensConductance(real R, real delta, real lensRatio, real s)
{
    const real rLens = lensRatio * R;

    if (delta > rLens - R) return real(0);

    const real a    = R + delta;
    const real rOut = Kokkos::min(
        Kokkos::sqrt(Kokkos::max(rLens*rLens - a*a, real(0))),
        R);

    if (delta >= s)
    {
        return lensIntegral(R, a, real(0), rOut);
    }

    const real rS = Kokkos::min(
        Kokkos::sqrt(Kokkos::max(R*R - (a - s)*(a - s), real(0))),
        rOut);

    real G = rS*rS / (real(2)*s);

    if (delta < 0)
    {
        G -= Kokkos::max(R*R - a*a, real(0)) / (real(2)*s);
    }

    if (rOut > rS)
    {
        G += lensIntegral(R, a, rS, rOut);
    }

    return Kokkos::max(G, real(0));
}

/// Lower grid index and weight of x in grid[off, off + n), clamped.
INLINE_FUNCTION_HD
void bracket(
    const deviceViewType1D<real>&   tab,
    uint32                          off,
    uint32                          n,
    real                            x,
    uint32&                         i0,
    real&                           w)
{
    if (x <= tab(off))
    {
        i0 = 0;
        w  = real(0);
        return;
    }

    if (x >= tab(off + n - 1))
    {
        i0 = n - 2;
        w  = real(1);
        return;
    }

    uint32 i = 0;

    while (i + 2 < n && tab(off + i + 1) <= x) ++i;

    i0 = i;
    w  = (x - tab(off + i)) / (tab(off + i + 1) - tab(off + i));
}

/**
 * @brief Particle-wall distribution factor D_iw of Johnson et al.
 * (2021), interpolated linearly in particle emissivity, wall
 * emissivity, solid fraction and distance.
 *
 * @details
 * tab holds the grids then the values of johnsonWallRDF:
 * [epsP(nEp), epsW(nEw), solidFrac(nSf), dist(nDist), rdf]. Values
 * outside a grid are clamped to its ends, except the distance: beyond
 * the last row D_iw is zero, and below one radius (an overlap) the
 * contact row is used.
 *
 * @param xi Centre-to-wall distance in particle radii.
 */
INLINE_FUNCTION_HD
real wallRDF(
    const deviceViewType1D<real>&   tab,
    uint32                          nEp,
    uint32                          nEw,
    uint32                          nSf,
    uint32                          nDist,
    real                            epsP,
    real                            epsW,
    real                            solidFrac,
    real                            xi)
{
    const uint32 offEw   = nEp;
    const uint32 offSf   = offEw + nEw;
    const uint32 offDist = offSf + nSf;
    const uint32 offRdf  = offDist + nDist;

    if (xi > tab(offDist + nDist - 1)) return real(0);

    uint32 iP, iW, iS, iD;
    real   wP, wW, wS, wD;

    bracket(tab, 0,       nEp,   epsP,      iP, wP);
    bracket(tab, offEw,   nEw,   epsW,      iW, wW);
    bracket(tab, offSf,   nSf,   solidFrac, iS, wS);
    bracket(tab, offDist, nDist, xi,        iD, wD);

    real D = real(0);

    for (uint32 a = 0; a < 2; ++a)
    {
        const real fa = a ? wP : real(1) - wP;

        for (uint32 b = 0; b < 2; ++b)
        {
            const real fb = b ? wW : real(1) - wW;

            for (uint32 c = 0; c < 2; ++c)
            {
                const real fc = c ? wD : real(1) - wD;

                for (uint32 e = 0; e < 2; ++e)
                {
                    const real fe = e ? wS : real(1) - wS;

                    const uint32 k =
                        (((iP + a)*nEw + iW + b)*nDist + iD + c)*nSf
                        + iS + e;

                    D += fa*fb*fc*fe*tab(offRdf + k);
                }
            }
        }
    }

    return D;
}

} // thermalWallKernels

} // pFlow

#endif // pFlow_thermalWallKernels_hpp
