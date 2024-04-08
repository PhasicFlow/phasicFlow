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

#include "KokkosUtilities.hpp"
#include "Range.hpp"
#include "numericConstants.hpp"

#include "cudaAlgorithms.hpp"
#include "kokkosAlgorithms.hpp"
#include "stdAlgorithms.hpp"

namespace pFlow
{
template<typename T, typename... properties>
INLINE_FUNCTION_H uint32
count(
  const ViewType1D<T, properties...>& view,
  uint32                              start,
  uint32                              end,
  const T&                            val
)
{
	using ExecutionSpace =
	  typename ViewType1D<T, properties...>::execution_space;

	uint32 numElems = end - start;

	return pFlow::algorithms::KOKKOS::count<T, ExecutionSpace>(
	  view.data() + start, numElems, val
	);
}

template<typename T, typename... properties>
INLINE_FUNCTION_H void
fill(ViewType1D<T, properties...>& view, rangeU32 span, T val)
{
	using exe_space = typename ViewType1D<T, properties...>::execution_space;
	auto subV       = Kokkos::subview(view, span.getPair());
	if constexpr (std::is_trivially_copyable_v<T>)
	{
		Kokkos::deep_copy(subV, val);
	}
	else if constexpr (isHostAccessible<exe_space>())
	{
		for (auto i = span.start(); i < span.end(); i++)
		{
			view[i] = val;
		}
	}
	else
	{
		static_assert("fill is not valid for non-trivially-copyable data type");
	}
}

template<typename T, typename... properties>
void
fill(ViewType1D<T, properties...>& view, uint32 start, uint32 end, T val)
{
	fill(view, rangeU32(start, end), val);
}

template<typename T, typename... properties>
void
fill(
  ViewType3D<T, properties...>& view,
  rangeU32                      range1,
  rangeU32                      range2,
  rangeU32                      range3,
  const T&                      val
)
{
	static_assert(std::is_trivially_copyable_v<T>, "Not valid type for fill");
	auto subV = Kokkos::subview(view, range1, range2, range3);
	Kokkos::deep_copy(subV, val);
}

template<typename T, typename... properties>
void
fill(ViewType3D<T, properties...>& view, const T& val)
{
	static_assert(std::is_trivially_copyable_v<T>, "Not valid type for fill");
	Kokkos::deep_copy(view, val);
}

template<typename Type, typename... properties>
void
fillSequence(
  ViewType1D<Type, properties...>& view,
  uint32                           start,
  uint32                           end,
  const Type                       startVal
)
{
	static_assert(
	  std::is_trivially_copyable_v<Type>, "Not valid type for fill"
	);
	using ExecutionSpace =
	  typename ViewType1D<Type, properties...>::execution_space;
	uint32 numElems = end - start;

	pFlow::algorithms::KOKKOS::fillSequence<Type, ExecutionSpace>(
	  view.data() + start, numElems, startVal
	);

	return;
}

template<
  typename Type,
  typename... properties,
  typename indexType,
  typename... indexProperties>
bool
fillSelected(
  ViewType1D<Type, properties...>           view,
  ViewType1D<indexType, indexProperties...> indices,
  uint32                                    numElems,
  Type                                      val
)
{
	static_assert(
	  std::is_trivially_copyable_v<Type>, "Not valid type for fillSelected"
	);
	static_assert(
	  areAccessible<
	    typename ViewType1D<Type, properties...>::execution_space,
	    typename ViewType1D<indexType, indexProperties...>::memory_space>(),
	  "In fillSelected, arguments view and indices must have similar spaces"
	);

	using ExSpace = typename ViewType1D<Type, properties...>::execution_space;
	using policy  = Kokkos::RangePolicy<ExSpace, Kokkos::IndexType<uint32>>;

	Kokkos::parallel_for(
	  "ViewAlgorithms::fillSelected",
	  policy(0, numElems),
	  LAMBDA_HD(uint32 i){
	    // view[indices[i]]= val;
	  }
	);
	Kokkos::fence();

	return true;
}

template<
  typename Type,
  typename... properties,
  typename indexType,
  typename... indexProperties>
bool
fillSelected(
  ViewType1D<Type, properties...>                 view,
  const ViewType1D<indexType, indexProperties...> indices,
  const ViewType1D<Type, properties...>           vals,
  const uint32                                    numElems
)
{
	static_assert(
	  std::is_trivially_copyable_v<Type>, "Not valid type for fillSelected"
	);
	static_assert(
	  areAccessible<
	    typename ViewType1D<Type, properties...>::execution_space,
	    typename ViewType1D<indexType, indexProperties...>::memory_space>(),
	  "In fillSelected arguments view and indices must have similar spaces"
	);

	using ExecutionSpace =
	  typename ViewType1D<Type, properties...>::execution_space;

	pFlow::algorithms::KOKKOS::fillSelected<Type, indexType, ExecutionSpace>(
	  view.data(), indices.data(), vals.data(), numElems
	);

	return true;
}

template<typename T, typename... properties>
INLINE_FUNCTION_H T
min(const ViewType1D<T, properties...>& view, uint32 start, uint32 end)
{
	using ExecutionSpace =
	  typename ViewType1D<T, properties...>::execution_space;

	uint32 numElems = end - start;

	return pFlow::algorithms::KOKKOS::min<T, ExecutionSpace>(
	  view.data() + start, numElems
	);
}

template<typename T, typename... properties>
INLINE_FUNCTION_H T
max(const ViewType1D<T, properties...>& view, uint32 start, uint32 end)
{
	using ExecutionSpace =
	  typename ViewType1D<T, properties...>::execution_space;

	uint32 numElems = end - start;

	return pFlow::algorithms::KOKKOS::max<T, ExecutionSpace>(
	  view.data() + start, numElems
	);
}

template<
  typename dType,
  typename... dProperties,
  typename sType,
  typename... sProperties>
INLINE_FUNCTION_H void
copy(
  const ViewType1D<dType, dProperties...>& dst,
  const ViewType1D<sType, sProperties...>& src
)
{
	Kokkos::deep_copy(dst, src);
}

template<
  typename dType,
  typename... dProperties,
  typename sType,
  typename... sProperties>
INLINE_FUNCTION_H void
copy(
  const ViewType1D<dType, dProperties...>& dst,
  uint32                                   dStart,
  const ViewType1D<sType, sProperties...>& src,
  uint32                                   sStart,
  uint32                                   sEnd
)
{
	range32 sSpan(sStart, sEnd);
	range32 dSpan(dStart, dStart + (sEnd - sStart));

	auto    srcSub = Kokkos::subview(src, sSpan);
	auto    dstSub = Kokkos::subview(dst, dSpan);

	Kokkos::deep_copy(dstSub, srcSub);
}

template<typename Type, typename... sProperties>
INLINE_FUNCTION_H void
getNth(Type& dst, const ViewType1D<Type, sProperties...>& src, const uint32 n)
{
	auto subV = Kokkos::subview(src, Kokkos::make_pair(n, n + 1));
	hostViewType1D<Type> dstView("getNth", 1);
	// hostViewTypeScalar
	Kokkos::deep_copy(dstView, subV);
	dst = *dstView.data();
}

template<typename T, typename... properties>
INLINE_FUNCTION_H void
sort(ViewType1D<T, properties...>& view, uint32 start, uint32 end)
{
	using ExecutionSpace =
	  typename ViewType1D<T, properties...>::execution_space;

	uint32 numElems = end - start;

	if constexpr (isHostAccessible<ExecutionSpace>())
	{
		pFlow::algorithms::STD::sort<T, true>(view.data() + start, numElems);
		return;
	}

#ifdef __CUDACC__

	pFlow::algorithms::CUDA::sort<T>(view.data() + start, numElems);
#else
	static_assert("sort on device is not defined!");

#endif

	return;
}

template<typename T, typename... properties, typename CompareFunc>
INLINE_FUNCTION_H void
sort(
  ViewType1D<T, properties...>& view,
  uint32                        start,
  uint32                        end,
  CompareFunc                   compare
)
{
	using ExecutionSpace =
	  typename ViewType1D<T, properties...>::execution_space;

	uint32 numElems = end - start;

	if constexpr (isHostAccessible<ExecutionSpace>())
	{
		pFlow::algorithms::STD::sort<T, CompareFunc, true>(
		  view.data() + start, numElems, compare
		);
		return;
	}

#ifdef __CUDACC__

	pFlow::algorithms::CUDA::sort<T, CompareFunc>(
	  view.data() + start, numElems, compare
	);
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
void
permuteSort(
  const ViewType1D<Type, properties...>&   view,
  uint32                                   start,
  uint32                                   end,
  ViewType1D<permType, permProperties...>& permuteView,
  uint32                                   permStart
)
{
	static_assert(
	  areAccessible<
	    typename ViewType1D<Type, properties...>::execution_space,
	    typename ViewType1D<permType, permProperties...>::memory_space>(),
	  "In permuteSort, view and permuteView should have the same space"
	);

	using ExecutionSpace =
	  typename ViewType1D<Type, properties...>::execution_space;

	uint32 numElems = end - start;

	pFlow::algorithms::STD::permuteSort<Type, permType, true>(
	  view.data() + start, permuteView.data() + permStart, numElems
	);
	return;

#ifdef __CUDACC__

	pFlow::algorithms::CUDA::permuteSort(
	  view.data() + start, permuteView.data() + permStart, numElems
	);
#else
	static_assert("sort on device is not defined!");

#endif
}

template<typename T>
INLINE_FUNCTION_HD int32
binarySearch_(const T* array, int32 length, const T& val)
{
	if (length <= 0)
		return -1;

	int low  = 0;
	int high = length - 1;

	while (low <= high)
	{
		int mid = low + (high - low) / 2;

		if (array[mid] > val)
		{
			high = mid - 1;
		}
		else if (array[mid] < val)
		{
			low = mid + 1;
		}
		else
		{
			return mid;
		}
	}

	return -1; // val not found in array[0, length)
}

/// On DEVICE and HOST calls
template<typename Type, typename... properties>
INLINE_FUNCTION_HD uint32
binarySearch(
  const ViewType1D<Type, properties...>& view,
  uint32                                 start,
  uint32                                 end,
  const Type&                            val
)
{
	if (end <= start)
		return -1;

	if (auto res = binarySearch_(view.data() + start, end - start, val);
	    res != -1)
	{
		return res + start;
	}
	else
	{
		return res;
	}
}

template<typename Type, typename... properties, typename... dProperties>
void
exclusiveScan(
  const ViewType1D<Type, properties...>& view,
  uint32                                 start,
  uint32                                 end,
  ViewType1D<Type, dProperties...>&      dView,
  uint32                                 dStart
)
{
	static_assert(
	  areAccessible<
	    typename ViewType1D<Type, properties...>::execution_space,
	    typename ViewType1D<Type, dProperties...>::memory_space>(),
	  "In exclusiveScan, view and dView should have the same space"

	);

	using ExecutionSpace =
	  typename ViewType1D<Type, properties...>::execution_space;

	uint32 numElems = end - start;

	pFlow::algorithms::KOKKOS::exclusiveScan<Type, ExecutionSpace>(
	  view.data() + start, dView.data() + dStart, numElems
	);
}

template<typename Type, typename... properties, typename... dProperties>
void
inclusiveScan(
  const ViewType1D<Type, properties...>& view,
  uint32                                 start,
  uint32                                 end,
  ViewType1D<Type, dProperties...>&      dView,
  uint32                                 dStart
)
{
	using ExecutionSpace =
	  typename ViewType1D<Type, properties...>::execution_space;

	static_assert(
	  areAccessible<
	    typename ViewType1D<Type, properties...>::execution_space,
	    typename ViewType1D<Type, dProperties...>::memory_space>(),
	  "In exclusiveScan, view and dView should have the same space"
	);

	uint32 numElems = end - start;

	pFlow::algorithms::KOKKOS::inclusiveScan<Type, ExecutionSpace>(
	  view.data() + start, dView.data() + dStart, numElems
	);
}

} // pFlow

#endif // __ViewAlgorithms_hpp__
