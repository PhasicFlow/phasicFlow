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

#ifndef __KokkosUtilities_H__
#define __KokkosUtilities_H__


#include "KokkosTypes.H"


namespace pFlow
{

template<typename ExecutionSpace>
INLINE_FUNCTION_H
bool constexpr isHostAccessible()
{
	return Kokkos::SpaceAccessibility<ExecutionSpace,HostSpace>::accessible;
}

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
void realloc( ViewType1D<Type,Properties...>& view, int32 len)
{
	Kokkos::realloc(view, len);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocNoInit(ViewType1D<Type,Properties...>& view, int32 len)
{
	using ViewType = ViewType1D<Type,Properties...>;
	word vl = view.label();
	view = ViewType();  // Deallocate first
  	view = ViewType( 
  		Kokkos::view_alloc(
  			Kokkos::WithoutInitializing,
  			vl), 
  		len);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocFill( ViewType1D<Type,Properties...>& view, int32 len, Type val)
{
	reallocNoInit(view, len);
	Kokkos::deep_copy(view, val);
}


template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void realloc( ViewType3D<Type,Properties...>& view, int32 len1, int32 len2, int32 len3)
{
	Kokkos::realloc(view, len1, len2, len3);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocNoInit(ViewType3D<Type,Properties...>& view, int32 len1, int32 len2, int32 len3)
{
	using ViewType = ViewType3D<Type,Properties...>;
	word vl = view.label();
	view = ViewType();  // Deallocate first
  	view = ViewType( 
  		Kokkos::view_alloc(
  			Kokkos::WithoutInitializing,
  			vl), 
  		len1, len2, len3);
}

template <
	typename Type,
	typename... Properties>
INLINE_FUNCTION_H
void reallocFill( ViewType3D<Type,Properties...>& view, int32 len1, int32 len2, int32 len3, Type val)
{
	reallocNoInit(view, len1, len2, len3);
	Kokkos::deep_copy(view, val);
}


template<typename ViewType>
INLINE_FUNCTION_H
void swapViews(ViewType& v1, ViewType &v2)
{
	auto tmp = v1;
	v1 = v2;
	v2 = tmp;
}

} // pFlow

#endif  //__KokkosUtilities_H__
