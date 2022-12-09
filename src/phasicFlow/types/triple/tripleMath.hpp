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

#define T3Func(fnName) 									\
template<typename T> 									\
INLINE_FUNCTION_HD pFlow::triple<T> pFlow::fnName(const pFlow::triple<T>& v) 			\
{																\
	return pFlow::triple<T>(fnName(v.x_), fnName(v.y_), fnName(v.z_));	\
}

#define T3Func2(fnName)  															\
template<typename T>																\
INLINE_FUNCTION_HD pFlow::triple<T> pFlow::fnName(const pFlow::triple<T>& arg1, const pFlow::triple<T>& arg2)	\
{																					\
	return pFlow::triple<T>(fnName(arg1.x_, arg2.x_), fnName(arg1.y_,arg2.y_), fnName(arg1.z_, arg2.z_)); \
}

//* * * * * * * * * * *  List of functinos * * * * * * * * //
// abs, mod, exp, log, log10, pow, sqrt, cbrt
// sin, cos, tan, asin, acos, atan, atan2
// sinh, cosh, tanh, asinh, acosh, atanh 
// min, max
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * //


T3Func(abs);
T3Func2(mod);
T3Func(exp);
T3Func(log);
T3Func(log10);
T3Func2(pow);
T3Func(sqrt);
T3Func(cbrt);
T3Func(sin);
T3Func(cos);
T3Func(tan);
T3Func(asin);
T3Func(acos);
T3Func(atan);
T3Func2(atan2);
T3Func(sinh);
T3Func(cosh);
T3Func(tanh);
T3Func(asinh);
T3Func(acosh);
T3Func(atanh);
T3Func2(min);
T3Func2(max);

//// special forms of functions 

// elements of t3 raised by e
template<typename T>
INLINE_FUNCTION_HD pFlow::triple<T> pFlow::pow(const pFlow::triple<T>& t3, T e)
{
	return pFlow::triple<T>( pow(t3.x_, e), pow(t3.y_,e), pow(t3.z_,e));
}

// return the min of 3 elements x, y, z
template<typename T>
INLINE_FUNCTION_HD T pFlow::min(const pFlow::triple<T>& t3)
{
	return min( min(t3.x_, t3.y_), t3.z_);
}

// return the max of 3 elements x, y, z
template<typename T>
INLINE_FUNCTION_HD T pFlow::max(const pFlow::triple<T>& t3)
{
	return max( max(t3.x_, t3.y_), t3.z_);
}



#undef T3Func
#undef T3Func2