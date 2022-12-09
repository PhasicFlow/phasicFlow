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

#ifndef __VectorDuals_hpp__
#define __VectorDuals_hpp__


#include "types.hpp"
#include "VectorDual.hpp"

namespace pFlow
{

using int8Vector_HD 	= VectorDual<int8>;

using int16Vector_HD 	= VectorDual<int16>;

using int32Vector_HD 	= VectorDual<int32>;

using int64Vector_HD 	= VectorDual<int64>;

using uint32Vector_HD 	= VectorDual<uint32>;

using labelVector_HD 	= VectorDual<label>;

using realVector_HD 	= VectorDual<real>;

using realx3Vector_HD 	= VectorDual<realx3>;

using realx3x3Vector_HD = VectorDual<realx3x3>;


}

#endif