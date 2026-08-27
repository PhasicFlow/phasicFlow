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

#ifndef pFlow_thermalConductionPFPMechanism_hpp
#define pFlow_thermalConductionPFPMechanism_hpp

#include "types.hpp"
#include "dictionary.hpp"
#include "mapperNBS.hpp"
#include "thermalConductionPFPKernels.hpp"

namespace pFlow
{

// Own full 27-cell traversal, independent of thermalRadiationMechanism's
// (SOC: no shared per-pair dispatch function between the two mechanisms
// -- see thermalInteraction.cpp for how the two are coordinated at the
// dispatcher level instead). Templated on 2 bools, not 3: radiation is
// not this mechanism's concern.
namespace thermalConductionPFPMechanismKernels
{

using policy = Kokkos::RangePolicy<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<pFlow::uint32>>;

template<bool CalcCond, bool CalcPFP>
void sweep(
    const pFlagTypeDevice&          m,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    const deviceViewType1D<real>&   fluidKappa,
    const deviceViewType1D<real>&   fluidAlpha,
    const mapperNBS::CellIterator&  cellIter,
    const realx3&                   domainMin,
    const real&                     cellSize,
    const int32x3&                  numCells,
    const real                      simYoungsModulus,
    deviceViewType1D<real>          Q_pp,
    deviceViewType1D<real>          Q_pfp)
{
    using namespace thermalConductionPFPKernels;

    auto r = m.activeRange();

    Kokkos::parallel_for(
        "thermalConductionPFPMechanism::sweep",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (m(i))
            {
                realx3 p_i = pos[i];
                real R_i   = 0.5 * diameter[i];
                real T_i   = temperature[i];

                int32 c_x = static_cast<int32>(
                    (p_i.x() - domainMin.x()) / cellSize);
                int32 c_y = static_cast<int32>(
                    (p_i.y() - domainMin.y()) / cellSize);
                int32 c_z = static_cast<int32>(
                    (p_i.z() - domainMin.z()) / cellSize);

                for (int32 cx = c_x - 1; cx <= c_x + 1; ++cx)
                {
                    for (int32 cy = c_y - 1; cy <= c_y + 1; ++cy)
                    {
                        for (int32 cz = c_z - 1; cz <= c_z + 1; ++cz)
                        {
                            if (cx >= 0 && cx < numCells.x() &&
                                cy >= 0 && cy < numCells.y() &&
                                cz >= 0 && cz < numCells.z())
                            {
                                uint32 j = cellIter.start(cx, cy, cz);

                                while (j != mapperNBS::CellIterator::NoPos)
                                {
                                    // i < j implies i != j -- symmetric
                                    // pair, computed once, applied to
                                    // both particles atomically inside
                                    // contactConduction/
                                    // particleFluidParticle.
                                    if (m(j) && i < j)
                                    {
                                        real dx = p_i.x() - pos[j].x();
                                        real dy = p_i.y() - pos[j].y();
                                        real dz = p_i.z() - pos[j].z();

                                        real distSq =
                                            dx*dx + dy*dy + dz*dz;

                                        real R_j = 0.5*diameter[j];
                                        real sumRadiiSq =
                                            (R_i+R_j)*(R_i+R_j);

                                        bool isContact =
                                            (distSq < sumRadiiSq);
                                        real dist = sqrt(distSq);
                                        real rc_real = 0.0;

                                        // Runs whenever in contact
                                        // even if CalcCond is false --
                                        // PFP needs rc_real as r_sij;
                                        // the Q_pp write is gated by
                                        // CalcCond inside.
                                        if (isContact &&
                                            dist > 1e-12)
                                        {
                                            rc_real = contactConduction(
                                                R_i, R_j, dist,
                                                nu[i], nu[j],
                                                E0[i], E0[j],
                                                simYoungsModulus,
                                                K[i], K[j],
                                                T_i, temperature[j],
                                                CalcCond,
                                                Q_pp, i, j);
                                        }

                                        if constexpr (CalcPFP)
                                        {
                                            if (dist > 1e-12)
                                            {
                                                real r_sij = isContact
                                                    ? rc_real
                                                    : 0.0;

                                                particleFluidParticle(
                                                    R_i, R_j, dist,
                                                    fluidKappa[i],
                                                    fluidKappa[j],
                                                    fluidAlpha[i],
                                                    fluidAlpha[j],
                                                    K[i], K[j],
                                                    T_i, temperature[j],
                                                    r_sij,
                                                    Q_pfp, i, j);
                                            }
                                        }
                                    }
                                    j = cellIter.next(j);
                                }
                            }
                        }
                    }
                }
            }
        });

    Kokkos::fence();
}

} // thermalConductionPFPMechanismKernels

/**
 * @brief Owns the parameters shared by static-contact conduction
 * (Q_pp) and particle-fluid-particle transfer (Q_pfp) -- combined
 * because both need the Hertzian contact radius from
 * thermalConductionPFPKernels::contactConduction(), even when Q_pp
 * itself is disabled.
 *
 * Constructed only when enableConduction or enablePFP is true.
 * Owns no per-particle memory: Q_pp/Q_pfp outputs live on
 * thermalSphereParticles, the class that consumes them.
 *
 * Self-contained: iterate() runs its own full 27-cell sweep,
 * independent of thermalRadiationMechanism's. When both mechanisms
 * are active, the traversal is computed twice (once here, once
 * there) -- an accepted trade-off for keeping each mechanism free
 * of any dependency on the other.
 */
class thermalConductionPFPMechanism
{
private:

    //- private members

        bool    enableConduction_ = false;

        bool    enablePFP_        = false;

        real    simYoungsModulus_ = 1e7;

public:

    //- constructors

        /// @brief Reads simYoungsModulus from thermoDict -- mandatory
        /// for the Hertzian contact-radius correction.
        thermalConductionPFPMechanism(
            const dictionary&   thermoDict,
            bool                enableConduction,
            bool                enablePFP);

        ~thermalConductionPFPMechanism() = default;

    //- public methods

        inline
        bool conductionEnabled() const
        {
            return enableConduction_;
        }

        inline
        bool pfpEnabled() const
        {
            return enablePFP_;
        }

        inline
        real simYoungsModulus() const
        {
            return simYoungsModulus_;
        }

        /// @brief Search radius needed: 2x the largest contact
        /// distance for conduction, 3x for PFP's wider reach.
        real requiredSearchCut(real maxBoundingSphere) const;

        /// @brief Dispatches to one of 4 compiled sweep variants
        /// (conductionEnabled_ x pfpEnabled_), so the disabled
        /// mechanism's branch is compiled out of the per-pair loop
        /// entirely.
        void iterate(
            const pFlagTypeDevice&          m,
            const deviceViewType1D<realx3>& pos,
            const deviceViewType1D<real>&   diameter,
            const deviceViewType1D<real>&   temperature,
            const deviceViewType1D<real>&   K,
            const deviceViewType1D<real>&   E0,
            const deviceViewType1D<real>&   nu,
            const deviceViewType1D<real>&   fluidKappa,
            const deviceViewType1D<real>&   fluidAlpha,
            const mapperNBS::CellIterator&  cellIter,
            const realx3&                   domainMin,
            const real&                     cellSize,
            const int32x3&                  numCells,
            deviceViewType1D<real>          Q_pp,
            deviceViewType1D<real>          Q_pfp) const;

}; // thermalConductionPFPMechanism

} // pFlow

#endif // pFlow_thermalConductionPFPMechanism_hpp
