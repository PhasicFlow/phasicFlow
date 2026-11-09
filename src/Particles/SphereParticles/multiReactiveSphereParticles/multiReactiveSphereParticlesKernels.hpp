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

#ifndef pFlow_multiReactiveSphereParticlesKernels_hpp
#define pFlow_multiReactiveSphereParticlesKernels_hpp

#include "types.hpp"
#include "pointFlag.hpp"
#include "multiReactionTypes.hpp"

namespace pFlow
{
namespace multiReactiveSphereParticlesKernels
{

/**
 * @brief Copies the solid mass fraction field of one species into its
 * column of the flat solid mass fraction array.
 *
 * @param mask       Active particle flag.
 * @param N_solid    Number of solid species.
 * @param species    Index of the solid species.
 * @param field      Solid mass fraction field of the species.
 * @param solidFracs [OUT] Flat solid mass fractions [N_p x N_solid].
 */
void loadSolidFraction
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const uint32                   species,
    const deviceViewType1D<real>&  field,
    deviceViewType1D<real>         solidFracs
);

/**
 * @brief Copies the column of one species of the flat solid mass fraction
 * array into its field.
 *
 * @param mask       Active particle flag.
 * @param N_solid    Number of solid species.
 * @param species    Index of the solid species.
 * @param solidFracs Flat solid mass fractions [N_p x N_solid].
 * @param field      [OUT] Solid mass fraction field of the species.
 */
void storeSolidFraction
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const uint32                   species,
    const deviceViewType1D<real>&  solidFracs,
    deviceViewType1D<real>         field
);

/**
 * @brief Copies the particle mass into the solid mass field.
 *
 * @param mask      Active particle flag.
 * @param mass      Particle mass [kg].
 * @param solidMass [OUT] Total solid mass of the particles [kg].
 */
void storeSolidMass
(
    const pFlagTypeDevice&         mask,
    const deviceViewType1D<real>&  mass,
    deviceViewType1D<real>         solidMass
);

/**
 * @brief Largest deviation of the solid mass fractions of a particle from
 * a sum of one, over the active particles.
 *
 * @param mask       Active particle flag.
 * @param N_solid    Number of solid species.
 * @param solidFracs Flat solid mass fractions [N_p x N_solid].
 */
real maxSolidFractionError
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    const deviceViewType1D<real>&  solidFracs
);

/**
 * @brief Scales the solid mass fractions of each particle to sum to one.
 *
 * @param mask       Active particle flag.
 * @param N_solid    Number of solid species.
 * @param solidFracs [IN/OUT] Flat solid mass fractions [N_p x N_solid].
 */
void normalizeSolidFractions
(
    const pFlagTypeDevice&         mask,
    const uint32                   N_solid,
    deviceViewType1D<real>         solidFracs
);

/**
 * @brief Restores the evolved state of the particles that have a stored
 * solid mass: mass, diameter and moment of inertia.
 *
 * The diameter follows the volume of the solid composition when shrinking
 * is on and keeps its value otherwise. Particles whose stored solid mass is
 * zero keep their nominal state.
 *
 * @param mask       Active particle flag.
 * @param shrinking  Shrinking Particle Model switch.
 * @param kinetics   Kinetics data (solid densities).
 * @param solidFracs Flat solid mass fractions [N_p x N_solid].
 * @param solidMass  Stored total solid mass of the particles [kg].
 * @param mass       [OUT] Particle mass [kg].
 * @param diameter   [IN/OUT] Particle diameter [m].
 * @param inertia    [OUT] Moment of inertia [kg m2].
 */
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
);

/**
 * @brief Largest relative difference between the volume of a particle
 * sphere and the volume of its solid composition, over the active
 * particles.
 *
 * @param mask       Active particle flag.
 * @param kinetics   Kinetics data (solid densities).
 * @param solidFracs Flat solid mass fractions [N_p x N_solid].
 * @param mass       Particle mass [kg].
 * @param diameter   Particle diameter [m].
 */
real maxVolumeMismatch
(
    const pFlagTypeDevice&         mask,
    const clcKineticsDevice        kinetics,
    const deviceViewType1D<real>&  solidFracs,
    const deviceViewType1D<real>&  mass,
    const deviceViewType1D<real>&  diameter
);

/**
 * @brief Raises the particle temperature by the reaction heat released in
 * the time step: dT = Q_rxn * dt / (m * Cp), with the particle mass before
 * the gas it lost in this step.
 *
 * @param mask          Active particle flag.
 * @param dt            Time step [s].
 * @param N_gas         Number of gas species.
 * @param gasMassSource Gas mass source [kg/s], flat [N_p x N_gas].
 * @param reactionHeat  Reaction heat of the particles [W].
 * @param mass          Particle mass [kg].
 * @param Cp            Particle specific heat capacity [J/(kg K)].
 * @param temperature   [IN/OUT] Particle temperature [K].
 */
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
);

} // multiReactiveSphereParticlesKernels
} // pFlow

#endif // pFlow_multiReactiveSphereParticlesKernels_hpp
