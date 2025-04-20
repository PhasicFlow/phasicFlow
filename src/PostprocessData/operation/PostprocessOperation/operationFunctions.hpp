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

#ifndef __operationFunctions_hpp__
#define __operationFunctions_hpp__

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
    const regionField<real>&        volFactor,
    const bool                      devideByVol,
    const std::vector<span<real>>&  weights,
    const span<real>&               phi,
    const includeMask::Mask&        mask
)
{
    const auto& regPoints = volFactor.regPoints(); 
    regionField<T> processedField(regFieldName, regPoints, T{});
    auto vols = regPoints.volumes();

    for(uint32 reg =0; reg<regPoints.size(); reg++)
    {
        auto partIndices = regPoints.indices(reg);
        
        auto w = weights[reg];
        T sum = T{};
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
            processedField[reg] = sum/(volFactor[reg] * vols[reg]);
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
    const regionField<real>&       volFactor,
    const bool                     devideByVol,
    const std::vector<span<real>>& weights,
    const span<real>&              phi,
    const includeMask::Mask&       mask
)
{
    
    const auto& regPoints = volFactor.regPoints(); 
    regionField<T> processedField(regFieldName, regPoints, T{});
    auto vols = regPoints.volumes();

    for(uint32 reg =0; reg<regPoints.size(); reg++)
    {
        auto partIndices = regPoints.indices(reg);
        auto w = weights[reg];
        T sumNum = T{};
        real sumDen = 0;
        uint n = 0;
        for(auto index:partIndices)
        {
            if( index!= -1)
            {
                if( mask(index))
                {
                    sumNum += w[n] * field[index]* phi[index];
                }
                sumDen += w[n] * phi[index];
            }
            
            n++;
        }
        
        if(devideByVol)
        {
            processedField[reg] = sumNum / max(sumDen, smallValue) / (volFactor[reg] * vols[reg]);
        }
        else
        {
            processedField[reg] = sumNum / max(sumDen, smallValue);
        }
        
    }

    return processedField;
}


template<typename T>
regionField<T> executeFluctuation2Operation
(
    const word&                    regFieldName,
    const span<T>&                 field,
    const regionField<T>&          fieldAvg,
    const regionField<real>&       volFactor, 
    const bool                     devideByVol, 
    const std::vector<span<real>>& weights,
    const includeMask::Mask&       mask
)
{
    const auto& regPoints = fieldAvg.regPoints();
    regionField<T> processedField(regFieldName, regPoints, T{});
    auto vols = regPoints.volumes();

    for(uint32 reg =0; reg<regPoints.size(); reg++)
    {
        auto partIndices = regPoints.indices(reg);
        auto w = weights[reg];
        auto vol = volFactor[reg] * vols[reg];
        T avField{};
        if(devideByVol)
        {
            avField = vol * fieldAvg[reg];
        }
        else
        {
            avField = fieldAvg[reg];
        }
        
        T sumNum = T{};
        real sumDen = 0;
        uint n = 0;
        for(auto index:partIndices)
        {
            if( index!= -1)
            {
                if( mask(index))
                {
                    sumNum += w[n] * pow( avField- field[index],static_cast<real>(2));
                }
                sumDen += w[n];
            }
            n++;
        }
        
        if(devideByVol)
        {
            processedField[reg] = sumNum / max(sumDen, smallValue) / vol;
        }
        else
        {
            processedField[reg] = sumNum / max(sumDen, smallValue);
        }
        
    }

    return processedField;
}

} // namespace pFlow

#endif //__operationFunctions_hpp__
