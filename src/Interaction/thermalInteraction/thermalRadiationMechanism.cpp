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
#include "thermalRadiationKernels.hpp"

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
    if (!thermoDict.containsDataEntry("radCut"))
    {
        fatalErrorInFunction 
            << "Missing MANDATORY entry 'radCut' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    radCut_ = thermoDict.getVal<real>("radCut");

    if (!thermoDict.containsDataEntry("radUpdateInterval"))
    {
        fatalErrorInFunction
            << "Missing MANDATORY entry 'radUpdateInterval' "
            << "in thermoPhysicalInteraction dictionary." << endl;
        fatalExit;
    }

    radUpdateInterval_ = thermoDict.getVal<uint32>("radUpdateInterval");

    if (radUpdateInterval_ == 0)
    {
        fatalErrorInFunction
            << "'radUpdateInterval' must be >= 1." << endl;
        fatalExit;
    }
}

//---------------------------- public methods ---------------------------------

void thermalRadiationMechanism::iterate(
    const pFlagTypeDevice&          m,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   temperature,
    const mapperNBS::CellIterator&  cellIter,
    const realx3&                   domainMin,
    const real&                     cellSize,
    const int32x3&                  numCells,
    deviceViewType1D<real>          radSumTemp,
    deviceViewType1D<uint32>        radNumPrt)
{
    bool updateThisStep = (stepCounter_ % radUpdateInterval_ == 0);
    stepCounter_++;

    // Not an update step: radSumTemp/radNumPrt simply keep their
    // last value -- the "history" the interval preserves.
    if (!updateThisStep)
    {
        return;
    }

    using namespace thermalRadiationKernels;

    real radCutSq = radCut_ * radCut_;

    auto r = m.activeRange();

    Kokkos::parallel_for(
        "thermalRadiationMechanism::iterate",
        policy(r.start(), r.end()),
        KOKKOS_LAMBDA(uint32 i)
        {
            if (m(i))
            {
                realx3 p_i = pos[i];
                real sumT = 0.0;
                uint32 count = 0;

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
                                    if (i != j && m(j))
                                    {
                                        real dx = p_i.x() - pos[j].x();
                                        real dy = p_i.y() - pos[j].y();
                                        real dz = p_i.z() - pos[j].z();

                                        real distSq =
                                            dx*dx + dy*dy + dz*dz;

                                        if (distSq <= radCutSq)
                                        {
                                            accumulateNeighborTemperature(
                                                temperature[j],
                                                sumT,
                                                count);
                                        }
                                    }
                                    j = cellIter.next(j);
                                }
                            }
                        }
                    }
                }

                radSumTemp[i] = sumT;
                radNumPrt[i]  = count;
            }
        });

    Kokkos::fence();
}

} // pFlow
