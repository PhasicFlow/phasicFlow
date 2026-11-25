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

#ifndef pFlow_particleModelFactor_hpp
#define pFlow_particleModelFactor_hpp

#include "multiReactionTypes.hpp"
#include "pFlowMacros.hpp"

/**
 * Rate of a reaction with a particle model (particleModel USCM or grain).
 *
 *   omega = k C^m s^2 pi d^2 f                                   [kmol/s]
 *
 * k is the Arrhenius rate constant on the outer surface, s = (1 - X)^(1/3)
 * the front position and f the factor of the resistances in series. With
 * k_s = k C^(m-1) and R = d/2:
 *
 *   Da    = k_s / k_c                           gas film
 *   Phi_p = k_s R / D_e                         diffusion in the particle
 *   Phi_g = k_s r_g^2 / (phi_A R D_P)           product layer of a grain
 *   s_o   = (Z + (1 - Z) s^3)^(1/3)
 *
 * USCM:   1/f = 1 + Phi_p s (1 - s/s_o) + Da s^2
 *
 * grain:  1/f = c [1 + Phi_g s (1 - s/s_o) + max(Phi_p s (1 - s), 1/eta - 1)]
 *               + Da s^2
 *         eta = 3 (phi coth(phi) - 1) / phi^2,   phi^2 = 3 Phi_p s^2
 *
 * c(sigma2, g, X) is read from the correction table of the reaction with
 * sigma2 = Phi_p/6 and g = Phi_g/6. Without a table the two pore terms are
 * combined as (a^3 + b^3)^(1/3) and c = 1.
 *
 * phi_A is the volume fraction of the solid reactant in the unreacted
 * particle, from the masses and true densities of the solids. The grains
 * of the reactant, of radius r_g, carry the reaction, so their surface is
 * 3 phi_A / r_g per unit particle volume. With rateBasis grain the constant
 * of the dictionary is per unit grain surface, k_g, and
 * k = k_g phi_A R / r_g; then Phi_g = k_g C^(m-1) r_g / D_P.
 */

