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


#ifndef __pointFields_hpp__
#define __pointFields_hpp__

#include "VectorSingle.hpp"
#include "pointField.hpp"
#include "types.hpp"

namespace pFlow
{

template<typename T>
  using pointField_H = pointField<VectorSingle, T, HostSpace>;

template<typename T>
  using pointField_D = pointField<VectorSingle, T>;

/*template<typename T>
  using pointField_HD = pointField<VectorDual, T>;*/


using int8PointField_D 		= pointField<VectorSingle, int8>;

using int8PointField_H 		= pointField<VectorSingle, int8, HostSpace>;

using int32PointField_D 	= pointField<VectorSingle, int32>;

/*using int32PointField_H 	= pointField<VectorSingle, int32, HostSpace>;

using int64PointField_D 	= pointField<VectorSingle, int64>;

using int64PointField_H 	= pointField<VectorSingle, int64, HostSpace>;

using uint32PointField_D 	= pointField<VectorSingle, uint32>;

using uint32PointField_H 	= pointField<VectorSingle, uint32, HostSpace>;

using labelPointField_D 	= pointField<VectorSingle, label>;

using labelPointField_H  	= pointField<VectorSingle, label, HostSpace>;

using realPointField_D 		= pointField<VectorSingle, real>;

using realPointField_H 		= pointField<VectorSingle, real, HostSpace>;

using realx3PointField_D 	= pointField<VectorSingle, realx3> ;

using realx3PointField_H 	= pointField<VectorSingle, realx3, HostSpace>;

using wordPointField_H 		= pointField<VectorSingle, word, HostSpace>;

using int8PointField_HD 	= pointField<VectorDual, int8>;

using int16PointField_HD 	= pointField<VectorDual, int16>;

using int32PointField_HD 	= pointField<VectorDual, int32>;

using int64PointField_HD 	= pointField<VectorDual, int64>;

using uint32PointField_HD 	= pointField<VectorDual, uint32>;

using labelPointField_HD 	= pointField<VectorDual, label>;

using realPointField_HD 	= pointField<VectorDual, real>;

using realx3PointField_HD 	= pointField<VectorDual, realx3>;

using wordPointField 		= pointField<Vector, word, vecAllocator<word>>;*/

}


#endif
