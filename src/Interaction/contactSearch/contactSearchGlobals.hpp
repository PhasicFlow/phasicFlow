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

#include "types.hpp"
#include "unsortedPairs.hpp"

#ifndef __contactSearchGlobals_hpp__
#define __contactSearchGlobals_hpp__

namespace pFlow
{

using csExecutionSpace = DefaultExecutionSpace;

using csIdType = uint32;

using csPairContainerType   = unsortedPairs<DefaultExecutionSpace, uint32>;

inline 
const uint32 BASE_MIRROR_WALL_INDEX = 1000000;

}

#endif
