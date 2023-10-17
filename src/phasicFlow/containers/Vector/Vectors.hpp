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

#ifndef __Vectors_hpp__
#define __Vectors_hpp__


#include "types.hpp"
#include "Vector.hpp"

namespace pFlow
{


using int8Vector 	= Vector<int8>;

using int32Vector 	= Vector<int32>;

using int64Vector 	= Vector<int64>;

using uint8Vector  = Vector<uint8>;

using uint32Vector   = Vector<uint32>;

using uint64Vector   = Vector<uint64>;

using realVector 	= Vector<real> 	;

using realx3Vector 	= Vector<realx3>;

using realx3x3Vector 	= Vector<realx3x3>;

using wordVector 		= Vector<word>;



}


#endif //__Vectors_hpp__