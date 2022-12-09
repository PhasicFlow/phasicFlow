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

#ifndef __baseAlgorithms_H__
#define __baseAlgorithms_H__


#include "numericConstants.H"
#include "KokkosUtilities.H"

inline const size_t sizeToSerial__ = 64;

namespace pFlow
{

// counts the number of elements that matches val
// the execution space is selected based on the View::execution_spcae 
/*template<typename T, typename... properties>
INLINE_FUNCTION_H
size_t count(
	const ViewType1D<T, properties...>& view,
	size_t start,
	size_t end,
	const T& val
	)
{

	auto RP = Kokkos::RangePolicy<
		Kokkos::IndexType<size_t>,
		typename ViewType1D<T, properties...>::execution_space >(start, end);

	size_t totalNum=0;
	Kokkos::parallel_reduce(
		"baseAlgorithms-count",
		RP,
		LAMBDA_HD(label i, size_t & valueToUpdate){
			if( equal(view[i], val) ) valueToUpdate += 1;
		}, totalNum );

	return totalNum;
}*/


template<typename T, typename... properties>
INLINE_FUNCTION_H
T min( const ViewType1D<T, properties...>& view, size_t start, size_t end ) 
{

	T minValue = largestPositive<T>();

	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename ViewType1D<T, properties...>::execution_space >(start, end);

	Kokkos::parallel_reduce("baseAlgorithms-min",
							RP, 
							LAMBDA_HD(label i, T& valueToUpdate){
								valueToUpdate = min(view[i],valueToUpdate);
							},
							Kokkos :: Min < T >( minValue )
							);
	return minValue;
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
T max( const ViewType1D<T, properties...>& view, size_t start, size_t end ) 
{

	T maxValue = largestNegative<T>();

	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename ViewType1D<T, properties...>::execution_space >(start, end);

	Kokkos::parallel_reduce("baseAlgorithms-max",
							RP, 
							LAMBDA_HD(label i, T& valueToUpdate){
								valueToUpdate = max(view[i],valueToUpdate);
							},
							Kokkos::Max<T>( maxValue )
							);
	return maxValue;
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
T min_serial(const ViewType1D<T, properties...>& view, size_t start, size_t end)
{
	T minValue = largestPositive<T>();
	for(label i=start; i<end; ++i)
	{
		minValue = min(minValue, view[i]);
	}
	return minValue;
}

template<typename T, typename... properties>
INLINE_FUNCTION_H
T max_serial(const ViewType1D<T, properties...>& view, size_t start, size_t end)
{
	T maxValue = largestNegative<T>();
	for(label i=start; i<end; ++i)
	{
		maxValue = max(maxValue, view[i]);
	}
	return maxValue;
}


template<typename UnaryFunction, typename T, typename... properties>
void apply_to_each(const ViewType1D<T, properties...>& view, size_t start, size_t end, UnaryFunction func)
{
	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename ViewType1D<T, properties...>::execution_space >(start, end);

	Kokkos::parallel_for("baseAlgorithms-for_each",
							RP, 
							LAMBDA_HD(label i){
								view[i] = func(i);
							}
							);
}


template<typename T, typename... properties>
void insertSetElementH
(
	ViewType1D<T, properties...>& view,
	hostViewType1D<label>& selected,
	T val
)
{
	
	for(auto i=0; i<selected.size();++i)
	{
		view[selected[i]] = val;
	}

}


template<typename T, typename... properties>
void insertSetElementH
(
	ViewType1D<T, properties...>& view,
	hostViewType1D<label>& selected,
	hostViewType1D<T>& vals
)
{
	
	for(auto i=0; i<selected.size(); ++i)
	{
		view[selected[i]] = static_cast<const T&>(vals[i]);
	}
}

template<typename T, typename... properties>
void insertSetElementD
(
	ViewType1D<T, properties...>& view,
	deviceViewType1D<label>& selected,
	T val
)
{
	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename ViewType1D<T, properties...>::execution_space >(0, selected.size());

	Kokkos::parallel_for(
		"baseAlgorithms-insertSetElementD",
		RP,
		LAMBDA_D(size_t i) {
			view[selected[i]] = val; } );

}

template<typename T, typename... properties>
void insertSetElementD
(
	ViewType1D<T, properties...>& view,
	deviceViewType1D<label>& selected,
	deviceViewType1D<T>& vals
)
{
	auto RP = Kokkos::RangePolicy<
	Kokkos::IndexType<size_t>,
	typename ViewType1D<T, properties...>::execution_space >(0, selected.size());

	Kokkos::parallel_for(
		"baseAlgorithms-insertSetElementD",
		RP,
		LAMBDA_D(size_t i) {
			view[selected[i]] = vals[i]; } );

}

/*template<typename T, typename... properties>
void fill
(
	ViewType1D<T, properties...>& view,
	range range,
	T val
)
{
	auto subV = Kokkos::subview(view, range);
	Kokkos::deep_copy(subV, val);
}*/


template<typename T, typename... properties>
void fill
(
	ViewType3D<T, properties...>& view,
	range range1,
	range range2,
	range range3,
	T val
)
{
	auto subV = Kokkos::subview(view, range1, range2, range3);
	Kokkos::deep_copy(subV, val);
}

}


#endif // __VectorSingleMath_H__
