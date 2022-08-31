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

#ifndef __types_H__
#define __types_H__


#include "bTypes.H"

#include "bTypesFunctions.H"

#include "triple.H"

#include "quadruple.H"

#include "typeInfo.H"


namespace pFlow
{

using int8x3 	= triple<int8>;
using int16x3 	= triple<int16>;
using int32x3 	= triple<int32>;
using int64x3	= triple<int64>;
using uint16x3 	= triple<uint16>;
using uint32x3 	= triple<uint32>;
using int32x3 	= triple<int32>;
using int64x3 	= triple<int64>;
using labelx3 	= triple<label>;
using realx3 	= triple<real>;

using uint16x3x3= triple<uint16x3>;
using uint32x3x3= triple<uint32x3>;
using int32x3x3 = triple<int32x3>;
using labelx3x3 = triple<labelx3>;
using realx3x3 	= triple<realx3>;

using real4 	= quadruple<real>;


template<>
inline word basicTypeName<int8x3>(){	return "int8x3"; }

template<>
inline word basicTypeName<int16x3>(){	return "int16x3"; }

template<>
inline word basicTypeName<int32x3>(){	return "int32x3"; }

template<>
inline word basicTypeName<int64x3>(){	return "int64x3"; }

template<>
inline word basicTypeName<uint16x3>(){	return "uint16x3"; }

template<>
inline word basicTypeName<uint32x3>(){	return "uint32x3"; }

template<>
inline word basicTypeName<labelx3>(){	return "labelx3"; }

template<>
inline word basicTypeName<realx3>(){	return "realx3";	}

template<>
inline word basicTypeName<uint16x3x3>(){ return "uint16x3x3"; }

template<>
inline word basicTypeName<uint32x3x3>(){ return "uint32x3x3"; }

template<>
inline word basicTypeName<realx3x3>(){ return "realx3x3"; }


template<>
inline word basicTypeName<real4>(){	return "real4";	}


extern const realx3 zero3;
extern const realx3 one3;
extern const uint32x3 zeroU3;
extern const uint32x3 oneU3;

extern const realx3x3 zero33;
extern const realx3x3 one33;
extern const uint32x3x3 zeroU33;
extern const uint32x3x3 oneU33;



} // pFlow


#endif //__types_H__