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

#include "phasicFlowKokkos.hpp"
#include "cells.hpp"
#include "pointFlag.hpp"


namespace pFlow::mapperNBSKernels
{

void findPointExtends(
    const deviceViewType1D<realx3>& points,
    const pFlagTypeDevice& flags,
    realx3& minPoint,
    realx3& maxPoint);

bool buildListsReduce(
    const cells& searchCell,
    const deviceViewType3D<uint32>& head,
    const deviceViewType1D<uint32>& next,
    const deviceViewType1D<realx3>& points,
    const pFlagTypeDevice& flags);

bool buildLists(
    const cells& searchCell,
    const deviceViewType3D<uint32>& head,
    const deviceViewType1D<uint32>& next,
    const deviceViewType1D<realx3>& points,
    const pFlagTypeDevice& flags);

}