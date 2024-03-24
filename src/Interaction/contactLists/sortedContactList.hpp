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

#ifndef __sortedContactList_hpp__ 
#define __sortedContactList_hpp__

#include "sortedPairs.hpp"

namespace pFlow
{

template<typename valueType, typename executionSpace, typename idType>
class sortedContactList
:
	public sortedPairs<executionSpace, idType>
{
public:

	using SortedPairs = sortedPairs<executionSpace, idType>;

	using ValueType 		= valueType;

	using IdType 			= typename SortedPairs::IdType;

	using ExecutionSpace 	= typename SortedPairs::ExecutionSpace;

	using memory_space 		= typename SortedPairs::memory_space;

	using PairType 			= typename SortedPairs::PairType;

	using ContainerType 	= typename SortedPairs::ContainerType;

	class TagReFillPairs{};

protected:

	ViewType1D<ValueType,ExecutionSpace> 	values_;

	uint32 									size0_ = 0;

	ViewType1D<PairType,ExecutionSpace> 	sortedPairs0_;

	ViewType1D<ValueType,ExecutionSpace> 	values0_;


	void adjustCapacity()
	{
		if(auto s = this->size(); s > values_.size())
		{
			reallocNoInit(values_, s);
		}
	}

	

	using rpReFillPairs = Kokkos::RangePolicy<
		ExecutionSpace,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<uint32>,
		TagReFillPairs>;

public:

	TypeInfoNV("sortedContactList");


	explicit sortedContactList(uint32 initialSize =1)
	:
		SortedPairs(initialSize),
		values_("values", SortedPairs::capacity()),
		sortedPairs0_("sortedPairs0", SortedPairs::capacity()),
		values0_("values0", SortedPairs::capacity())
	{}

	bool beforeBroadSearch()
	{
		swapViews(values0_, values_);
		swapViews(sortedPairs0_, this->sortedPairs_);
		size0_ = this->size();
		return SortedPairs::beforeBroadSearch();
	}

	bool afterBroadSearch()
	{
		SortedPairs::afterBroadSearch();
		
		adjustCapacity();

		Kokkos::parallel_for(
			"sortedContactList::reFillPairs",
			rpReFillPairs(0, this->size() ),
			*this
			);
		Kokkos::fence();

		return true;
	}

	INLINE_FUNCTION_HD
	ValueType getValue(uint32 idx)const
	{
		return values_[idx];
	}

	INLINE_FUNCTION_HD
	void setValue(uint32 idx, const ValueType& val)const
	{
		values_[idx] = val;
	}

	INLINE_FUNCTION_HD
	void operator()(TagReFillPairs, uint32 idx)const
	{

		uint32 searchLen = max(size0_/1000u,10u);
		uint32 start 	= idx-min(searchLen,idx);
		uint32 end 	= min(size0_,idx+searchLen);
		auto newPair = this->sortedPairs_[idx];
		if(	auto idx0 = binarySearch(
				sortedPairs0_,
				start,
				end,
				newPair);
				idx0!=-1)
		{
			values_[idx] = values0_[idx0];
		}
		else if(auto idx0 = binarySearch(
			sortedPairs0_,
			start, 
			end,
			newPair);
			idx0!=-1)
		{
			values_[idx] = values0_[idx0];
			
		}
		else
		{
			values_[idx] = ValueType();
		}
	}



}; // sortedContactList



} // pFlow



#endif //__sortedContactList_hpp__
