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
    deviceViewType1D<uint32> removeMask, 
    deviceViewType1D<uint32>& removeList, 
    deviceViewType1D<uint32>& keepList
)
{
    uint32 numKeep = numTotal - numRemove;
    deviceViewType1D<uint32> rList("rList",numRemove);
    deviceViewType1D<uint32> kList("kList",numKeep);

    exclusiveScan(removeMask, 0u, numTotal+1, removeMask, 0u);

    Kokkos::parallel_for
    (
        "pFlow::boundaryBaseKernels::createRemoveKeepLists",
        deviceRPolicyStatic(0, numTotal),
        LAMBDA_HD(uint32 i)
		{
			if(removeMask(i)!= removeMask(i+1))
				rList(removeMask(i)) = i;
            else
                kList(i-removeMask(i)) = i;
		}
    );
    Kokkos::fence();

    removeList = rList;
    keepList = kList;

}


void pFlow::boundaryBaseKernels::createRemoveKeepIndices
(
    deviceViewType1D<uint32> indices,
    uint32 numRemove,
    deviceViewType1D<uint32> removeMask, 
    deviceViewType1D<uint32>& removeIndices, 
    deviceViewType1D<uint32>& keepIndices
)
{
    uint32 numTotal = indices.size();
    uint32 numKeep = numTotal - numRemove;
    deviceViewType1D<uint32> rIndices("rIndices",numRemove);
    deviceViewType1D<uint32> kIndices("kIndices",numKeep);

    exclusiveScan(removeMask, 0u, numTotal+1, removeMask, 0u);

    Kokkos::parallel_for
    (
        "pFlow::boundaryBaseKernels::createRemoveKeepLists",
        deviceRPolicyStatic(0, numTotal),
        LAMBDA_HD(uint32 i)
		{
			if(removeMask(i)!= removeMask(i+1))
				rIndices(removeMask(i)) = indices(i);
            else
                kIndices(i-removeMask(i)) = indices(i);
		}
    );
    Kokkos::fence();

    removeIndices = rIndices;
    keepIndices = kIndices;
}