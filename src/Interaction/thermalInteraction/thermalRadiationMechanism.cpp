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

#include "thermalRadiationMechanism.hpp"

namespace pFlow
{

using policy = Kokkos::RangePolicy<
    pFlow::DefaultExecutionSpace,
    Kokkos::Schedule<Kokkos::Dynamic>,
    Kokkos::IndexType<pFlow::uint32>>;

//----------------------------- constructors ----------------------------------

thermalRadiationMechanism::thermalRadiationMechanism(
    const dictionary&   thermoDict)
{
    radCut_ = thermoDict.getVal<real>("radCut");
    radUpdateInterval_ = thermoDict.getValMax<uint32>("radUpdateInterval", 1);
}

//---------------------------- public methods ---------------------------------

void thermalRadiationMechanism::iterate(
    const pFlagTypeDevice&          flags,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   temperature,
    const mapperNBS&                mapper,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt)
{
    bool updateThisStep = (stepCounter_ % radUpdateInterval_ == 0);
    stepCounter_++;

    if (!updateThisStep)
    {
        return;
    }

    real radCutSq = radCut_ * radCut_;

    auto r = flags.activeRange();

    auto cellIter    = mapper.getCellIterator();
    auto searchCells = mapper.getSearchCells();

    Kokkos::parallel_for(
        "thermalRadiationMechanism::iterate",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 idx_i)
        {
            if (!flags(idx_i))
            {
                return;
            }

            realx3  p_i = pos[idx_i];
            real    T_i = temperature[idx_i];
            int32x3 currentCell = searchCells.pointIndex(p_i);

            constexpr int32 ox[13] =
                {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0};
            constexpr int32 oy[13] =
                {-1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0};
            constexpr int32 oz[13] =
                {-1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1};

            // same cell
            uint32 idx_j = cellIter.start(
                currentCell.x(), currentCell.y(), currentCell.z());

            while (idx_j != mapperNBS::CellIterator::NoPos)
            {
                if (idx_i < idx_j)
                {
                    real dx = p_i.x() - pos[idx_j].x();
                    real dy = p_i.y() - pos[idx_j].y();
                    real dz = p_i.z() - pos[idx_j].z();
                    real distSq = dx*dx + dy*dy + dz*dz;

                    if (distSq <= radCutSq)
                    {
                        Kokkos::atomic_add(
                            &radSumTemp[idx_i], temperature[idx_j]);
                        Kokkos::atomic_add(&radNumPrt[idx_i], uint32(1));
                        Kokkos::atomic_add(&radSumTemp[idx_j], T_i);
                        Kokkos::atomic_add(&radNumPrt[idx_j], uint32(1));
                    }
                }

                idx_j = cellIter.next(idx_j);
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

                        if (distSq <= radCutSq)
                        {
                            Kokkos::atomic_add(
                                &radSumTemp[idx_i], temperature[idx_j]);
                            Kokkos::atomic_add(&radNumPrt[idx_i], uint32(1));
                            Kokkos::atomic_add(&radSumTemp[idx_j], T_i);
                            Kokkos::atomic_add(&radNumPrt[idx_j], uint32(1));
                        }

                        idx_j = cellIter.next(idx_j);
                    }
                }
            }
        });

    Kokkos::fence();
}

} // pFlow
