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

#include "multiReactiveSphereParticlesKernels.hpp"

namespace pFlow
{
namespace multiReactiveSphereParticlesKernels
{

// Static scheduling: every kernel here does the same work for each particle
using policy = Kokkos::RangePolicy
<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Static>,
    Kokkos::IndexType<pFlow::uint32>
>;

/// Restores the particle state; Shrinking selects the diameter update.
template<bool Shrinking>
void restoreSolidStateKernel
(
    const pFlagTypeDevice&         mask,
    const clcKineticsDevice        kinetics,
    const deviceViewType1D<real>&  solidFracs,
    const deviceViewType1D<real>&  solidMass,
    deviceViewType1D<real>         mass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "restoreSolidState",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i) && solidMass[i] > real(0))
            {
                const real m = solidMass[i];
                mass[i] = m;

                if constexpr (Shrinking)
                {
                    real vol = real(0);

                    for (int32 s = 0; s < kinetics.N_solid; ++s)
                    {
                        vol += solidFracs[i * kinetics.N_solid + s]
                             / kinetics.solidDensities[s];
                    }

                    diameter[i] = Kokkos::cbrt
                    (
                        real(6) * m * vol / Kokkos::numbers::pi_v<real>
                    );
                }

                const real radius = real(0.5) * diameter[i];
                inertia[i] = real(0.4) * m * radius * radius;
            }
        }
    );

    Kokkos::fence();
}

void loadSolidFraction
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const uint32                   species,
    const deviceViewType1D<real>&  field,
    deviceViewType1D<real>         solidFracs
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "loadSolidFraction",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i))
            {
                solidFracs[i * N_solid + species] = field[i];
            }
        }
    );

    Kokkos::fence();
}

void storeSolidFraction
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const uint32                   species,
    const deviceViewType1D<real>&  solidFracs,
    deviceViewType1D<real>         field
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "storeSolidFraction",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i))
            {
                field[i] = solidFracs[i * N_solid + species];
            }
        }
    );

    Kokkos::fence();
}

void storeSolidMass
(
    const pFlagTypeDevice&         mask,
    const deviceViewType1D<real>&  mass,
    deviceViewType1D<real>         solidMass
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "storeSolidMass",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i))
            {
                solidMass[i] = mass[i];
            }
        }
    );

    Kokkos::fence();
}

real maxSolidFractionError
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const deviceViewType1D<real>&  solidFracs
)
{
    auto r = mask.activeRange();

    real maxError = real(0);

    Kokkos::parallel_reduce
    (
        "maxSolidFractionError",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i, real& localMax)
        {
            if (mask(i))
            {
                real sum = real(0);

                for (uint32 s = 0; s < N_solid; ++s)
                {
                    sum += solidFracs[i * N_solid + s];
                }

                localMax = Kokkos::max(localMax, Kokkos::abs(sum - real(1)));
            }
        },
        Kokkos::Max<real>(maxError)
    );

    Kokkos::fence();

    return maxError;
}

void normalizeSolidFractions
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    deviceViewType1D<real>         solidFracs
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "normalizeSolidFractions",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i))
            {
                real sum = real(0);

                for (uint32 s = 0; s < N_solid; ++s)
                {
                    sum += solidFracs[i * N_solid + s];
                }

                sum = Kokkos::max(sum, real(1e-12));

                for (uint32 s = 0; s < N_solid; ++s)
                {
                    solidFracs[i * N_solid + s] /= sum;
                }
            }
        }
    );

    Kokkos::fence();
}

void restoreSolidState
(
    const pFlagTypeDevice&         mask,
    const bool                     shrinking,
    const clcKineticsDevice        kinetics,
    const deviceViewType1D<real>&  solidFracs,
    const deviceViewType1D<real>&  solidMass,
    deviceViewType1D<real>         mass,
    deviceViewType1D<real>         diameter,
    deviceViewType1D<real>         inertia
)
{
    if (shrinking)
    {
        restoreSolidStateKernel<true>
        (
            mask, kinetics, solidFracs, solidMass, mass, diameter, inertia
        );
    }
    else
    {
        restoreSolidStateKernel<false>
        (
            mask, kinetics, solidFracs, solidMass, mass, diameter, inertia
        );
    }
}

real maxVolumeMismatch
(
    const pFlagTypeDevice&         mask,
    const clcKineticsDevice        kinetics,
    const deviceViewType1D<real>&  solidFracs,
    const deviceViewType1D<real>&  mass,
    const deviceViewType1D<real>&  diameter
)
{
    auto r = mask.activeRange();

    real maxMismatch = real(0);

    Kokkos::parallel_reduce
    (
        "maxVolumeMismatch",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i, real& localMax)
        {
            if (mask(i))
            {
                real volSolid = real(0);

                for (int32 s = 0; s < kinetics.N_solid; ++s)
                {
                    volSolid += solidFracs[i * kinetics.N_solid + s]
                              / kinetics.solidDensities[s];
                }

                volSolid *= mass[i];

                const real d = diameter[i];
                const real volSphere =
                    Kokkos::numbers::pi_v<real> * d * d * d / real(6);

                localMax = Kokkos::max
                (
                    localMax,
                    Kokkos::abs(volSphere - volSolid)
                  / Kokkos::max(volSphere, real(1e-30))
                );
            }
        },
        Kokkos::Max<real>(maxMismatch)
    );

    Kokkos::fence();

    return maxMismatch;
}

void integrateReactionHeat
(
    const pFlagTypeDevice&         mask,
    const real                     dt,
    const uint32                   N_gas,
    const deviceViewType1D<real>&  gasMassSource,
    const deviceViewType1D<real>&  reactionHeat,
    const deviceViewType1D<real>&  mass,
    const deviceViewType1D<real>&  Cp,
    deviceViewType1D<real>         temperature
)
{
    auto r = mask.activeRange();

    Kokkos::parallel_for
    (
        "integrateReactionHeat",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (mask(i))
            {
                // Net gas mass released in this step: the particle mass
                // before it is the present mass plus that gas mass.
                real gasMass = real(0);

                for (uint32 g = 0; g < N_gas; ++g)
                {
                    gasMass += gasMassSource[i * N_gas + g] * dt;
                }

                // Thermal inertia m * Cp [J/K], protected against division
                // by zero
                const real thermalInertia =
                    Kokkos::max((mass[i] + gasMass) * Cp[i], real(1e-12));

                temperature[i] += reactionHeat[i] / thermalInertia * dt;
            }
        }
    );

    Kokkos::fence();
}

} // multiReactiveSphereParticlesKernels
} // pFlow