namespace pFlow
{
namespace particleModel
{

/// Conversion X of a reaction from the masses of its solid reactant and
/// solid product: X = a n_P / (n_A + a n_P), a = |nu_A| / nu_P.
INLINE_FUNCTION_HD
real conversion(
    const clcKineticsDevice& kinetics,
    const int32              j,
    const real*              m_solid)
{
    const int32 iA = kinetics.solidReactantIdx[j];
    const int32 iP = kinetics.particleModel[j].solidProductIdx;

    const real nA = m_solid[iA] / kinetics.solidMw[iA];
    const real nP = m_solid[iP] / kinetics.solidMw[iP];

    const real a =
        -kinetics.stoicSolid[j * kinetics.N_solid + iA]
        / kinetics.stoicSolid[j * kinetics.N_solid + iP];

    const real den = nA + a * nP;

    if (den <= real(0)) return real(1);

    return a * nP / den;
}

/// Volume fraction of the solid reactant of reaction j in the unreacted
/// particle of diameter d: the reactant now in the particle and the
/// reactant that has formed the solid product.
INLINE_FUNCTION_HD
real reactantVolumeFraction(
    const clcKineticsDevice& kinetics,
    const int32              j,
    const real*              m_solid,
    const real               d)
{
    const int32 iA = kinetics.solidReactantIdx[j];
    const int32 iP = kinetics.particleModel[j].solidProductIdx;

    const real a =
        -kinetics.stoicSolid[j * kinetics.N_solid + iA]
        / kinetics.stoicSolid[j * kinetics.N_solid + iP];

    const real nA0 =
        m_solid[iA] / kinetics.solidMw[iA]
        + a * m_solid[iP] / kinetics.solidMw[iP];

    const real V_A0 = nA0 * kinetics.solidMw[iA] / kinetics.solidDensities[iA];

    return real(6) * V_A0 / (Kokkos::numbers::pi_v<real> * d * d * d);
}

/// Effectiveness factor of a sphere, eta = 3 (phi coth(phi) - 1) / phi^2.
INLINE_FUNCTION_HD
real effectiveness(const real phi)
{
    if (phi < real(1e-3)) return real(1) - phi * phi / real(15);

    return real(3) * (phi / Kokkos::tanh(phi) - real(1)) / (phi * phi);
}

/// Diffusivity in the particle at temperature T: the molecular part
/// follows T^1.75 and the Knudsen part T^0.5, combined in series.
INLINE_FUNCTION_HD
real diffusivity(const clcParticleModelParams& pm, const real T)
{
    real D_m = pm.D_e;
    real D_K = pm.D_eKnudsen;

    if (pm.D_eTemperature > real(0))
    {
        const real Tr = T / pm.D_eTemperature;

        D_m *= Kokkos::pow(Tr, real(1.75));
        D_K *= Kokkos::sqrt(Tr);
    }

    if (D_K > real(0)) return D_m * D_K / (D_m + D_K);

    return D_m;
}

/// Index i of the interval of the ascending grid x[0..n-1] that holds v,
/// with the weight w of x[i+1]; v outside the grid is held at the edge.
INLINE_FUNCTION_HD
void tableInterval(
    const real* x,
    const int32 n,
    const real  v,
    int32&      i,
    real&       w)
{
    i = 0;
    w = real(0);

    if (n < 2 || v <= x[0]) return;

    if (v >= x[n - 1])
    {
        i = n - 2;
        w = real(1);
        return;
    }

    int32 lo = 0;
    int32 hi = n - 1;

    while (hi - lo > 1)
    {
        const int32 mid = (lo + hi) / 2;

        if (v >= x[mid]) lo = mid;
        else             hi = mid;
    }

    i = lo;
    w = (v - x[lo]) / (x[lo + 1] - x[lo]);
}

/// Correction c(sigma2, g, X) of a reaction: linear in ln(sigma2), g and X
/// on ln c. The table holds ln(sigma2), g, X and then ln c with the index
/// (i*nG + j)*nX + k.
INLINE_FUNCTION_HD
real correction(
    const clcParticleModelParams& pm,
    const real*                   table,
    const real                    sigma2,
    const real                    g,
    const real                    X)
{
    if (pm.tableOffset < 0) return real(1);

    const real* lnS = table + pm.tableOffset;
    const real* gG  = lnS + pm.nSigma2;
    const real* xG  = gG + pm.nG;
    const real* lnc = xG + pm.nX;

    int32 i, jg, k;
    real  wi, wj, wk;

    tableInterval(
        lnS, pm.nSigma2,
        Kokkos::log(Kokkos::max(sigma2, real(1e-300))),
        i, wi);
    tableInterval(gG, pm.nG, g, jg, wj);
    tableInterval(xG, pm.nX, X, k, wk);

    const int32 i1 = pm.nSigma2 > 1 ? i  + 1 : i;
    const int32 j1 = pm.nG      > 1 ? jg + 1 : jg;
    const int32 k1 = pm.nX      > 1 ? k  + 1 : k;

    real v = real(0);

    for (int32 a = 0; a < 2; ++a)
    {
        for (int32 b = 0; b < 2; ++b)
        {
            const int32 row = ((a ? i1 : i) * pm.nG + (b ? j1 : jg)) * pm.nX;

            const real atX =
                (real(1) - wk) * lnc[row + k] + wk * lnc[row + k1];

            v += (a ? wi : real(1) - wi) * (b ? wj : real(1) - wj) * atX;
        }
    }

    return Kokkos::exp(v);
}

/// Rate omega [kmol/s] of reaction j with its particle model.
/// k_rate is the Arrhenius constant of the dictionary at the particle
/// temperature T, C_gas the gas concentration and d the particle diameter.
/// k_cParticle is the gas-film coefficient of the particle; with 0 the
/// reaction takes k_c of its dictionary.
INLINE_FUNCTION_HD
real rate(
    const clcKineticsDevice& kinetics,
    const int32              j,
    const real*              m_solid,
    const real               T,
    const real               C_gas,
    const real               k_rate,
    const real               d,
    const real               k_cParticle)
{
    const clcParticleModelParams& pm = kinetics.particleModel[j];

    const real X = conversion(kinetics, j, m_solid);
    const real s = Kokkos::cbrt(Kokkos::max(real(1) - X, real(0)));

    if (C_gas <= real(0) || s <= real(0)) return real(0);

    const real R = real(0.5) * d;
    const real m = kinetics.gasOrder[j];

    real k     = k_rate;
    real phi_A = real(0);

    if (pm.model == clcModelGrain)
    {
        phi_A = reactantVolumeFraction(kinetics, j, m_solid, d);

        if (pm.grainBasis) k *= phi_A * R / pm.grainRadius;
    }

    const real k_s =
        (m == real(1)) ? k : k * Kokkos::pow(C_gas, m - real(1));

    const real D_e   = diffusivity(pm, T);
    const real Phi_p = k_s * R / D_e;
    const real k_c   = k_cParticle > real(0) ? k_cParticle : pm.k_c;
    const real Da    = k_c > real(0) ? k_s / k_c : real(0);
    const real s_o   = Kokkos::cbrt(pm.Z + (real(1) - pm.Z) * s * s * s);

    real invF;

    if (pm.model == clcModelUSCM)
    {
        invF = real(1) + Phi_p * s * (real(1) - s / s_o) + Da * s * s;
    }
    else
    {
        const real Phi_g =
            pm.D_P > real(0)
            ? k_s * pm.grainRadius * pm.grainRadius
                / (phi_A * R * pm.D_P)
            : real(0);

        const real layer = Phi_g * s * (real(1) - s / s_o);
        const real pore  = Phi_p * s * (real(1) - s);
        const real start =
            real(1) / effectiveness(Kokkos::sqrt(real(3) * Phi_p) * s)
            - real(1);

        if (pm.tableOffset >= 0)
        {
            const real c = correction(
                pm, kinetics.correctionTable,
                Phi_p / real(6), Phi_g / real(6), X);

            invF = c * (real(1) + layer + Kokkos::max(pore, start))
                + Da * s * s;
        }
        else
        {
            invF = real(1) + layer + Da * s * s
                + Kokkos::cbrt(pore * pore * pore + start * start * start);
        }
    }

    return k_s * C_gas * s * s
        * Kokkos::numbers::pi_v<real> * d * d / invF;
}

} // particleModel
} // pFlow

#endif // pFlow_particleModelFactor_hpp
