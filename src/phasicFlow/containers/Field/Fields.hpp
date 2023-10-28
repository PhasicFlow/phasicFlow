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

using int8Field_D 		= Field<VectorSingle, int8>;

using int8Field_H 		= Field<VectorSingle, int8, HostSpace>;

/*using int32Field_D 		= Field<VectorSingle, int32>;

using int32Field_H 		= Field<VectorSingle, int32, HostSpace>;

using int64Field_D 		= Field<VectorSingle, int64>;

using int64Field_H 		= Field<VectorSingle, int64, HostSpace>;

using uint32Field_D 	= Field<VectorSingle, uint32>;

using uint32Field_H 	= Field<VectorSingle, uint32, HostSpace>;

using labelField_D 		= Field<VectorSingle, label>;

using labelField_H 		= Field<VectorSingle, label, HostSpace> ;

using realField_D 		= Field<VectorSingle, real>;

using realField_H 		= Field<VectorSingle, real, HostSpace> ;

using realx3Field_D 	= Field<VectorSingle, realx3>;

using realx3Field_H 	= Field<VectorSingle, realx3, HostSpace>;

using uint16x3Field_D 	= Field<VectorSingle, uint16x3>;

using uint16x3Field_H 	= Field<VectorSingle, uint16x3, HostSpace>;

using uint32x3Field_D   = Field<VectorSingle, uint32x3>;

using uint32x3Field_H   = Field<VectorSingle, uint32x3, HostSpace>;

using int32x3Field_D 	= Field<VectorSingle, int32x3>;

using int32x3Field_H 	= Field<VectorSingle, int32x3, HostSpace>;

using int64x3Field_D 	= Field<VectorSingle, int64x3>;

using int64x3Field_H 	= Field<VectorSingle, int64x3, HostSpace>;

using realx3x3Field_D 	= Field<VectorSingle, realx3x3>;

using realx3x3Field_H 	= Field<VectorSingle, realx3x3, HostSpace>;*/

// - no typedef on device (since word does not compile on CUDA)
using wordField_H 		= Field<VectorSingle, word, HostSpace>;


// host device fields
/*using int8Field_HD 		= Field<VectorDual, int8>;

using int32Field_HD 	= Field<VectorDual, int32>;

using int64Field_HD 	= Field<VectorDual, int64>;

using uint32Field_HD 	= Field<VectorDual, uint32>;

using labelField_HD 	= Field<VectorDual, label>;

using realField_HD 		= Field<VectorDual, real>;

using realx3Field_HD 	= Field<VectorDual, realx3>;

using uint32x3Field_HD  = Field<VectorDual, uint32x3>;

using int32x3Field_HD 	= Field<VectorDual, int32x3>;

using int64x3Field_HD 	= Field<VectorDual, int64x3>;

using realx3x3Field_HD 	= Field<VectorDual, realx3x3>;*/


using wordField 		= Field<Vector, word , vecAllocator<word>>; 



}



#endif  //__Fields_hpp__
