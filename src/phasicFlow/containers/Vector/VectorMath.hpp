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


#define VecFunc(fnName) 									\
template<typename T, typename Allocator>										\
inline pFlow::Vector<T, Allocator> pFlow::fnName(const Vector<T,Allocator>& v)	\
{															\
	Vector<T, Allocator> res(v.capacity(), Logical());		\
	for(auto& e:v)											\
	{														\
		res.push_back( fnName(e) );							\
	}														\
	return std::move(res);									\
}															\
template<typename T, typename Allocator, typename indexFunc>					\
inline pFlow::Vector<T, Allocator> pFlow::fnName(const Vector<T, Allocator>& v, indexFunc iFn) 	\
{																		\
	Vector<T, Allocator> res(v.capacity(), Logical());								\
	for(label i=0; i<v.size(); i++)										\
	{																	\
		if( iFn(i) )													\
			res.push_back(fnName(v[i]));								\
		else															\
			res.push_back(v[i]);										\
	}																	\
	return std::move(res);												\
}

#define VecFunc2(fnName) 									\
template<typename T, typename Allocator>										\
inline pFlow::Vector<T, Allocator> pFlow::fnName(const Vector<T, Allocator>& v1, const Vector<T, Allocator>& v2)	\
{															\
	Vector<T, Allocator> res(v1.capacity(), Logical());					\
	for(label i=0; i<v1.size(); i++)							\
	{														\
		res.push_back( fnName(v1[i], v2[i]));				\
	}														\
	return std::move(res);									\
}															\
template<typename T, typename Allocator, typename indexFunc>					\
inline pFlow::Vector<T, Allocator> pFlow::fnName(const Vector<T, Allocator>& v1, const Vector<T, Allocator>& v2, indexFunc iFn) 	\
{																		\
	Vector<T, Allocator> res(v1.capacity(), Logical());							\
	for(label i=0; i<v1.size(); i++)									\
	{																	\
		if( iFn(i) )													\
			res.push_back(fnName(v1[i], v2[i]));						\
		else															\
			res.push_back(v1[i]);										\
	}																	\
	return std::move(res);												\
}


//* * * * * * * * * * *  List of functinos * * * * * * * * //
// abs, mod, exp, log, log10, pow, sqrt, cbrt
// sin, cos, tan, asin, acos, atan, atan2
// sinh, cosh, tanh, asinh, acosh, atanh 
// min, max
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * //

VecFunc(abs);
VecFunc2(mod);
VecFunc(exp);
VecFunc(log);
VecFunc(log10);
VecFunc2(pow);
VecFunc(sqrt);
VecFunc(cbrt);
VecFunc(sin);
VecFunc(cos);
VecFunc(tan);
VecFunc(asin);
VecFunc(acos);
VecFunc(atan);
VecFunc2(atan2);
VecFunc(sinh);
VecFunc(cosh);
VecFunc(tanh);
VecFunc(asinh);
VecFunc(acosh);
VecFunc(atanh);

#undef VecFunc

//// special implementation of math functions 
namespace pFlow
{

template<typename T, typename Allocator>
inline Vector<T, Allocator> pow(const Vector<T, Allocator>& v, T e)
{
	Vector<T, Allocator> res(v.capacity(), Logical());
	for(auto& elm:v)
	{
		res.push_back(pow(elm,e));
	}
	return std::move(res);
}

template<typename T, typename Allocator, typename indexFunc>
inline Vector<T,Allocator> pow(const Vector<T,Allocator>& v, T e, indexFunc iFn)
{
	Vector<T, Allocator> res(v.capacity(), Logical());
	for(label i=0; i<v.size(); i++)
	{
		if(iFn(i))
		{
			res.push_back(pow(v[i],e));
		}
		else
		{
			res.push_back(v[i]);
		}
	}
	return std::move(res);
}

template<typename T, typename Allocator>
inline T min(const Vector<T, Allocator>& v)
{
	T minVal(largestPositive<T>());
	for(auto& elm:v)
	{
		minVal = min(elm, minVal);
	}
	return minVal;
}

template<typename T, typename Allocator, typename indexFunc>
inline T min(const Vector<T, Allocator>& v, indexFunc iFn)
{
	T minVal(largestPositive<T>());
	for(label i=0; i<v.size(); i++)
	{
		if(iFn(i))
		{
			minVal = min(v[i], minVal);
		}
		
	}
	return minVal;
}

template<typename T, typename Allocator>
inline T max(const Vector<T, Allocator>& v)
{
	T maxVal(largestNegative<T>());
	for(auto& elm:v)
	{
		maxVal = max(elm, maxVal);
	}
	return maxVal;
}

template<typename T, typename Allocator ,typename indexFunc>
inline T max(const Vector<T, Allocator>& v, indexFunc iFn)
{
	T maxVal(largestNegative<T>());
	for(label i=0; i<v.size(); i++)
	{
		if(iFn(i))
		{
			maxVal = max(v[i], maxVal);
		}
		
	}
	return maxVal;
}

template<typename T, typename Allocator>
inline T sum(const Vector<T, Allocator>& v)
{
	T s = static_cast<T>(0);
	for(auto& elm:v)
	{
		s += elm;
	}
	return s;
}

template<typename T, typename Allocator, typename indexFunc>
inline T sum(const Vector<T, Allocator>& v, indexFunc iFn)
{
	T s = static_cast<T>(0);
	for(label i=0; i<v.size(); ++i)
	{
		if(iFn(i))
			s += v[i];
	}
	return s;
}

}