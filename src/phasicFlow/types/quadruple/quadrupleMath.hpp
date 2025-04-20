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

#define Q4Func(fnName)                                                         \
	template<typename T>                                                       \
	inline pFlow::quadruple<T> pFlow::fnName(const quadruple<T>& q)            \
	{                                                                          \
		return quadruple<T>(fnName(q.s_), fnName(q.v_));                       \
	}

#define Q4Func2(fnName)                                                        \
	template<typename T>                                                       \
	inline pFlow::quadruple<T> pFlow::fnName(                                  \
	  const quadruple<T>& arg1, const quadruple<T>& arg2                       \
	)                                                                          \
	{                                                                          \
		return quadruple<T>(                                                   \
		  fnName(arg1.s_, arg2.s_), fnName(arg1.v_, arg2.v_)                   \
		);                                                                     \
	}

//* * * * * * * * * * *  List of functinos * * * * * * * * //
// abs, mod, exp, log, log10, pow, sqrt, cbrt
// sin, cos, tan, asin, acos, atan, atan2
// sinh, cosh, tanh, asinh, acosh, atanh
// min, max
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Q4Func(abs);
Q4Func2(mod);
Q4Func(exp);
Q4Func(log);
Q4Func(log10);
Q4Func2(pow);
Q4Func(sqrt);
Q4Func(cbrt);
Q4Func(sin);
Q4Func(cos);
Q4Func(tan);
Q4Func(asin);
Q4Func(acos);
Q4Func(atan);
Q4Func2(atan2);
Q4Func(sinh);
Q4Func(cosh);
Q4Func(tanh);
Q4Func(asinh);
Q4Func(acosh);
Q4Func(atanh);
Q4Func2(min);
Q4Func2(max);

template<typename T>
inline pFlow::quadruple<T>
pFlow::pow(const pFlow::quadruple<T>& q4, T e)
{
	return quadruple<T>(pow(q4.s_, e), pow(q4.v_, e));
}

// return the min of 3 elements x, y, z
template<typename T>
inline T
pFlow::min(const quadruple<T>& q4)
{
	return min(min(q4.v_), q4.s_);
}

// return the max of 3 elements x, y, z
template<typename T>
inline T
pFlow::max(const quadruple<T>& q4)
{
	return max(max(q4.v_), q4.s_);
}

#undef Q4Func
#undef Q4Func2
