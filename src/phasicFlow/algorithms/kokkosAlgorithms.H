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


#ifndef __kokkosAlgorithms_H__ 
#define __kokkosAlgorithms_H__

#include "KokkosTypes.H"


namespace pFlow::algorithms::KOKKOS
{

template<typename Type, typename ExecutionSpace>
INLINE_FUNCTION_H
int32 count(const Type* first, int32 numElems, const Type& val)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;
	int32 num = 0;
	Kokkos::parallel_reduce("count",
							policy(0, numElems),
							LAMBDA_HD(int32 i, int32& updateVal){
								if(equal(first[i],val)) updateVal++;
							},
							num);
	return num;
}

// fill should be done using deep_copy by kokkos
//void fill(Type* first, int32 numElems, const Type& val);

template<typename Type, typename ExecutionSpace>
INLINE_FUNCTION_H
void fillSequence(Type* first, int32 numElems, const Type& firstVal)
{
	
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;

	Kokkos::parallel_for(
		"fillSequence",
		policy(0, numElems),
		LAMBDA_HD(int32 i){ 
			first[i] = firstVal+i;
		});
	Kokkos::fence();
}

template<typename Type, typename indexType, typename ExecutionSpace>
INLINE_FUNCTION_H
void fillSelected(Type* first, const indexType* indices, const int32 numElems, const Type val)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;
	Kokkos::parallel_for(
		"fillSelected",
		policy(0,numElems),
		LAMBDA_HD(int32 i){
			first[indices[i]]= val;
		});
	Kokkos::fence();
}

template<typename Type, typename indexType, typename ExecutionSpace>
INLINE_FUNCTION_H
void fillSelected(Type* first, const indexType* indices, const Type* vals, const int32 numElems)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;

	Kokkos::parallel_for(
		"fillSelected",
		policy(0,numElems),
		LAMBDA_HD(int32 i){
			first[indices[i]]= vals[i];
		});
	Kokkos::fence();
}

template<typename Type, typename ExecutionSpace>
INLINE_FUNCTION_H
Type max(const Type* first, int32 numElems)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;
	Type maxElement=0;

	Kokkos::parallel_reduce(
		"max",
		policy(0, numElems),
		LAMBDA_HD(int32 i, Type& maxUpdate){
			if(maxUpdate<first[i]) maxUpdate = first[i];
		},
		Kokkos::Max<Type>(maxElement));

	return maxElement;
}

template<typename Type, typename ExecutionSpace>
INLINE_FUNCTION_H
Type min(const Type* first, int32 numElems)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;
	Type minElement;

	Kokkos::parallel_reduce(
		"min",
		policy(0, numElems),
		LAMBDA_HD(int32 i, Type& minUpdate){
			if(first[i] < minUpdate) minUpdate = first[i];
		},
		Kokkos::Min<Type>(minElement));

	return minElement;
}

// we either use CUDA or STD srots
//void sort(Type* first, int32 numElems);
//void sort(Type* first, int32 numElems, CompareFunc compare);
//void permuteSort(const Type* first, PermuteType* pFirst, int32 numElems);

template<typename Type, typename DestType, typename ExecutionSpace>
void exclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{	
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;

	Kokkos::parallel_scan(
		"exclusiveScan",
		policy(0, numElems),
		LAMBDA_HD(const int32 i, DestType& valToUpdate, const bool final)
		{
			const int32 val = first[i];
			if(final)
				dFirst[i] = valToUpdate;
			valToUpdate += val;
		});
}

template<typename Type, typename DestType, typename ExecutionSpace>
void inclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{
	using policy = Kokkos::RangePolicy<
			ExecutionSpace,
			Kokkos::IndexType<int32> >;

	Kokkos::parallel_scan(
		"inclusiveScan",
		policy(0, numElems),
		LAMBDA_HD(const int32 i, int32& valToUpdate, const bool final)
		{
			const int32 val = first[i];
			valToUpdate += val;
			if(final)
				dFirst[i] = valToUpdate;
		});
}

} //pFlow::algorithms::KOKKOS


#endif //__kokkosAlgorithms_H__
