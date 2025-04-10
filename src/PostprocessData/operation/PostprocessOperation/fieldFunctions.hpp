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

#ifndef __fieldFunctions_hpp__
#define __fieldFunctions_hpp__

#include <vector>

#include "span.hpp"
#include "regionPoints.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"

namespace pFlow
{


template<typename T>
regionField<T> executeSumOperation
(
    const word&                     regFieldName,
    const span<T>&                  field, 
    const regionPoints&             regPoints,
    const bool                      devideByVol,
    const std::vector<span<real>>&  weights,
    const span<real>&               phi,
    const includeMask::Mask&        mask
)
{
    regionField<T> processedField(regFieldName, regPoints, T{});

    for(uint32 reg =0; reg<regPoints.size(); reg++)
    {
        auto partIndices = regPoints.indices(reg);
        auto vols = regPoints.volumes();
        auto w = weights[reg];
        T sum{};
        uint n = 0;
        for(auto index:partIndices)
        {
            if( index!= -1 && mask( index ))
            {
                sum += w[n] * field[index]* phi[index];
            }
            n++;
        }
        if(devideByVol)
        {
            processedField[reg] = sum/vols[reg];
        }
        else
        {
            processedField[reg] = sum;
        }
        
    }

    return processedField;
}

template<typename T>
regionField<T> executeAverageOperation
(
    const word&                    regFieldName,
    const span<T>&                 field, 
    const regionPoints&            regPoints, 
    const std::vector<span<real>>& weights,
    const span<real>&              phi,
    const includeMask::Mask&       mask
)
{
    regionField<T> processedField(regFieldName, regPoints, T{});

    for(uint32 reg =0; reg<regPoints.size(); reg++)
    {
        auto partIndices = regPoints.indices(reg);
        auto w = weights[reg];
        T sumNum{};
        real sumDen{};
        uint n = 0;
        for(auto index:partIndices)
        {
            if( index!= -1 && mask( index ))
            {
                sumNum += w[n] * field[index]* phi[index];
            }
            sumDen += w[n] * phi[index];
            n++;
        }

        sumDen = max(sumDen, smallValue);
        processedField[reg] = sumNum/sumDen;
        
    }

    return processedField;
}

} // namespace pFlow

#endif //__fieldFunctions_hpp__
