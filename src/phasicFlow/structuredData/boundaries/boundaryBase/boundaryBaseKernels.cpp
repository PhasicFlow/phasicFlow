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

#include "boundaryBaseKernels.hpp"

void pFlow::boundaryBaseKernels::createRemoveKeepLists
(
    uint32 numTotal, 
    uint32 numRemove, 
    const uint32Vector_D& removeMask, 
    uint32Vector_D& removeList, 
    uint32Vector_D& keepList
)
{
    uint32 numKeep = numTotal - numRemove;
    removeList.reallocate(numRemove,numRemove);
    keepList.reallocate(numKeep,numKeep);
    
    auto maskD = removeMask.deviceViewAll();
    const auto& removeD = removeList.deviceViewAll();
    const auto& keepD = keepList.deviceViewAll();
    
    exclusiveScan(
        maskD, 
        0u, 
        numTotal+1, 
        maskD, 
        0u);

    Kokkos::parallel_for
    (
        "pFlow::boundaryBaseKernels::createRemoveKeepLists",
        deviceRPolicyStatic(0, numTotal),
        LAMBDA_HD(uint32 i)
		{
			if(maskD(i)!= maskD(i+1))
				removeD(maskD(i)) = i;
            else
                keepD(i-maskD(i)) = i;
		}
    );
    Kokkos::fence();

}


void pFlow::boundaryBaseKernels::createRemoveKeepIndices
(
    const uint32Vector_D& indices,
    uint32 numRemove,
    const uint32Vector_D& removeMask, 
    uint32Vector_D& removeIndices, 
    uint32Vector_D& keepIndices,
    bool exactCap
)
{
    uint32 numTotal = indices.size();
    uint32 numKeep = numTotal - numRemove;
    if(exactCap)
    {
        removeIndices.reallocate(numRemove, numRemove);
        keepIndices.reallocate(numKeep, numKeep);
    }
    else
    {
        removeIndices.clear();
        removeIndices.resize(numRemove);
        
        keepIndices.clear();
        keepIndices.resize(numKeep);
    }
    

    auto maskD = removeMask.deviceViewAll();
    const auto& removeD = removeIndices.deviceViewAll();
    const auto& keepD = keepIndices.deviceViewAll();
    const auto& indicesD = indices.deviceViewAll();

    exclusiveScan(maskD, 0u, numTotal+1, maskD, 0u);

    Kokkos::parallel_for
    (
        "pFlow::boundaryBaseKernels::createRemoveKeepLists",
        deviceRPolicyStatic(0, numTotal),
        LAMBDA_HD(uint32 i)
		{
			if(maskD(i)!= maskD(i+1))
				removeD(maskD(i)) = indicesD(i);
            else
                keepD(i-maskD(i)) = indicesD(i);
		}
    );
    Kokkos::fence();   
}