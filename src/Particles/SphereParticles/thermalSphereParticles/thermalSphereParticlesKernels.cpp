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

#include "thermalSphereParticlesKernels.hpp"

namespace pFlow 
{
namespace thermalSphereParticlesKernels
{

// Schedule<Static> is used for optimal load balancing on uniform particle
// arrays
using policy = Kokkos::RangePolicy<
    pFlow::DefaultExecutionSpace, 
    Kokkos::Schedule<Kokkos::Static>, 
    Kokkos::IndexType<pFlow::uint32>>;

void initThermalProperties(
    const pFlagTypeDevice&          m, 
    const deviceViewType1D<uint32>& idx, 
    const deviceViewType1D<real>&   sCp, 
    const deviceViewType1D<real>&   sK, 
    const deviceViewType1D<real>&   sEps,
    const deviceViewType1D<real>&   sE0,
    const deviceViewType1D<real>&   sNu,
    deviceViewType1D<real>          Cp, 
    deviceViewType1D<real>          K, 
    deviceViewType1D<real>          emissivity, 
    deviceViewType1D<real>          E0,
    deviceViewType1D<real>          nu)
{ 
    auto r = m.activeRange(); 
    
    Kokkos::parallel_for(
        "initThermalProps", 
        policy(r.start(), r.end()), 
        KOKKOS_LAMBDA(uint32 i)
        { 
            if (m(i))
            { 
                // Map the particle to its material type
                uint32 j = idx[i]; 
                
                // Scatter properties to the main particle arrays
                Cp[i]         = sCp[j]; 
                K[i]          = sK[j]; 
                emissivity[i] = sEps[j];
                E0[i]         = sE0[j];
                nu[i]         = sNu[j];
            } 
        }); 
    
    Kokkos::fence(); 
}

void calcFluidParticleHeatTransfer(
    const pFlagTypeDevice&          m, 
    const deviceViewType1D<real>&   d, 
    const deviceViewType1D<real>&   ms, 
    const deviceViewType1D<real>&   Cp, 
    const deviceViewType1D<real>&   T, 
    const deviceViewType1D<real>&   Q_pp,
    const deviceViewType1D<real>&   Q_pfp,
    deviceViewType1D<real>          TR)
{ 
    auto r = m.activeRange(); 
    
    Kokkos::parallel_for(
        "calcHeatTransferRate", 
        policy(r.start(), r.end()), 
        KOKKOS_LAMBDA(uint32 i)
        { 
            if (m(i))
            { 
                // --------------------------------------------------------- //
                // Particle Energy Equation (Lumped Capacitance Model)
                // Standalone tier: no CFD mesh, so no Q_conv/Q_rad term.
                // m * Cp * dT/dt = Q_pp + Q_pfp
                // --------------------------------------------------------- //
                
                // Thermal Inertia = Mass [kg] * Specific Heat Capacity 
                // [J/(kg.K)] = [J/K]
                // Protection against division by zero (Thermal Inertia Guard)
                real thermalInertia = max(ms[i] * Cp[i], real(1e-12));
                
                TR[i] = (Q_pp[i] + Q_pfp[i]) / thermalInertia;
            } 
        }); 
    
    Kokkos::fence(); 
}

void calcFluidParticleHeatTransfer(
    const pFlagTypeDevice&          m, 
    const deviceViewType1D<real>&   d, 
    const deviceViewType1D<real>&   ms, 
    const deviceViewType1D<real>&   Cp, 
    const deviceViewType1D<real>&   T, 
    const deviceViewType1D<real>&   Q_conv, 
    const deviceViewType1D<real>&   Q_rad,
    const deviceViewType1D<real>&   Q_pp,
    const deviceViewType1D<real>&   Q_pfp,
    deviceViewType1D<real>          TR)
{ 
    auto r = m.activeRange(); 
    
    Kokkos::parallel_for(
        "calcHeatTransferRateCoupled", 
        policy(r.start(), r.end()), 
        KOKKOS_LAMBDA(uint32 i)
        { 
            if (m(i))
            { 
                // --------------------------------------------------------- //
                // Particle Energy Equation (Lumped Capacitance Model)
                // Fluid-coupled tier.
                // m * Cp * dT/dt = Q_conv + Q_rad + Q_pp + Q_pfp
                // --------------------------------------------------------- //
                
                // Thermal Inertia = Mass [kg] * Specific Heat Capacity 
                // [J/(kg.K)] = [J/K]
                // Protection against division by zero (Thermal Inertia Guard)
                real thermalInertia = max(ms[i] * Cp[i], real(1e-12));
                
                TR[i] = (Q_conv[i] + Q_rad[i] + Q_pp[i] + Q_pfp[i]) / 
                    thermalInertia;
            } 
        }); 
    
    Kokkos::fence(); 
}

void integrateTemperature(
    const pFlagTypeDevice&          m, 
    const real                      dt, 
    deviceViewType1D<real>          T, 
    const deviceViewType1D<real>&   TR)
{ 
    auto r = m.activeRange(); 
    
    Kokkos::parallel_for(
        "integrateTemp", 
        policy(r.start(), r.end()), 
        KOKKOS_LAMBDA(uint32 i)
        { 
            if (m(i))
            { 
                // --------------------------------------------------------- //
                // Explicit Euler Integration for particle temperature.
                // Stability is naturally enhanced because Q_rad is 
                // analytically linearized in the CFD solver, preventing 
                // the stiff T^4 equation from causing numerical explosions.
                // --------------------------------------------------------- //
                
                T[i] += TR[i] * dt; 
            } 
        }); 
    
    Kokkos::fence(); 
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // thermalSphereParticlesKernels
} // pFlow
