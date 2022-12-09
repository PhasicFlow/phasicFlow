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

#ifndef __ViewAlgorithms_hpp__
#define __ViewAlgorithms_hpp__


#include "numericConstants.hpp"
#include "KokkosUtilities.hpp"
#include "kokkosAlgorithms.hpp"
#include "stdAlgorithms.hpp"
#include "cudaAlgorithms.hpp"


namespace pFlow
{ 	

inline const size_t maxSizeToSerial__ = 64;

template<typename T, typename... properties>
INLINE_FUNCTION_H
int32 count(
	const ViewType1D<T, properties...>& view,
	int32 start,
	int32 end,
	const T& val)
{
	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			return pFlow::algorithms::STD::count<T,false>(
				view.data()+start,
				numElems,
				val);
		}
	}

	return  pFlow::algorithms::KOKKOS::count<T, ExecutionSpace>(
			view.data()+start,
			numElems,
			val);
}	

template<typename T, typename... properties>
INLINE_FUNCTION_H
void fill
(
	ViewType1D<T, properties...>& view,
	range span,
	T val
)
{
	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;


	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		int32 numElems = span.second-span.first;
		if( numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::fill<T,false>(
				view.data()+span.first,
				numElems,
				val);
			return;
		}
	}

	auto subV = Kokkos::subview(view, span);
	Kokkos::deep_copy(subV, val);
}

template<typename T, typename... properties>
void fill
(
	ViewType1D<T, properties...>& view,
	int32 start,
	int32 end,
	T val
)
{
	fill(view, range(start,end),val);
}

template<
	typename Type,
	typename... properties>
void fillSequence(
	ViewType1D<Type, properties...>& view,
	int32 start,
	int32 end,
	const Type startVal
	)
{

	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{ 
			pFlow::algorithms::STD::fillSequence<Type,false>(
				view.data()+start,
				numElems,
				startVal);
			return ;
		}
	}

	pFlow::algorithms::KOKKOS::fillSequence<Type, ExecutionSpace>(
			view.data()+start,
			numElems,
			startVal);

	return ;
}


template<
	typename Type,
	typename... properties,
	typename indexType,
	typename... indexProperties>
bool fillSelected(
	ViewType1D<Type, properties...> view,
	const ViewType1D<indexType, indexProperties...> indices,
	const int32 numElems,
	const Type val,
	typename std::enable_if_t<
		areAccessible<
			typename ViewType1D<Type, properties...>::execution_space,
			typename ViewType1D<indexType, indexProperties...>::memory_space>(),
		bool> = true )
{

	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	
	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{ 
			pFlow::algorithms::STD::fillSelected<Type,indexType,false>(
				view.data(),
				indices.data(),
				numElems,
				val);
			return true;
		}
	}

	pFlow::algorithms::KOKKOS::fillSelected<Type, indexType, ExecutionSpace>(
			view.data(),
			indices.data(),
			numElems,
			val);

	return true;
}

template<
	typename Type,
	typename... properties,
	typename indexType,
	typename... indexProperties>
	//typename valType> //,
	//typename... valProperties>
bool fillSelected(
	ViewType1D<Type, properties...> view,
	const ViewType1D<indexType, indexProperties...> indices,
	const ViewType1D<Type, indexProperties...> vals,
	const int32 numElems ,
	typename std::enable_if_t<
		areAccessible<
			typename ViewType1D<Type, properties...>::execution_space,
			typename ViewType1D<indexType, indexProperties...>::memory_space>(),
		bool> = true )
{

	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	


	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{ 
			pFlow::algorithms::STD::fillSelected<Type,indexType,false>(
				view.data(),
				indices.data(),
				vals.data(),
				numElems
				);
			return true;
		}
	}


	pFlow::algorithms::KOKKOS::fillSelected<Type, indexType, ExecutionSpace>(
			view.data(),
			indices.data(),
			vals.data(),
			numElems
			);

	return true;
}


template<typename T, typename... properties>
INLINE_FUNCTION_H
T min( 
	const ViewType1D<T, properties...>& view,
	int32 start,
	int32 end)
{

	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			return 
			pFlow::algorithms::STD::min<T,false>(
				view.data()+start,
				numElems);
		}
	}

	return  
	pFlow::algorithms::KOKKOS::min<T, ExecutionSpace>(
		view.data()+start,
		numElems);
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
T max( 
	const ViewType1D<T, properties...>& view,
	int32 start,
	int32 end)
{

	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			return 
			pFlow::algorithms::STD::max<T,false>(
				view.data()+start,
				numElems);
		}
	}

	return  
	pFlow::algorithms::KOKKOS::max<T, ExecutionSpace>(
		view.data()+start,
		numElems);
} 

template <
	typename dType,
	typename... dProperties,
	typename sType,
	typename... sProperties>
INLINE_FUNCTION_H
void copy(
	const ViewType1D<dType, dProperties...>& dst,
	const ViewType1D<sType, sProperties...>& src
	)
{
	Kokkos::deep_copy(dst,src);
}

template <
	typename dType,
	typename... dProperties,
	typename sType,
	typename... sProperties>
INLINE_FUNCTION_H
void copy(
	const ViewType1D<dType, dProperties...>& dst,
	int32 dStart,
	const ViewType1D<sType, sProperties...>& src,
	int32 sStart,
	int32 sEnd
	)
{

	range sSpan(sStart,sEnd);
	range dSpan(dStart,dStart+(sEnd-sStart));
	
	auto srcSub = Kokkos::subview(src, sSpan);
	auto dstSub = Kokkos::subview(dst, dSpan);

	Kokkos::deep_copy(dstSub,srcSub);
}

