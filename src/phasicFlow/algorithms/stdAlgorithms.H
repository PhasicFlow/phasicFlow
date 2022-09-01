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

#ifndef __stdAlgorithms_H__ 
#define __stdAlgorithms_H__

#include <algorithm>
#include <execution>

#include "pFlowMacros.H"
#include "algorithmFunctions.H"
#include "types.H"

namespace pFlow::algorithms::STD
{

template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
int32 count(const Type* first, int32 numElems, const Type& val)
{
	if constexpr (useParallel)
		return std::count_if(
			std::execution::par_unseq,
			first, first+numElems,
			[=](const Type& check){ return equal(check,val);} );
	else
		return std::count_if(
			first, first+numElems,
			[=](const Type& check){ return equal(check,val);} );
	return 0; // for nvcc
}


template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void fill(Type* first, int32 numElems, const Type& val)
{
	if constexpr (useParallel)
		std::fill(std::execution::par_unseq, first, first+numElems, val);
	else
		std::fill(first, first+numElems, val);
}

template<typename Type, typename indexType, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void fillSelected(Type* first, const indexType* indices, const int32 numElems, const Type val)
{
	if constexpr(useParallel)
	{
		std::for_each_n(
			std::execution::par_unseq,
			indices,
			numElems,
			[=](indexType i){
				first[i] = val;
			});
	}
	else
	{
		std::for_each_n(
			indices,
			numElems,
			[=](indexType i){
				first[i] = val;
			});
	}
}

template<typename Type, typename indexType, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void fillSelected(Type* first, const indexType* indices, const Type* vals, const int32 numElems)
{
	for(indexType i=0; i<numElems; i++)
	{
		first[indices[i]]=vals[i]; 
	}
}

template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void fillSequence(Type* first, int32 numElems, const Type& firstVal)
{
	if constexpr (useParallel)
		std::for_each_n(
			std::execution::par_unseq,
			first,
			numElems,
			[=](Type& ref){ ref = firstVal+std::distance(first,&ref);});
	else
		std::iota(first, first+numElems, firstVal);
}

template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
Type max(const Type* first, int32 numElems)
{
	if constexpr(useParallel)
		return *std::max_element(
				std::execution::par_unseq,
				first,
				first+numElems,
				less<Type>());
	else
		return *std::max_element(
				first,
				first+numElems,
				less<Type>());
}

template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
Type min(const Type* first, int32 numElems)
{
	if constexpr(useParallel)
		return *(std::min_element(
				std::execution::par_unseq,
				first,
				first+numElems,
				less<Type>()));
	else
		return *(std::min_element(
				first,
				first+numElems,
				less<Type>()));
}

template<typename Type, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void sort(Type* first, int32 numElems)
{
	if constexpr(useParallel)
	{
		std::sort(
			std::execution::par,
			first,
			first+numElems,
			less<Type>());
	}
	else
	{
		std::sort(
			first,
			first+numElems,
			less<Type>());
	}
}

template<typename Type, typename CompareFunc, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void sort(Type* first, int32 numElems, CompareFunc compare)
{
	if constexpr(useParallel)
	{
		std::sort(
			std::execution::par_unseq,
			first,
			first+numElems,
			compare);
	}
	else
	{
		std::sort(
			first,
			first+numElems,
			compare);
	}
}

template<typename Type, typename PermuteType, bool useParallel = useStdParallel__>
INLINE_FUNCTION_H
void permuteSort(const Type* first, PermuteType* pFirst, int32 numElems)
{
	struct compOperator
	{
		const Type* first_;
		bool operator()(const PermuteType& lhs, const PermuteType& rhs)const {
			return first_[lhs]<first_[rhs]; }
	};

	compOperator compare{first};
	fillSequence<Type, useParallel>(pFirst, numElems, static_cast<PermuteType>(0));
	sort<Type, compOperator, useParallel>(pFirst, numElems, compare);
}


template<typename Type, typename DestType, bool useParallel = useStdParallel__>
void exclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{
	if constexpr (useParallel)
	{
		//// - this does not work when first and dFirst point to the same location
		std::exclusive_scan(
			//std::execution::par_unseq,
			first, first+numElems,
			dFirst, 0);
	}
	else
	{
		std::exclusive_scan(
			first, first+numElems,
			dFirst,0);
	}
}

template<typename Type, typename DestType, bool useParallel = useStdParallel__>
void inclusiveScan(Type* first, DestType* dFirst, int32 numElems)
{
	if constexpr (useParallel)
	{
		std::inclusive_scan(
			std::execution::par_unseq,
			first, first+numElems,
			dFirst);
	}
	else
	{
		std::inclusive_scan(
			first, first+numElems,
			dFirst);
	}
}

}


#endif //__stdAlgorithms_H__