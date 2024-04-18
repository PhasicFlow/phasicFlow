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

#ifdef __CUDACC__
#include "math.h"
#else
#include <cmath>
#endif

#include "builtinTypes.hpp"
#include "pFlowMacros.hpp"

//* * * * * * * * * * *  List of functinos * * * * * * * * //
// abs, mod, exp, log, log10, pow, sqrt, cbrt
// sin, cos, tan, asin, acos, atan, atan2
// sinh, cosh, tanh, asinh, acosh, atanh
// min, max
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace pFlow
{

INLINE_FUNCTION_HD
real
abs(real x)
{
#ifdef __CUDACC__
	return ::fabs(x);
#else
	return std::fabs(x);
#endif
}

#ifndef __CUDACC__
INLINE_FUNCTION_HD
int64
abs(int64 x)
{
	return std::abs(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD int32
abs(int32 x)
{
	return std::abs(x);
}
#endif

INLINE_FUNCTION_HD real
mod(real x, real y)
{
#ifdef __CUDACC__
	return ::fmod(x, y);
#else
	return std::fmod(x, y);
#endif
}

INLINE_FUNCTION_HD int64
mod(int64 x, int64 y)
{
	return x % y;
}

INLINE_FUNCTION_HD int32
mod(int32 x, int32 y)
{
	return x % y;
}

INLINE_FUNCTION_HD int64
mod(uint64 x, uint64 y)
{
	return x % y;
}

INLINE_FUNCTION_HD auto
mod(uint32 x, uint32 y)
{
	return x % y;
}

#ifndef __CUDACC__
INLINE_FUNCTION_HD real
remainder(real x, real y)
{
	return std::remainder(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_H
real
exp(real x)
{
	return std::exp(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
log(real x)
{
	return std::log(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
log10(real x)
{
	return std::log10(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
pow(real x, real y)
{
	return std::pow(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
sqrt(real x)
{
	return std::sqrt(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
cbrt(real x)
{
	return std::cbrt(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
sin(real x)
{
	return std::sin(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
cos(real x)
{
	return std::cos(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
tan(real x)
{
	return std::tan(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
asin(real x)
{
	return std::asin(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
acos(real x)
{
	return std::acos(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
atan(real x)
{
	return std::atan(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD real
atan2(real y, real x)
{
	return std::atan2(y, x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
sinh(real x)
{
	return std::sinh(x);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
cosh(real x)
{
	return std::cosh(x);
}
#endif

INLINE_FUNCTION_HD real
tanh(real x)
{
#ifdef __CUDACC__
	return ::tanh(x);
#else
	return std::tanh(x);
#endif
}

INLINE_FUNCTION_HD real
asinh(real x)
{
#ifdef __CUDACC__
	return ::asinh(x);
#else
	return std::asinh(x);
#endif
}

INLINE_FUNCTION_HD real
acosh(real x)
{
#ifdef __CUDACC__
	return ::acosh(x);
#else
	return std::acosh(x);
#endif
}

#ifndef __CUDACC__
INLINE_FUNCTION_HD
real
atanh(real x)
{
	return std::atanh(x);
}
#endif

INLINE_FUNCTION_HD real
min(real x, real y)
{
#ifdef __CUDACC__
	return ::fmin(x, y);
#else
	return std::fmin(x, y);
#endif
}

#ifndef __CUDACC__
INLINE_FUNCTION_HD
int64
min(int32 x, int32 y)
{
	return std::min(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
int64
min(int64 x, int64 y)
{
	return std::min(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD
uint64
min(uint64 x, uint64 y)
{
	return std::min(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD uint32
min(uint32 x, uint32 y)
{
	return std::min(x, y);
}
#endif

INLINE_FUNCTION_HD real
max(real x, real y)
{
#ifdef __CUDACC__
	return ::fmax(x, y);
#else
	return std::fmax(x, y);
#endif
}

#ifndef __CUDACC__
INLINE_FUNCTION_HD int64
max(int64 x, int64 y)
{
	return std::max(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD int32
max(int32 x, int32 y)
{
	return std::max(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD uint64
max(uint64 x, uint64 y)
{
	return std::max(x, y);
}
#endif

#ifndef __CUDACC__
INLINE_FUNCTION_HD uint32
max(uint32 x, uint32 y)
{
	return std::max(x, y);
}
#endif

} // pFlow

#endif // __math_hpp__
