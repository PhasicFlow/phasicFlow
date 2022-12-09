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

using int16Vector 	= Vector<int16>;

using int32Vector 	= Vector<int32>;

using int64Vector 	= Vector<int64>;

using uint32Vector 	= Vector<uint32>;

using labelVector 	= Vector<label>;

using realVector 	= Vector<real> 	;

using realx3Vector 	= Vector<realx3>;

using uint16x3Vector= Vector<uint16x3>;

using uint32x3Vector= Vector<uint32x3>;

using int32x3Vector = Vector<int32x3>;

using int64x3Vector = Vector<int64x3>;

using uint16x3x3Vector 	= Vector<uint16x3x3>;

using uint32x3x3Vector 	= Vector<uint32x3x3>;

using int32x3x3Vector 	= Vector<int32x3x3>;

using realx3x3Vector 	= Vector<realx3x3>;

using wordVector 		= Vector<word>;

/*template<>
inline word Vector<label>::TYPENAME() { return "labelVector"; }
typedef Vector<label> 	labelVector;

template<>
inline word Vector<real>::TYPENAME() { return "realVector"; }
typedef Vector<real> 	realVector;

template<>
inline word Vector<unit3>::TYPENAME() { return "unit3Vector"; }
typedef Vector<unit3> 	unit3Vector;

template<>
inline word Vector<real3>::TYPENAME() { return "real3Vector"; }
typedef Vector<real3>	real3Vector;

template<>
inline word Vector<real33>::TYPENAME() { return "real33Vector"; }
typedef Vector<real33>	real33Vector;

template<>
inline word Vector<bool>::TYPENAME() { return "boolVector"; }
typedef Vector<bool> 	boolVector;

template<>
inline word Vector<word>::TYPENAME() { return "wordVector"; }
typedef Vector<word> 	wordVector;

template<>
inline word Vector<sint>::TYPENAME() { return "sintVector"; }
typedef Vector<sint> 	sintVector;*/

}


#endif //__Vectors_hpp__