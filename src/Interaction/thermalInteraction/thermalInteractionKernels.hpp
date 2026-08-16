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

#ifndef pFlow_thermalInteractionKernels_hpp
#define pFlow_thermalInteractionKernels_hpp

#include "types.hpp"
#include "pointFlag.hpp"
#include "mapperNBS.hpp"

namespace pFlow
{
namespace thermalInteractionKernels
{

/**
 * @brief GPU kernel computing collisional (Q_pp), Particle-Fluid-Particle
 *        (Q_pfp), and radiation heat transfer for all active particle pairs.
 *
 * @details
 * Dispatched by thermalInteraction::iterate() once per DEM sub-step.
 * The kernel performs three independent calculations in a single neighbour-
 * search sweep:
 *
 * ### 1. Static-contact conduction (Q_pp)
 * Heat conduction through solid–solid contact between particles in
 * sustained/static contact. The contact radius is computed purely from
 * geometry: rc = sqrt(R_eff * overlap), the standard Hertzian relationship
 * between the normal interpenetration of two spheres and their circular
 * contact-patch radius. No relative velocity or contact-time model is
 * involved. Because the simulation typically uses a softened Young's
 * modulus to allow a larger DEM timestep, the resulting overlap - and
 * hence rc - overstates what a real, stiffer material would produce; a
 * correction factor derived from the ratio of simulated to real Young's
 * modulus scales rc back down before it is used in the heat transfer
 * rate. Enabled when `calcCond = true`.
 *
 * ### 2. Particle-Fluid-Particle sub-grid heat transfer (Q_pfp)
 * When two particles are within a dimensionless gap H/R* <= 0.5, the fluid
 * between them forms a thin thermal bridge.  The effective conductance is:
 *
 *   Q_pfp = 2pi k_f R* \int_0^{H/R*} r^2/(h + r^2/2R*) dr
 *
 * evaluated using 5-point Gauss-Legendre quadrature (Rong & Horio, 1999).
 * Enabled when `calcPFP = true`.
 *
 * #### Sub-grid energy approximation
 * The fluid energy equation (TEqn on the CFD side) does **not** contain
 * a corresponding sink term for the energy consumed by the PFP bridge.
 * This is an intentional sub-grid approximation based on the following:
 *
 *   1. PFP operates below CFD mesh resolution in the unresolved regime.
 *   2. The heat redistributes between solid particles; the fluid acts only
 *      as a passive conduit — energy given to particle i is received from
 *      particle j and vice versa.  Net fluid energy change approx 0 at the
 *      cell scale.
 *   3. For dilute-to-moderate packing (alpha_s < 0.45), PFP is typically one
 *      order of magnitude smaller than convective heat transfer, introducing
 *      < 5 % error in the fluid energy budget.
 *
 * **Validity conditions:**
 *   - Cell volume >> particle volume (unresolved regime, V_cell/V_p > 10)
 *   - Solid conductivity >> fluid conductivity (k_s/k_f >> 1)
 *   - Solid volume fraction alpha_s < 0.45
 *
 * For dense beds (alpha_s > 0.5) or particles with low k_s/k_f, the fluid-side
 * energy sink from PFP should be evaluated and added to TEqn explicitly.
 *
 * #### Note on fluid property sampling
 * `fluidKappa` and `fluidAlpha` are sampled at the particle's Eulerian cell
 * centre (not at the bridge midpoint).  The resulting discretisation error
 * is O(d_p / Delta x), negligible for d_p << Delta x.
 *
 * ### 3. Radiation (neighbourhood-based)
 * Accumulates radiating-neighbour temperature sums and counts for the
 * linearised radiation model in sphereHeatTransfer. Radiative exchange
 * happens only between solid particles; the carrier fluid is radiatively
 * transparent and never appears in this accumulation. Enabled when
 * `calcRad = true`.
 *
 * @param calcRad   Enable radiation neighbourhood accumulation.
 * @param calcCond  Enable particle-particle static-contact conduction (Q_pp).
 * @param calcPFP   Enable particle-fluid-particle sub-grid transfer (Q_pfp).
 */
void calcThermalInteractions(
    const pFlagTypeDevice&          m,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<realx3>& tvel,
    const deviceViewType1D<realx3>& rvel,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   mass,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   Cp,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    // local fluid kappa at particle cell [W/(m.K)]
    const deviceViewType1D<real>&   fluidKappa,
    // local fluid porosity alpha at particle cell [-]
    const deviceViewType1D<real>&   fluidAlpha,
    const mapperNBS::CellIterator&  cellIter,
    const realx3&                   domainMin,
    const real&                     cellSize,
    const int32x3&                  numCells,
    const real                      radCut,
    const real                      simYoungsModulus,
    const bool                      calcRad,
    const bool                      calcCond,
    const bool                      calcPFP,
    // static-contact conduction output [W]
    deviceViewType1D<real>          Q_pp,
    // PFP sub-grid output [W]
    deviceViewType1D<real>          Q_pfp,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt);

} // thermalInteractionKernels
} // pFlow

#endif // pFlow_thermalInteractionKernels_hpp



