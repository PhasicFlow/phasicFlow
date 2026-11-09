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

#include <limits>

#include "multiShrinkingInteractionKernels.hpp"

namespace pFlow
{
namespace multiShrinkingKernels
{

// Dynamic scheduling: particles with and without an active reaction do very
// different amounts of work
using DynamicPolicy = Kokkos::RangePolicy
<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<pFlow::int32>
>;

// Static scheduling: the reduction does the same work for every particle
using StaticPolicy = Kokkos::RangePolicy
<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Static>,
    Kokkos::IndexType<pFlow::uint32>
>;

/// Evaluates the reactions of one particle; Shrinking selects the
/// diameter update.
template<bool Shrinking>
KOKKOS_INLINE_FUNCTION
void calcMultiReactionSingleParticle
(
    const int32                    i,
    const real                     dt,
    const pFlagTypeDevice&         mask,
    const deviceViewType1D<real>&  T_p,
    const deviceViewType1D<real>&  gasConcs,
    const clcKineticsDevice        kinetics,
    deviceViewType1D<real>         solidFracs,
    deviceViewType1D<real>         totalMass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia,
    deviceViewType1D<real>         gasMassSource,
    deviceViewType1D<real>         gasMassSourceSp,
    deviceViewType1D<real>         qrxn_particle
)
{
    if (!mask(i)) return;

    const real d     = diameter [i];
    const real m_tot = totalMass[i];
    const real T     = T_p[i];

    // Particle surface area [m^2]
    const real A_p = Kokkos::numbers::pi_v<real> * d * d;

    real q_total = real(0);
    real m_solid[clcMaxSpecies] = {};
    real gas_src [clcMaxSpecies] = {};
    real gas_Sp  [clcMaxSpecies] = {};

    for (int32 s = 0; s < kinetics.N_solid; ++s)
        m_solid[s] = solidFracs[i * kinetics.N_solid + s] * m_tot;

    // Reaction rates omega [kmol/s]
    real omega[clcMaxSpecies] = {};
    bool anyReactionOn = false;

    for (int32 j = 0; j < kinetics.N_rxn; ++j)
    {
        const int32 gasIdx = kinetics.gasSpeciesIdx   [j];
        const int32 solIdx = kinetics.solidReactantIdx[j];

        // Each reaction has its own onset temperature
        if (T < kinetics.onsetTemperature[j]) continue;

        anyReactionOn = true;

        if (m_solid[solIdx] <= real(1e-15)) continue;
        if (m_tot           <= real(1e-15)) continue;

        const real C_gas =
            Kokkos::max(gasConcs[i * kinetics.N_gas + gasIdx], real(0));

        // Mass fraction of the tracked solid reactant
        const real Y_solid = m_solid[solIdx] / m_tot;

        const real k_rate =
            kinetics.preExpFactor[j]
            * Kokkos::exp(-kinetics.activationTemperature[j] / T);

        omega[j] =
            k_rate
            * Kokkos::pow(C_gas,   kinetics.gasOrder[j])
            * Kokkos::pow(Y_solid, kinetics.solidOrder[j])
            * A_p;
    }

    // No reaction is active at this temperature: leave the particle as it is
    if (!anyReactionOn) return;

    // Mass of each solid species that the rates would consume within dt
    real consumed[clcMaxSpecies] = {};

    for (int32 j = 0; j < kinetics.N_rxn; ++j)
    {
        if (omega[j] <= real(0)) continue;

        for (int32 s = 0; s < kinetics.N_solid; ++s)
        {
            const real nu_s = kinetics.stoicSolid[j * kinetics.N_solid + s];

            if (nu_s < real(0))
                consumed[s] += (-nu_s) * omega[j] * kinetics.solidMw[s] * dt;
        }
    }

    // A species cannot be consumed by more than the particle holds: the
    // reactions drawing on an exhausted species are scaled down together,
    // which keeps the products in their stoichiometric share
    for (int32 j = 0; j < kinetics.N_rxn; ++j)
    {
        if (omega[j] <= real(0)) continue;

        real scale = real(1);

        for (int32 s = 0; s < kinetics.N_solid; ++s)
        {
            if (   kinetics.stoicSolid[j * kinetics.N_solid + s] < real(0)
                && consumed[s] > m_solid[s])
            {
                scale = Kokkos::min(scale, m_solid[s] / consumed[s]);
            }
        }

        omega[j] *= scale;
    }

    // Apply the rates: gas and solid mass updates and reaction heat
    for (int32 j = 0; j < kinetics.N_rxn; ++j)
    {
        if (omega[j] <= real(0)) continue;

        const real omega_j = omega[j];

        // Picard linearisation of the gas sources (implicit stabilisation)
        for (int32 g = 0; g < kinetics.N_gas; ++g)
        {
            const real nu_g = kinetics.stoicGas[j * kinetics.N_gas + g];

            if (nu_g != real(0))
                gas_src[g] += nu_g * omega_j * kinetics.gasMw[g];

            if (nu_g < real(0))
            {
                const real C_g = Kokkos::max
                (
                    gasConcs[i * kinetics.N_gas + g],
                    real(1e-15)
                );

                gas_Sp[g] += (-nu_g) * kinetics.gasMw[g] * (omega_j / C_g);
            }
        }

        for (int32 s = 0; s < kinetics.N_solid; ++s)
        {
            const real nu_s = kinetics.stoicSolid[j * kinetics.N_solid + s];
            if (nu_s != real(0))
                m_solid[s] += nu_s * omega_j * kinetics.solidMw[s] * dt;
        }

        // IUPAC sign: dH > 0 endothermic, dH < 0 exothermic
        q_total -= omega_j * kinetics.dH_rxn[j];
    }

    // Updated total solid mass and volume
    real m_tot_new = real(0);
    real vol_new   = real(0);

    for (int32 s = 0; s < kinetics.N_solid; ++s)
    {
        m_solid[s] = Kokkos::max(m_solid[s], real(0));
        m_tot_new += m_solid[s];
        vol_new   += m_solid[s] / kinetics.solidDensities[s];
    }

    totalMass[i] = m_tot_new;

    if constexpr (Shrinking)
    {
        // The diameter follows the solid volume
        const real d_new = Kokkos::cbrt
        (
            real(6) * vol_new / Kokkos::numbers::pi_v<real>
        );
        diameter[i] = d_new;
        inertia[i]  = real(0.4) * m_tot_new
                      * (real(0.5) * d_new) * (real(0.5) * d_new);
    }
    else
    {
        // The diameter is frozen; the inertia follows the changing mass
        const real r_curr = real(0.5) * diameter[i];
        inertia[i] = real(0.4) * m_tot_new * r_curr * r_curr;
    }

    qrxn_particle[i] = q_total;

    // Solid mass fractions, normalised to sum exactly to one to remove the
    // floating-point drift of the repeated additive updates
    const real invMass = real(1) / Kokkos::max(m_tot_new, real(1e-15));
    real sum_fracs = real(0);

    for (int32 s = 0; s < kinetics.N_solid; ++s)
    {
        solidFracs[i * kinetics.N_solid + s] = m_solid[s] * invMass;
        sum_fracs += solidFracs[i * kinetics.N_solid + s];
    }

    sum_fracs = Kokkos::max(sum_fracs, real(1e-12));

    for (int32 s = 0; s < kinetics.N_solid; ++s)
    {
        solidFracs[i * kinetics.N_solid + s] /= sum_fracs;
    }

    for (int32 g = 0; g < kinetics.N_gas; ++g)
    {
        gasMassSource  [i * kinetics.N_gas + g] = gas_src[g];
        gasMassSourceSp[i * kinetics.N_gas + g] = gas_Sp [g];
    }
}

/// Launches the reaction kernel for the chosen diameter model.
template<bool Shrinking>
void launchMultiReaction
(
    const pFlagTypeDevice&         mask,
    const real                     dt,
    const deviceViewType1D<real>&  T_p,
    const deviceViewType1D<real>&  gasConcs,
    const clcKineticsDevice        kinetics,
    deviceViewType1D<real>         solidFracs,
    deviceViewType1D<real>         totalMass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia,
    deviceViewType1D<real>         gasMassSource,
    deviceViewType1D<real>         gasMassSourceSp,
    deviceViewType1D<real>         qrxn_particle
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "calcMultiReaction",
        DynamicPolicy(r.start(), r.end()),
        KOKKOS_LAMBDA(int32 i)
        {
            calcMultiReactionSingleParticle<Shrinking>
            (
                i,
                dt,
                mask,
                T_p,
                gasConcs,
                kinetics,
                solidFracs,
                totalMass,
                diameter,
                inertia,
                gasMassSource,
                gasMassSourceSp,
                qrxn_particle
            );
        }
    );

