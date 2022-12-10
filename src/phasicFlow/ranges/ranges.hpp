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

#ifndef __ranges_hpp__ 
#define __ranges_hpp__

#include "stridedRange.hpp"
#include "intervalRange.hpp"
#include "combinedRange.hpp"

namespace pFlow
{

using int32StridedRagne = stridedRange<int32>;
using int64StridedRange = stridedRange<int64>;
using realStridedRange  = stridedRange<real>;

using int32IntervalRange = intervalRange<int32>;
using int64IntervalRange = intervalRange<int64>;
using realIntervalRange	 = intervalRange<real>;

using int32CombinedRange = combinedRange<int32>;
using int64CombinedRange = combinedRange<int64>;
using realCombinedRange  = combinedRange<real>;

}

#endif //__ranges_hpp__
