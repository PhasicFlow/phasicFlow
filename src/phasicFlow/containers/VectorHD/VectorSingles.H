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


#ifndef __VectorSingles_H__
#define __VectorSingles_H__


#include "types.H"
#include "VectorSingle.H"

namespace pFlow
{

typedef VectorSingle<int8> 				int8Vector_D;

typedef VectorSingle<int8, HostSpace> 	int8Vector_H;

typedef VectorSingle<int16> 			int16Vector_D;

typedef VectorSingle<int16, HostSpace> 	int16Vector_H;

typedef VectorSingle<int32> 			int32Vector_D;

typedef VectorSingle<int32, HostSpace> 	int32Vector_H;

typedef VectorSingle<int64> 			int64Vector_D;

typedef VectorSingle<int64, HostSpace> 	int64Vector_H;

typedef VectorSingle<uint32> 			uint32Vector_D;

typedef VectorSingle<uint32, HostSpace> uint32Vector_H;

typedef VectorSingle<label> 			labelVector_D;

typedef VectorSingle<label, HostSpace> 	labelVector_H;

typedef VectorSingle<real> 				realVector_D;

typedef VectorSingle<real, HostSpace> 	realVector_H;

typedef VectorSingle<realx3> 				realx3Vector_D;

typedef VectorSingle<realx3, HostSpace> 	realx3Vector_H;

typedef VectorSingle<uint16x3> 				uint16x3Vector_D;

typedef VectorSingle<uint16x3, HostSpace> 	uint16x3Vector_H;

typedef VectorSingle<uint32x3> 				uint32x3Vector_D;

typedef VectorSingle<uint32x3, HostSpace> 	uint32x3Vector_H;

typedef VectorSingle<int32x3> 				int32x3Vector_D;

typedef VectorSingle<int32x3, HostSpace> 	int32x3Vector_H;

typedef VectorSingle<int64x3> 				int64x3Vector_D;

typedef VectorSingle<int64x3, HostSpace> 	int64x3Vector_H;

typedef VectorSingle<realx3x3> 				realx3x3Vector_D;

typedef VectorSingle<realx3x3, HostSpace> 	realx3x3Vector_H;

}

#endif