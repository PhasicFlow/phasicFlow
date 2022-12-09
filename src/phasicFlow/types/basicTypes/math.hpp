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

#ifndef __math_hpp__
#define __math_hpp__

#include <cmath>

#ifdef __CUDACC__
#include "math.h"
#endif

#include "builtinTypes.hpp"

//* * * * * * * * * * *  List of functinos * * * * * * * * //
// abs, mod, exp, log, log10, pow, sqrt, cbrt
// sin, cos, tan, asin, acos, atan, atan2
// sinh, cosh, tanh, asinh, acosh, atanh 
// min, max
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace pFlow
{


INLINE_FUNCTION_HD real abs(real x)
{
#ifdef __CUDACC__
	return ::fabs(x);
#else
	return std::fabs(x);
#endif
}


INLINE_FUNCTION_HD int64 abs(int64 x)
{
#ifdef __CUDACC__
	return ::abs(x);
#else
	return std::abs(x);
#endif
}

INLINE_FUNCTION_HD int32 abs(int32 x)
{
#ifdef __CUDACC__
	return ::abs(x);
#else
	return std::abs(x);
#endif
}


INLINE_FUNCTION_HD real mod(real x, real y)
{
#ifdef __CUDACC__
	return ::fmod(x, y);
#else
	return std::fmod(x, y);
#endif
}

INLINE_FUNCTION_HD int64 mod(int64 x, int64 y)
{
	return x%y;
}

INLINE_FUNCTION_HD int32 mod(int32 x, int32 y)
{
	return x%y;
}

INLINE_FUNCTION_HD int64 mod(label x, label y)
{
	return x%y;
}

INLINE_FUNCTION_HD int32 mod(uint32 x, uint32 y)
{
	return x%y;
}

INLINE_FUNCTION_HD real remainder(real x, real y)
{
#ifdef __CUDACC__
	return ::remainder(x,y);
#else
	return std::remainder(x,y);
#endif
}

INLINE_FUNCTION_HD real exp(real x)
{
#ifdef __CUDACC__
	return ::exp(x);
#else
	return std::exp(x);
#endif
}

INLINE_FUNCTION_HD real log(real x)
{
#ifdef __CUDACC__
	return ::log(x);
#else
	return std::log(x);
#endif
}

INLINE_FUNCTION_HD real log10(real x)
{
#ifdef __CUDACC__
	return ::log10(x);
#else
	return std::log10(x);
#endif
}

INLINE_FUNCTION_HD real pow(real x, real y)
{
#ifdef __CUDACC__
	return ::pow(x, y);
#else
	return std::pow(x, y);
#endif

}


INLINE_FUNCTION_HD real sqrt(real x)
{
#ifdef __CUDACC__
	return ::sqrt(x);
#else
	return std::sqrt(x);
#endif
}


INLINE_FUNCTION_HD real cbrt (real x)
{
#ifdef __CUDACC__
	return ::cbrt (x);
#else
	return std::cbrt (x);
#endif
}


INLINE_FUNCTION_HD real sin(real x)
{
#ifdef __CUDACC__
	return ::sin(x);
#else
	return std::sin(x);
#endif
}


INLINE_FUNCTION_HD real cos(real x)
{
#ifdef __CUDACC__
	return ::cos(x);
#else
	return std::cos(x);
#endif
}


INLINE_FUNCTION_HD real tan(real x)
{
#ifdef __CUDACC__
	return ::tan(x);
#else
	return std::tan(x);
#endif
}

INLINE_FUNCTION_HD real asin(real x)
{
#ifdef __CUDACC__
	return ::asin(x);
#else
	return std::asin(x);
#endif
	
}


INLINE_FUNCTION_HD real acos(real x)
{
#ifdef __CUDACC__
	return ::acos(x);
#else
	return std::acos(x);
#endif
}


INLINE_FUNCTION_HD real atan(real x)
{
#ifdef __CUDACC__
	return ::atan(x);
#else
	return std::atan(x);
#endif

}


INLINE_FUNCTION_HD real atan2(real y, real x)
{
#ifdef __CUDACC__
	return ::atan2(y, x);
#else
	return std::atan2(y, x);
#endif
}


INLINE_FUNCTION_HD real sinh(real x)
{
#ifdef __CUDACC__
	return ::sinh(x);
#else
	return std::sinh(x);
#endif
}


INLINE_FUNCTION_HD real cosh(real x)
{
#ifdef __CUDACC__
	return ::cosh(x);
#else
	return std::cosh(x);
#endif
}


INLINE_FUNCTION_HD real tanh(real x)
{
#ifdef __CUDACC__
	return ::tanh(x);
#else
	return std::tanh(x);
#endif
}

INLINE_FUNCTION_HD real asinh(real x)
{
#ifdef __CUDACC__
	return ::asinh(x);
#else
	return std::asinh(x);
#endif
}

INLINE_FUNCTION_HD real acosh(real x)
{
#ifdef __CUDACC__
	return ::acosh(x);
#else
	return std::acosh(x);
#endif
}

INLINE_FUNCTION_HD real atanh(real x)
{
#ifdef __CUDACC__
	return ::atanh(x);
#else
	return std::atanh(x);
#endif
}

INLINE_FUNCTION_HD real min(real x, real y)
{
#ifdef __CUDACC__
	return ::fmin(x,y);
#else
	return std::fmin(x,y);
#endif
}

INLINE_FUNCTION_HD int64 min(int32 x, int32 y)
{
#ifdef __CUDACC__
	return ::min(x, y);
#else
	return std::min(x, y);
#endif
}

INLINE_FUNCTION_HD int64 min(int64 x, int64 y)
{
#ifdef __CUDACC__
	return ::min(x, y);
#else
	return std::min(x, y);
#endif
}

INLINE_FUNCTION_HD label min(label x, label y)
{
#ifdef __CUDACC__
	return ::min(x, y);
#else
	return std::min(x, y);
#endif

}

INLINE_FUNCTION_HD uint32 min(uint32 x, uint32 y)
{
#ifdef __CUDACC__
	return ::min(x, y);
#else
	return std::min(x, y);
#endif
}

INLINE_FUNCTION_HD uint32 min(uint16 x, uint16 y)
{
#ifdef __CUDACC__
	return ::min(x, y);
#else
	return std::min(x, y);
#endif
}

INLINE_FUNCTION_HD real max(real x, real y)
{
#ifdef __CUDACC__
	return ::fmax(x,y);
#else
	return std::fmax(x,y);
#endif
}

INLINE_FUNCTION_HD int64 max(int64 x, int64 y)
{
#ifdef __CUDACC__
	return ::max(x, y);
#else
	return std::max(x, y);
#endif
}

INLINE_FUNCTION_HD int32 max(int32 x, int32 y)
{
#ifdef __CUDACC__
	return ::max(x, y);
#else
	return std::max(x, y);
#endif
}

INLINE_FUNCTION_HD label max(label x, label y)
{
#ifdef __CUDACC__
	return ::max(x, y);
#else
	return std::max(x, y);
#endif
}


INLINE_FUNCTION_HD uint32 max(uint32 x, uint32 y)
{
#ifdef __CUDACC__
	return ::max(x, y);
#else
	return std::max(x, y);
#endif
}

INLINE_FUNCTION_HD uint32 max(uint16 x, uint16 y)
{
#ifdef __CUDACC__
	return ::max(x, y);
#else
	return std::max(x, y);
#endif

}

} // pFlow



#endif // __math_hpp__
