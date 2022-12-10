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

#ifndef __unsortedPairs_hpp__
#define __unsortedPairs_hpp__

#include "KokkosTypes.hpp"
#include "types.hpp"

namespace pFlow
{


template<typename executionSpace, typename idType>
class unsortedPairs
{
public:

	using UnsortedPairs 	= unsortedPairs<executionSpace,idType>;

	using IdType 			= idType;

	using ExecutionSpace 	= executionSpace;

	using memory_space 		= typename ExecutionSpace::memory_space;

	using PairType 			= kPair<idType,idType>;

	using ContainerType 	= unorderedSet<PairType, ExecutionSpace>;

	struct pairAccessor
	{
		using PairType = typename UnsortedPairs::PairType;

		ContainerType 		Container_;

		INLINE_FUNCTION_HD
		int32 size()const { return Container_.size(); }

		INLINE_FUNCTION_HD
		int32 loopCount()const { return Container_.capacity(); }

		INLINE_FUNCTION_HD
		bool isValid(int32 idx)const { return Container_.valid_at(idx); }

		INLINE_FUNCTION_HD
		PairType getPair(int idx)const {	return Container_.key_at(idx); }

		INLINE_FUNCTION_HD
		bool getPair(int32 idx, PairType& pair)const {
			if(Container_.valid_at(idx))	{
				pair = Container_.key_at(idx);
				return true;
			}
			return false;
		}
	};

protected:
	
	ContainerType 	container_;


public:
	
	// - type info
	TypeInfoNV("unsorderedPairs");

	// constructor
	unsortedPairs(int32 capacity=1)
	:
		container_(capacity) // the minimum capacity would be 128 
	{}

	bool beforeBroadSearch()
	{
		container_.clear();
		return true;
	}

	bool afterBroadSearch()
	{
		return true;
	}
	
	// - Device call
	INLINE_FUNCTION_HD
	int32 insert(idType i, idType j)const
	{
		if(auto insertResult = container_.insert(PairType(i,j)); insertResult.failed())
			return -1;
		else
			return insertResult.index();
			
	}

	INLINE_FUNCTION_HD
	int32 insert(const PairType& p)const
	{
		if(auto insertResult = container_.insert(p); insertResult.failed())
			return -1;
		else
			return insertResult.index();

	}

	// - Device call
	//   return the pair at index idx
	//   perform no check for size and existance
	INLINE_FUNCTION_HD
	PairType getPair(int32 idx)const
	{
		return container_.key_at(idx);
	}

	// - Device call
	//   return the pair at index idx
	INLINE_FUNCTION_HD
	bool getPair(int32 idx, PairType& p)const
	{
		if(container_.valid_at(idx))
		{
			p = container_.key_at(idx);
			return true;
		}
		else
		{

			return false;
		}
	}

	INLINE_FUNCTION_HD
	int32 find(const PairType & p)const
	{
		if( auto idx = container_.find(p); 
			idx != Kokkos::UnorderedMapInvalidIndex )
			return idx;
		else
			return -1;
	}

	INLINE_FUNCTION_HD
	bool isValid(int32 idx)const
	{
		return container_.valid_at(idx);	
	}


	INLINE_FUNCTION_HD
	int32 capacity() const
	{
		return container_.capacity();
	}

	int32 loopCount()const
	{
		return container_.capacity();
	}
	
	//use this when the value of size_ is updated
	INLINE_FUNCTION_H
	int32 size()const
	{
		return container_.size();
	}

	pairAccessor getPairs()const
	{
		return {container_};
	}

	/// increase the capacity of the container by at-least len 
	/// the content will be erased. 
	INLINE_FUNCTION_H
	void increaseCapacityBy(int32 len)
	{
		uint newCap = container_.capacity()+len;
		this->clear();
		container_.rehash(newCap);
	}

	INLINE_FUNCTION_H
	void clear()
	{
		container_.clear();
	}

	const ContainerType& container()const
	{
		return container_;
	}

};


}

#endif //__unsortedPairs_hpp__