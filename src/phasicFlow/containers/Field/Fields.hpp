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

#ifndef __Fields_hpp__
#define __Fields_hpp__

#include "types.hpp"
#include "Field.hpp"
#include "VectorSingle.hpp"
//#include "VectorDual.hpp"


namespace pFlow
{

using int8Field_D 		= Field<int8>;

using int8Field_H 		= Field<int8, HostSpace>;

using int32Field_D 		= Field<int32>;

using int32Field_H 		= Field<int32, HostSpace>;

using int64Field_D 		= Field<int64>;

using int64Field_H 		= Field<int64, HostSpace>;

using uint8Field_D     = Field<uint8>;

using uint8Field_H     = Field<uint8, HostSpace>;

using uint32Field_D 	= Field<uint32>;

using uint32Field_H 	= Field<uint32, HostSpace>;

using uint64Field_D 		= Field<uint64>;

using uint64Field_H 		= Field<uint64, HostSpace> ;

using uint32x3Field_D   = Field<uint32x3>;

using uint32x3Field_H   = Field<uint32x3, HostSpace>;

using realField_D 		= Field<real>;

using realField_H 		= Field<real, HostSpace> ;

using realx3Field_D 	= Field<realx3>;

using realx3Field_H 	= Field<realx3, HostSpace>;

using realx3x3Field_D 	= Field<realx3x3>;

using realx3x3Field_H 	= Field<realx3x3, HostSpace>;

// - no typedef on device (since word does not compile on CUDA)
using wordField_H 		= Field<word, HostSpace>;


}



#endif  //__Fields_hpp__
