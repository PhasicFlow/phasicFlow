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

#ifndef __interactionTypes_hpp__
#define __interactionTypes_hpp__



#include "types.hpp"


namespace pFlow
{


// always use signed types
using CELL_INDEX_TYPE 	= int32;

using ID_TYPE 			= int32;

//constexpr int32 minCellIndex = largestNegative<CELL_INDEX_TYPE>();

//constexpr int32 maxCellIndex = largestPositive<CELL_INDEX_TYPE>();

}


#endif //__interactionTypes_hpp__
