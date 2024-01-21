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
  using pointField_H = pointField<T, HostSpace>;

template<typename T>
  using pointField_D = pointField<T>;


using int8PointField_D 		= pointField_D<int8>;
using int8PointField_H 		= pointField_H<int8>;

using uint8PointField_D 		= pointField_D<uint8>;
using uint8PointField_H 		= pointField_H<uint8>;

using int32PointField_D 		= pointField_D<int32>;
using int32PointField_H 		= pointField_H<int32>;

using uint32PointField_D 		= pointField_D<uint32>;
using uint32PointField_H 		= pointField_H<uint32>;

using int64PointField_D 		= pointField_D<int64>;
using int63PointField_H 		= pointField_H<int64>;

using uint64PointField_D 		= pointField_D<uint64>;
using uint64PointField_H 		= pointField_H<uint64>;

using int32PointField_D 		= pointField_D<int32>;
using int32PointField_H 		= pointField_H<int32>;

using realPointField_D 		= pointField_D<real>;
using realPointField_H 		= pointField_H<real>;

using realx3PointField_D 		= pointField_D<realx3>;
using realx3PointField_H 		= pointField_H<realx3>;

using realx4PointField_D 		= pointField_D<realx4>;
using realx4PointField_H 		= pointField_H<realx4>;





}


#endif
