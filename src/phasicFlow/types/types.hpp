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

#ifndef __types_hpp__
#define __types_hpp__


#include "bTypes.hpp"

#include "bTypesFunctions.hpp"

#include "triple.hpp"

#include "quadruple.hpp"

#include "typeInfo.hpp"


namespace pFlow
{

using int8x3 	= triple<int8>;
using int32x3 	= triple<int32>;
using int64x3	= triple<int64>;
using uint8x3   = triple<uint8>;
using uint32x3 	= triple<uint32>;
using uint64x3 	= triple<uint64>;
using realx3 	= triple<real>;

using int32x3x3 = triple<int32x3>;
using uint32x3x3= triple<uint32x3>;
using realx3x3 	= triple<realx3>;

using realx4 	= quadruple<real>;


template<>
inline word basicTypeName<int8x3>(){	return "int8x3"; }

template<>
inline word basicTypeName<int32x3>(){	return "int32x3"; }

template<>
inline word basicTypeName<int64x3>(){	return "int64x3"; }

template<>
inline word basicTypeName<uint8x3>(){	return "uint8x3"; }

template<>
inline word basicTypeName<uint32x3>(){	return "uint32x3"; }

template<>
inline word basicTypeName<uint64x3>(){	return "unit64x3"; }

template<>
inline word basicTypeName<realx3>(){	return "realx3";	}

template<>
inline word basicTypeName<int32x3x3>(){ return "int32x3x3"; }

template<>
inline word basicTypeName<uint32x3x3>(){ return "uint32x3x3"; }

template<>
inline word basicTypeName<realx3x3>(){ return "realx3x3"; }


template<>
inline word basicTypeName<realx4>(){	return "realx4";	}


extern const realx3 zero3;
extern const realx3 one3;


extern const realx3x3 zero33;
extern const realx3x3 one33;

extern const realx4 zero4; 

} // pFlow


#endif //__types_hpp__