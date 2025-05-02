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


#ifndef __sortedPairs_hpp__
#define __sortedPairs_hpp__

#include "unsortedPairs.hpp"
#include "KokkosUtilities.hpp"

namespace pFlow
{


template<typename executionSpace, typename idType>
class sortedPairs
:
	public unsortedPairs<executionSpace, idType>
{
public:

	using UnsortedPairs = unsortedPairs<executionSpace,idType>;

	using IdType 			= typename UnsortedPairs::IdType;

	using ExecutionSpace 	= typename UnsortedPairs::ExecutionSpace;

	using memory_space 		= typename ExecutionSpace::memory_space;

	using PairType 			= typename UnsortedPairs::PairType;

	using ContainerType 	= typename UnsortedPairs::ContainerType;

	struct pairAccessor
	{
		using PairType = typename sortedPairs::PairType;

		uint32 size_;

		ViewType1D<PairType,ExecutionSpace> sortedParis_;

		INLINE_FUNCTION_HD
		uint32 size()const { return size_; }

		INLINE_FUNCTION_HD
		uint32 loopCount()const { return size_; }

		INLINE_FUNCTION_HD
		bool isValid(uint32 i)const { return i<size_; }

		INLINE_FUNCTION_HD
		PairType getPair(uint32 i)const {	return sortedParis_[i];	}

		INLINE_FUNCTION_HD
		bool getPair(uint32 i, PairType& pair)const {
			if(i<size_)	{
				pair = sortedParis_[i];
				return true;
			}
			return false;
		}
	};

	class TagFillFlag{};

	class TagFillPairs{};

protected:

	/// size of pair list
	uint32 			size_ = 0;

	ViewType1D<uint32,ExecutionSpace>		flags_;

	ViewType1D<PairType,ExecutionSpace> 	sortedPairs_;

	using rpFillFlag = Kokkos::RangePolicy<
		ExecutionSpace,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<uint32>,
		TagFillFlag	>;

	using rpFillPairs = Kokkos::RangePolicy<
		ExecutionSpace,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<uint32>,
		TagFillPairs>;
	
public:
	
	// - type info
	TypeInfoNV("sortedPairs");


	// constructors
	explicit sortedPairs(const word& name, uint32 initialSize =1)
	:
		UnsortedPairs(initialSize),
		flags_( groupNames(name, "flags_"), UnsortedPairs::capacity()+1),
		sortedPairs_(groupNames(name, "sortedPairs_"), UnsortedPairs::capacity())
	{}

	
	bool beforeBroadSearch()
	{
		this->clear();
		return true;
	}

	bool afterBroadSearch()
	{
		prepareSorted();
		return true;
	}

	// - Device call
	//   return the pair at index idx
	//   perform no check for size and existance
	INLINE_FUNCTION_HD
	PairType getPair(uint32 idx)const
	{
		return sortedPairs_[idx];
	}

	// - Device/host call
	//   return the pair at index idx
	INLINE_FUNCTION_HD
	bool getPair(uint32 idx, PairType& p)const
	{
		if(isValid(idx))
		{
			p = getPair(idx);
			return true;
		}
		else
		{
			return false;
		}
	}

	INLINE_FUNCTION_HD
	bool isValid(uint32 idx)const
	{
		return idx < size_;	
	}


	//use this when the value of size_ is updated
	INLINE_FUNCTION_H
	uint32 size()const
	{
		return size_;
	}

	uint32 loopCount()const
	{
		return size_;
	}

	pairAccessor getPairs()const
	{
		return {size_, sortedPairs_};
	}

	INLINE_FUNCTION_H
	void clear()
	{
		UnsortedPairs::clear();
		size_ = 0;
	}

	void prepareSorted()
	{
		// first check the size of flags_ 
		uint32 capacity = UnsortedPairs::capacity();

		if( capacity+1 > flags_.size() )
		{
			reallocInit(flags_, capacity+1);
		}

		// fill the flags
		Kokkos::parallel_for(
			"contactPairsSorted::fillFlag",
			rpFillFlag(0,capacity+1),
			*this);
		Kokkos::fence();
		

		// inclusive scan on flags_
		exclusiveScan(flags_, 0, capacity+1,flags_,0);

		Kokkos::fence(); // Kokkos's scan is blocking I guess. So, this could be removed. 
		
		// get the last value of flags_ to obtain the size of sortedPairs_		
		getNth(size_, flags_, capacity);
		
		if(size_ == 0 )return;

		// resize sortedPairs_ if necessary;
		if( size_>sortedPairs_.size() )
		{
			// get more space to prevent reallocations in next iterations
			uint32 len = size_*1.1+1; 
			reallocInit(sortedPairs_, len);
		}
		
		Kokkos::parallel_for(
			"contactPairsSorted::fillPairs",
			rpFillPairs(0,this->capacity()),
			*this);
		Kokkos::fence();

		// - sort paris based on the first and second
		sort(sortedPairs_, 0, size_ );
		
		

	}

	INLINE_FUNCTION_HD
	void operator()(TagFillFlag, uint32 i)const
	{
		if(this->container_.valid_at(i) )
			flags_[i] = 1;
		else
			flags_[i] = 0;
	}

	INLINE_FUNCTION_HD
	void operator()(TagFillPairs, uint32 i)const
	{
		auto fi = flags_[i];
		if(fi!=flags_[i+1])
			sortedPairs_[fi] = this->container_.key_at(i);
	}	

};

}

#endif //__sortedPairs_hpp__
