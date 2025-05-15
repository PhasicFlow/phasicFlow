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

#ifndef __postprocessOperationFunctions_hpp__
#define __postprocessOperationFunctions_hpp__

#include <variant>

#include "types.hpp"
#include "iOstream.hpp"
#include "regionField.hpp"

namespace pFlow::postprocessData
{

/// Type alias for processed region field types.
/// Only regionField<real>, regionField<realx3>, and regionField<realx4> are supported
/// in the postprocessOperation class.
using processedRegFieldType = std::variant
<
    regionField<real>,
    regionField<realx3>,
    regionField<realx4>
>;


template<typename T>
inline
bool writeField
(
    iOstream&             os,
    TimeValueType             t,
    const regionField<T>  field,
    uint32                threshold,
    const T&              defValue=T{}    
)
{
    const auto& regPoints = field.regPoints();
    const uint32 n = field.size();
    os<<t<<tab;
    for(uint32 i=0; i<n; i++)
    {
        auto numPar = regPoints.indices(i).size();
        if(numPar >= threshold)
        {
            if constexpr(std::is_same_v<T,realx3>)
            {
                os<<field[i].x()<<' '<<field[i].y()<<' '<<field[i].z()<<tab;
            }
            else if constexpr( std::is_same_v<T,realx4>)
            {
                os << field[i].x() << ' ' << field[i].y() << ' ' << field[i].z() << ' ' << field[i].w() << tab;
            }
            else
            {
                os<<field[i]<<tab;
            }
            
        }
        else
        {
            if constexpr(std::is_same_v<T,realx3>)
            {
                os<<defValue.x()<<' '<<defValue.y()<<' '<<defValue.z()<<tab;
            }
            else if constexpr( std::is_same_v<T,realx4>)
            {
                os << defValue.x() << ' ' << defValue.y() << ' ' << defValue.z() << ' ' << defValue.w() << tab;
            }
            else
            {
                os<<defValue<<tab;
            }
        }

    }
    os<<endl;
    return true;
}

} // namespace pFlow::postprocessData

#endif //__postprocessOperationFunctions_hpp__