    Kokkos::fence();
}

void calcMultiReaction
(
    const pFlagTypeDevice&         mask,
    const real                     dt,
    const bool                     shrinking,
    const deviceViewType1D<real>&  T_p,
    const deviceViewType1D<real>&  gasConcs,
    const clcKineticsDevice        kinetics,
    deviceViewType1D<real>         solidFracs,
    deviceViewType1D<real>         totalMass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia,
    deviceViewType1D<real>         gasMassSource,
    deviceViewType1D<real>         gasMassSourceSp,
    deviceViewType1D<real>         qrxn_particle
)
{
    if (shrinking)
    {
        launchMultiReaction<true>
        (
            mask, dt, T_p, gasConcs, kinetics, solidFracs, totalMass,
            diameter, inertia, gasMassSource, gasMassSourceSp, qrxn_particle
        );
    }
    else
    {
        launchMultiReaction<false>
        (
            mask, dt, T_p, gasConcs, kinetics, solidFracs, totalMass,
            diameter, inertia, gasMassSource, gasMassSourceSp, qrxn_particle
        );
    }
}

real minTemperature
(
    const pFlagTypeDevice&         mask,
    const deviceViewType1D<real>&  T_p
)
{
    auto r = mask.activeRange();

    real minT = std::numeric_limits<real>::max();

    Kokkos::parallel_reduce
    (
        "minTemperature",
        StaticPolicy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i, real& localMin)
        {
            if (mask(i))
            {
                localMin = Kokkos::min(localMin, T_p[i]);
            }
        },
        Kokkos::Min<real>(minT)
    );

    Kokkos::fence();

    return minT;
}

} // multiShrinkingKernels
} // pFlow
