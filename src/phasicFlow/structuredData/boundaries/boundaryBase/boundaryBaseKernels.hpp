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

#ifndef __boundaryBaseKernels_hpp__
#define __boundaryBaseKernels_hpp__

#include "phasicFlowKokkos.hpp"

namespace pFlow::boundaryBaseKernels
{

void createRemoveKeepLists(
    uint32 numTotal, 
    uint32 numRemove, 
    deviceViewType1D<uint32> removeMask, 
    deviceViewType1D<uint32>& removeList, 
    deviceViewType1D<uint32>& keepList);

void createRemoveKeepIndices(
    deviceViewType1D<uint32> indices,
    uint32 numRemove,
    deviceViewType1D<uint32> removeMask, 
    deviceViewType1D<uint32>& removeIndices, 
    deviceViewType1D<uint32>& keepIndices);


}

#endif //__boundaryBaseKernels_hpp__