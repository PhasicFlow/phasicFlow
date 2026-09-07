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

namespace thermalConductionPFPMechanismKernels
{

using policy = Kokkos::RangePolicy<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<pFlow::uint32>>;

template<bool CalcPP, bool CalcPFP>
void sweep(
    const pFlagTypeDevice&          flags,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    const deviceViewType1D<real>&   fluidKappa,
    const deviceViewType1D<real>&   fluidAlpha,
    const mapperNBS&                mapper,
    const real                      simYoungsModulus,
    deviceViewType1D<real>          Q_pp,
    deviceViewType1D<real>          Q_pfp)
{
    using namespace thermalConductionPFPKernels;

    auto r = flags.activeRange();

    auto cellIter    = mapper.getCellIterator();
    auto searchCells = mapper.getSearchCells();

    Kokkos::parallel_for(
        "thermalConductionPFPMechanism::sweep",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 idx_i)
        {
            if (!flags(idx_i))
            {
                return;
            }

            constexpr int32 ox[13] =
                {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0};
            constexpr int32 oy[13] =
                {-1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0};
            constexpr int32 oz[13] =
                {-1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1};

            realx3  p_i = pos[idx_i];
            real    R_i = 0.5 * diameter[idx_i];
            real    T_i = temperature[idx_i];
            int32x3 currentCell = searchCells.pointIndex(p_i);

            uint32 idx_j = mapperNBS::CellIterator::NoPos;

            // same cell -- guarded with inCellRange the same way the
            // 13 neighbour offsets below are, since a particle's own
            // cell can fall outside the grid just as easily as a
            // neighbour offset can.
            if (searchCells.inCellRange(currentCell))
            {
                idx_j = cellIter.start(
                    currentCell.x(), currentCell.y(), currentCell.z());

                while (idx_j != mapperNBS::CellIterator::NoPos)
                {
                    if (idx_i < idx_j)
                    {
                        real dx = p_i.x() - pos[idx_j].x();
                        real dy = p_i.y() - pos[idx_j].y();
                        real dz = p_i.z() - pos[idx_j].z();
                        real distSq = dx*dx + dy*dy + dz*dz;

                        real R_j = 0.5 * diameter[idx_j];
                        real sumRadiiSq = (R_i + R_j) * (R_i + R_j);

                        bool isContact = (distSq < sumRadiiSq);
                        real dist = sqrt(distSq);
                        real rc_real = 0.0;

                        if (isContact && dist > 1e-12)
                        {
                            rc_real = contactConduction(
                                R_i, R_j, dist,
                                nu[idx_i], nu[idx_j],
                                E0[idx_i], E0[idx_j],
                                simYoungsModulus,
                                K[idx_i], K[idx_j],
                                T_i, temperature[idx_j],
                                CalcPP,
                                Q_pp, idx_i, idx_j);
                        }

                        if constexpr (CalcPFP)
                        {
                            if (dist > 1e-12)
                            {
                                real r_sij = isContact ? rc_real : 0.0;

                                particleFluidParticle(
                                    R_i, R_j, dist,
                                    fluidKappa[idx_i], fluidKappa[idx_j],
                                    fluidAlpha[idx_i], fluidAlpha[idx_j],
                                    K[idx_i], K[idx_j],
                                    T_i, temperature[idx_j],
                                    r_sij,
                                    Q_pfp, idx_i, idx_j);
                            }
                        }
                    }

                    idx_j = cellIter.next(idx_j);
                }
            }

            // neighbour cells
            for (uint32 ni = 0; ni < 13; ++ni)
            {
                int32x3 neighborCell(
                    currentCell.x() + ox[ni],
                    currentCell.y() + oy[ni],
                    currentCell.z() + oz[ni]);

                if (searchCells.inCellRange(neighborCell))
                {
                    idx_j = cellIter.start(
                        neighborCell.x(),
                        neighborCell.y(),
                        neighborCell.z());

                    while (idx_j != mapperNBS::CellIterator::NoPos)
                    {
                        real dx = p_i.x() - pos[idx_j].x();
                        real dy = p_i.y() - pos[idx_j].y();
                        real dz = p_i.z() - pos[idx_j].z();
                        real distSq = dx*dx + dy*dy + dz*dz;

                        real R_j = 0.5 * diameter[idx_j];
                        real sumRadiiSq = (R_i + R_j) * (R_i + R_j);

                        bool isContact = (distSq < sumRadiiSq);
                        real dist = sqrt(distSq);
                        real rc_real = 0.0;

                        if (isContact && dist > 1e-12)
                        {
                            rc_real = contactConduction(
                                R_i, R_j, dist,
                                nu[idx_i], nu[idx_j],
                                E0[idx_i], E0[idx_j],
                                simYoungsModulus,
                                K[idx_i], K[idx_j],
                                T_i, temperature[idx_j],
                                CalcPP,
                                Q_pp, idx_i, idx_j);
                        }

                        if constexpr (CalcPFP)
                        {
                            if (dist > 1e-12)
                            {
                                real r_sij = isContact ? rc_real : 0.0;

                                particleFluidParticle(
                                    R_i, R_j, dist,
                                    fluidKappa[idx_i], fluidKappa[idx_j],
                                    fluidAlpha[idx_i], fluidAlpha[idx_j],
                                    K[idx_i], K[idx_j],
                                    T_i, temperature[idx_j],
                                    r_sij,
                                    Q_pfp, idx_i, idx_j);
                            }
                        }

                        idx_j = cellIter.next(idx_j);
                    }
                }
            }
        });

    Kokkos::fence();
}

} // thermalConductionPFPMechanismKernels

/**
 * @brief Q_pp (contact conduction) and Q_pfp (particle-fluid-
 * particle), combined because both need contactConduction()'s
 * Hertzian contact radius, even when Q_pp itself is disabled.
 * Constructed only when enablePP or enablePFP is true.
 */
class thermalConductionPFPMechanism
{
private:
    //- private members
        bool    enablePP_  = false;
        bool    enablePFP_ = false;
        real    simYoungsModulus_ = 1e7;
public:
    //- constructors
        thermalConductionPFPMechanism(
            const dictionary&   thermoDict,
            bool                enablePP,
            bool                enablePFP);
        ~thermalConductionPFPMechanism() = default;
    //- public methods
        inline
        bool ppEnabled() const
        {
            return enablePP_;
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
        real requiredSearchCut(real maxBoundingSphere) const;
        void iterate(
            const pFlagTypeDevice&          flags,
            const deviceViewType1D<realx3>& pos,
            const deviceViewType1D<real>&   diameter,
            const deviceViewType1D<real>&   temperature,
            const deviceViewType1D<real>&   K,
            const deviceViewType1D<real>&   E0,
            const deviceViewType1D<real>&   nu,
            const deviceViewType1D<real>&   fluidKappa,
            const deviceViewType1D<real>&   fluidAlpha,
            const mapperNBS&                mapper,
            deviceViewType1D<real>          Q_pp,
            deviceViewType1D<real>          Q_pfp) const;
}; // thermalConductionPFPMechanism

} // pFlow

#endif // pFlow_thermalConductionPFPMechanism_hpp
