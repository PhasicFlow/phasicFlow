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

#ifndef pFlow_thermalSphereParticlesKernels_hpp
#define pFlow_thermalSphereParticlesKernels_hpp

#include "types.hpp"
#include "pointFlag.hpp"

namespace pFlow
{

/**
 * @namespace pFlow::thermalSphereParticlesKernels
 * @brief High-performance GPU/CPU kernels for Lagrangian particle 
 *        thermodynamics.
 * 
 * @details
 * This namespace isolates the raw numerical integration and physical 
 * calculations for particle heat transfer from the memory management classes. 
 * By using Kokkos `deviceViewType1D`, these functions map directly to 
 * massively parallel execution spaces (like NVIDIA CUDA or AMD HIP), ensuring 
 * that thermodynamic updates for millions of particles occur efficiently.
 */
namespace thermalSphereParticlesKernels 
{

    /**
     * @brief Maps macroscopic shape properties to individual particle arrays.
     * 
     * @param mask       Active particle flag.
     * @param shapeIndex The index linking a particle to its material type.
     * @param Cp         [OUT] Specific heat capacity array of particles.
     * @param K          [OUT] Thermal conductivity array of particles.
     * @param emissivity [OUT] Surface emissivity array of particles.
     * @param E0         [OUT] Real Young's Modulus array of particles.
     * @param nu         [OUT] Poisson's Ratio array of particles.
     * @param shapeCp    Dictionary-loaded heat capacities per material type.
     * @param shapeK     Dictionary-loaded conductivities per material type.
     * @param shapeEps   Dictionary-loaded emissivities per material type.
     * @param shapeE0    Dictionary-loaded Real Young's Moduli per material.
     * @param shapeNu    Dictionary-loaded Poisson's Ratios per material type.
     */
    void initThermalProperties(
        const pFlagTypeDevice&          mask,
        const deviceViewType1D<uint32>& shapeIndex,
        deviceViewType1D<real>          Cp,
        deviceViewType1D<real>          K,
        deviceViewType1D<real>          emissivity,
        deviceViewType1D<real>          E0,
        deviceViewType1D<real>          nu,
        const deviceViewType1D<real>&   shapeCp,
        const deviceViewType1D<real>&   shapeK,
        const deviceViewType1D<real>&   shapeEps,
        const deviceViewType1D<real>&   shapeE0,
        const deviceViewType1D<real>&   shapeNu);
    
    /**
     * @brief Evaluates the First Law of Thermodynamics for each particle.
     * 
     * @details 
     * Uses the Lumped Capacitance Model to calculate the temporal 
     * temperature derivative: 
     * dT/dt = (Q_conv + Q_rad + Q_pp + Q_pfp) / (m * Cp)
     * 
     * @param mask            Active particle flag.
     * @param diameter        Particle diameter array.
     * @param mass            Particle mass array.
     * @param Cp              Particle specific heat capacity array.
     * @param temperature     Current particle temperature array.
     * @param Q_conv          Convective heat source array [W].
     * @param Q_rad           Radiative heat source array [W].
     * @param Q_pp            Collisional heat transfer array [W].
     * @param Q_pfp           Particle-Fluid-Particle sub-grid heat [W].
     * @param temperatureRate [OUT] The resulting rate of temperature change.
     */
    void calcFluidParticleHeatTransfer(
        const pFlagTypeDevice&          mask,
        const deviceViewType1D<real>&   diameter,
        const deviceViewType1D<real>&   mass,
        const deviceViewType1D<real>&   Cp,
        const deviceViewType1D<real>&   temperature,
        const deviceViewType1D<real>&   Q_conv,
        const deviceViewType1D<real>&   Q_rad,
        const deviceViewType1D<real>&   Q_pp,
        const deviceViewType1D<real>&   Q_pfp,
        deviceViewType1D<real>          temperatureRate);
    
    /**
     * @brief Marches the particle temperatures forward in time.
     * 
     * @details 
     * Applies Explicit Euler integration: 
     * T(t+dt) = T(t) + (dT/dt) * dt
     * 
     * @param mask            Active particle flag.
     * @param dt              The physical time step size [s].
     * @param temperature     [IN/OUT] Particle temperature array to be updated.
     * @param temperatureRate Computed rate of temperature change (dT/dt).
     */
    void integrateTemperature(
        const pFlagTypeDevice&          mask,
        const real                      dt,
        deviceViewType1D<real>          temperature,
        const deviceViewType1D<real>&   temperatureRate);

} // thermalSphereParticlesKernels
} // pFlow

#endif // pFlow_thermalSphereParticlesKernels_hpp



