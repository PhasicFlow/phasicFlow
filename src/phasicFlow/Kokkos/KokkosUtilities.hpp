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

#ifndef __KokkosUtilities_hpp__
#define __KokkosUtilities_hpp__


#include "KokkosTypes.hpp"
#include "pFlowMacros.hpp"
#include "types.hpp"
#include "span.hpp"
#include "iOstream.hpp"

namespace pFlow
{

template<typename ExecutionSpace>
INLINE_FUNCTION_H
bool constexpr isHostAccessible()
{
	return Kokkos::SpaceAccessibility<ExecutionSpace,HostSpace>::accessible;
}

template<typename ExecutionSpace>
INLINE_FUNCTION_H
bool constexpr isDeviceAccessible()
{
	return Kokkos::SpaceAccessibility<ExecutionSpace,DefaultExecutionSpace::memory_space>::accessible;
}

/// Is MemoerySpace accessible from ExecutionSpace
template<typename ExecutionSpace, typename MemoerySpace>
INLINE_FUNCTION_H
bool constexpr areAccessible()
{
	return Kokkos::SpaceAccessibility<ExecutionSpace,MemoerySpace>::accessible;
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocInit( ViewType1D<Type,Properties...>& view, uint32 len)
{
	Kokkos::realloc(Kokkos::WithoutInitializing, view, len);	
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocNoInit(ViewType1D<Type,Properties...>& view, uint32 len)
{
	Kokkos::realloc(Kokkos::WithoutInitializing, view, len);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocFill( ViewType1D<Type,Properties...>& view, uint32 len, Type val)
{
	reallocNoInit(view, len);
	Kokkos::deep_copy(view, val);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocInit( ViewType2D<Type,Properties...>& view, uint32 len1, uint32 len2)
{
	Kokkos::realloc(view, len1, len2);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocNoInit(ViewType2D<Type,Properties...>& view, uint32 len1, uint32 len2)
{	
  	Kokkos::realloc(Kokkos::WithoutInitializing, view, len1, len2);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocFill( ViewType2D<Type,Properties...>& view, uint32 len1, uint32 len2, Type val)
{
	reallocNoInit(view, len1, len2);
	Kokkos::deep_copy(view, val);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocInit( ViewType3D<Type,Properties...>& view, uint32 len1, uint32 len2, uint32 len3)
{
	Kokkos::realloc(view, len1, len2, len3);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocNoInit(ViewType3D<Type,Properties...>& view, uint32 len1, uint32 len2, uint32 len3)
{
	
  	Kokkos::realloc(Kokkos::WithoutInitializing, view, len1, len2, len3);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocFill( ViewType3D<Type,Properties...>& view, uint32 len1, uint32 len2, uint32 len3, Type val)
{
	reallocNoInit(view, len1, len2, len3);
	Kokkos::deep_copy(view, val);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void resizeInit(ViewType1D<Type,Properties...>& view, uint32 newLen)
{
	Kokkos::resize(view, newLen);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void resizeNoInit(ViewType1D<Type,Properties...>& view, uint32 newLen)
{
	Kokkos::resize(Kokkos::WithoutInitializing, view, newLen);
}

template<typename ViewType>
INLINE_FUNCTION_H
void swapViews(ViewType& v1, ViewType &v2)
{
	static_assert(
      std::is_move_assignable<ViewType>::value && std::is_move_constructible<ViewType>::value,
      "swapViews arguments must be move assignable and move constructible");

	ViewType tmp = std::move(v1);
  	v1     = std::move(v2);
  	v2     = std::move(tmp);
}


template<typename T1, typename T2>
INLINE_FUNCTION_H
iOstream& operator <<(iOstream& os, const Pair<T1,T2>& p)
{
	os<<'('<<p.first<<" "<<p.second<<')';
	return os;
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
span<T> makeSpan(ViewType1D<T, properties...> & v)
{
	return span<T>(v.data(), v.size());
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
span<T> makeSpan(ViewType1D<T, properties...> & v, uint32 size)
{
	return span<T>(v.data(), size);
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
span<T> makeSpan(const ViewType1D<T, properties...> & v)
{
	return span<T>(const_cast<T*>(v.data()), v.size());
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
span<T> makeSpan(const ViewType1D<T, properties...> & v, uint32 size)
{
	return span<T>(const_cast<T*>(v.data()), size);
}


template<typename T, typename... properties>
INLINE_FUNCTION_H
iOstream& operator <<(iOstream& os, const ViewType1D<T, properties...> & v)
{
	
	using ExSpace = typename ViewType1D<T, properties...>::execution_space;

	static_assert(isHostAccessible<ExSpace>(), "View memory is not accessible from Host");

	span<T> spn(v.data(), v.size());
	os<<spn;
	
	return os;
}


} // pFlow

#endif  //__KokkosUtilities_hpp__
