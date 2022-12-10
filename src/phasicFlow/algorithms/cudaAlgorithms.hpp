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


#ifndef __cudaAlgorithms_hpp__ 
#define __cudaAlgorithms_hpp__


#ifdef __CUDACC__

#include <thrust/fill.h>
#include <thrust/count.h>
#include <thrust/scan.h>
#include <thrust/sort.h>
#include <thrust/extrema.h>
#include <thrust/execution_policy.h>


#include "pFlowMacros.hpp"
#include "algorithmFunctions.hpp"
#include "types.hpp"

namespace pFlow::algorithms::CUDA
{

template<typename Type>
INLINE_FUNCTION_H
int32 count(const Type* first, int32 numElems, const Type& val)
{
	return thrust::count_if(
			thrust::device,
			first, first+numElems,
			LAMBDA_HD(const Type& check){ return equal(check,val);} );
}


template<typename Type>
INLINE_FUNCTION_H
void fill(Type* first, int32 numElems, const Type& val)
{
	thrust::fill(thrust::device, first, first+numElems, val);
}

template<typename Type>
INLINE_FUNCTION_H
void fillSequence(Type* first, int32 numElems, const Type& firstVal)
{
	
	thrust::for_each_n(
		thrust::device,
		first,
		numElems,
		LAMBDA_HD(Type& ref){ ref = firstVal+(&ref - first);});
}

template<typename Type, typename indexType>
INLINE_FUNCTION_H
void fillSelected(Type* first, const indexType* indices, const int32 numElems, const Type val)
{
		
	thrust::for_each_n(
			thrust::device,
			indices,
			numElems,
			LAMBDA_HD(indexType i){
				first[i] = val;
			});

}

template<typename Type>
INLINE_FUNCTION_H
Type max(const Type* first, int32 numElems)
{
	Type resH;
	auto* resD = thrust::max_element(
				thrust::device,
				first,
				first+numElems,
				less<Type>());
	
	cudaMemcpy(&resH, resD, sizeof(Type),cudaMemcpyDeviceToHost);
	return resH;
}

template<typename Type>
INLINE_FUNCTION_H
Type min(const Type* first, int32 numElems)
{
	Type resH;
	auto* resD = thrust::min_element(
				thrust::device,
				first,
				first+numElems,
				less<Type>());
	cudaMemcpy(&resH, resD, sizeof(Type),cudaMemcpyDeviceToHost);
	return resH;
}

template<typename Type>
INLINE_FUNCTION_H
void sort(Type* first, int32 numElems)
{
	
	thrust::sort(
		thrust::device,
		first,
		first+numElems,
		less<Type>());
	
}

template<typename Type, typename CompareFunc>
INLINE_FUNCTION_H
void sort(Type* first, int32 numElems, CompareFunc compare)
{
	
	thrust::sort(
		thrust::device,
		first,
		first+numElems,
		compare);
	
}

template<typename Type, typename PermuteType>
INLINE_FUNCTION_H
void permuteSort(const Type* first, PermuteType* pFirst, int32 numElems)
{
	
	fillSequence(pFirst, numElems, static_cast<PermuteType>(0));
	thrust::sort(
		thrust::device,
		pFirst,
		pFirst+numElems,
		LAMBDA_HD(const PermuteType& lhs, const PermuteType& rhs){
			return first[lhs]<first[rhs]; });
}


template<typename Type, typename DestType>
void exclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{	
	thrust::exclusive_scan(
		thrust::device,
		first, first+numElems,
		dFirst, 0);	
}

template<typename Type, typename DestType>
void inclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{
	thrust::inclusive_scan(
			thrust::device,
			first, first+numElems,
			dFirst);
}

} // end of namespace 


#endif //__CUDA__


#endif //__cudaAlgorithms_hpp__