template <
	typename dType,
	typename sType,
	typename... sProperties>
INLINE_FUNCTION_H
void getNth(
	dType& dst,
	const ViewType1D<sType, sProperties...>& src,
	const int32 n
	)
{
	range span(n,n+1);
	auto subV = Kokkos::subview(src, span);
	hostViewType1D<dType> dstView("getNth",1);
	Kokkos::deep_copy(dstView,subV);
	dst = *dstView.data();
}


template<typename T, typename... properties>
INLINE_FUNCTION_H
void sort( 
	ViewType1D<T, properties...>& view,
	int32 start,
	int32 end)
{

	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::sort<T,false>(
				view.data()+start,
				numElems);
			return;
		}
		else
		{
			pFlow::algorithms::STD::sort<T,true>(
				view.data()+start,
				numElems);
			return;
		}
	}

#ifdef __CUDACC__
	
	pFlow::algorithms::CUDA::sort<T>(
		view.data()+start,
		numElems);
#else
	static_assert("sort on device is not defined!");

#endif

	return;
}

template<typename T, typename... properties, typename CompareFunc>
INLINE_FUNCTION_H
void sort( 
	ViewType1D<T, properties...>& view,
	int32 start,
	int32 end,
	CompareFunc compare)
{

	using ExecutionSpace = typename ViewType1D<T, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::sort<T,CompareFunc,false>(
				view.data()+start,
				numElems,
				compare);
			return;
		}
		else
		{
			pFlow::algorithms::STD::sort<T,CompareFunc,true>(
				view.data()+start,
				numElems,
				compare);
			return;
		}
	}

#ifdef __CUDACC__
	
	pFlow::algorithms::CUDA::sort<T, CompareFunc>(
		view.data()+start,
		numElems,
		compare);
#else
	static_assert("sort on device is not defined!");

#endif

	return;
}

template<
	typename Type,
	typename... properties,
	typename permType,
	typename... permProperties>
void permuteSort(
	const ViewType1D<Type, properties...>& view,
	int32 start,
	int32 end,
	ViewType1D<permType, permProperties...>& permuteView,
	int32 permStart,
	typename std::enable_if_t<
		areAccessible<
			typename ViewType1D<Type, properties...>::execution_space,
			typename ViewType1D<permType, permProperties...>::memory_space>(),
		bool> = true )
{
	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	
	int32 numElems = end-start;

	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::permuteSort<Type,permType,false>(
				view.data()+start,
				permuteView.data()+permStart,
				numElems	);
			return;
		}
		else
		{
			pFlow::algorithms::STD::permuteSort<Type,permType,true>(
				view.data()+start,
				permuteView.data()+permStart,
				numElems);
			return;
		}
	}

#ifdef __CUDACC__
	
	pFlow::algorithms::CUDA::permuteSort(
		view.data()+start,
		permuteView.data()+permStart,
		numElems);
#else
	static_assert("sort on device is not defined!");

#endif

}

template<
	typename Type,
	typename... properties>
INLINE_FUNCTION_HD
int32 binarySearch(
	const ViewType1D<Type, properties...>& view,
	int32 start,
	int32 end,
	const Type& val)
{
	
	if(end<=start)return -1;

	if(auto res = 
		pFlow::algorithms::binarySearch(view.data()+start,end-start,val); res>=0) {
		return res+start;
	}
	else{
		return res;
	}
}

template<
	typename Type,
	typename... properties,
	typename dType,
	typename... dProperties>
void exclusiveScan(
	const ViewType1D<Type, properties...>& view,
	int32 start,
	int32 end,
	ViewType1D<dType, dProperties...>& dView,
	int32 dStart,
	typename std::enable_if_t<
		areAccessible<
			typename ViewType1D<Type, properties...>::execution_space,
			typename ViewType1D<dType, dProperties...>::memory_space>(),
		bool> = true )
{
	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	
	int32 numElems = end-start;
	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::exclusiveScan<Type,dType,false>(
				view.data()+start,
				dView.data()+dStart,
				numElems);
			return;
		}
	}

	pFlow::algorithms::KOKKOS::exclusiveScan<Type,dType,ExecutionSpace>(
		view.data()+start,
		dView.data()+dStart,
		numElems);
}


template<
	typename Type,
	typename... properties,
	typename dType,
	typename... dProperties>
void inclusiveScan(
	const ViewType1D<Type, properties...>& view,
	int32 start,
	int32 end,
	ViewType1D<dType, dProperties...>& dView,
	int32 dStart,
	typename std::enable_if_t<
		areAccessible<
			typename ViewType1D<Type, properties...>::execution_space,
			typename ViewType1D<dType, dProperties...>::memory_space>(),
		bool> = true )
{
	using ExecutionSpace = typename ViewType1D<Type, properties...>::execution_space;
	
	int32 numElems = end-start;
	if constexpr( isHostAccessible<ExecutionSpace>())
	{
		if(numElems<maxSizeToSerial__)
		{
			pFlow::algorithms::STD::inclusiveScan<Type,dType,false>(
				view.data()+start,
				dView.data()+dStart,
				numElems);
			return;
		}
	}

	pFlow::algorithms::KOKKOS::inclusiveScan<Type,dType,ExecutionSpace>(
		view.data()+start,
		dView.data()+dStart,
		numElems);
}

} // pFlow


#endif // Viewalgorithms
