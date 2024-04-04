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

#include "internalPointsKernels.hpp"

void pFlow::internalPointsKernels::changePosition
(
    deviceViewType1D<realx3> points,
    deviceViewType1D<uint32> indices, 
    realx3 transferVector
)
{
    auto s = static_cast<uint32>(indices.size());
    if(s==0u)return;

    Kokkos::parallel_for
    (
        "internalPointsKernels::changePosition",
        deviceRPolicyStatic(0u, s),
        LAMBDA_HD(uint32 i)
        {   
            points[indices[i]] += transferVector;
        }
    );
    Kokkos::fence();
    
}