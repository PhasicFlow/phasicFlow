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


#ifndef __NBS_H__
#define __NBS_H__

#include "NBSLevel0.H"

namespace pFlow
{


template<typename executionSpace>
class NBS
{
public:

	using NBSLevel0Type	= NBSLevel0<executionSpace>;

	using IdType 		= typename NBSLevel0Type::IdType;
	
	using IndexType 	= typename NBSLevel0Type::IndexType;

	using Cells 		= typename NBSLevel0Type::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space 	= typename NBSLevel0Type::execution_space;

	using memory_space 		= typename NBSLevel0Type::memory_space;


protected:

	real 		sizeRatio_ = 1.0;

	int32 		updateFrequency_= 1;

	int32 		currentIter_ 	= 0;

	bool 		performedSearch_ = false;

	NBSLevel0Type NBSLevel0_;

private:

	bool performSearch()
	{
		if(currentIter_ % updateFrequency_ == 0)
		{
			currentIter_++;
			return true;

		}else
		{
			currentIter_++;
			return false;
		}
	}

public:

	TypeNameNV("NBS");

		NBS(
			dictionary dict,
			const 	box& domain,
			real 	maxSize,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam)
		:
			sizeRatio_( 
				max(
					dict.getValOrSet<real>("sizeRatio", 1.0),
					1.0
					)),
			updateFrequency_(
				max( 
					dict.getValOrSet<int32>("updateFrequency", 1),
					1
					)),
			NBSLevel0_(
				domain, 
				maxSize*sizeRatio_,
				sizeRatio_,
				position,
				diam)
		{}
			
		INLINE_FUNCTION_HD
		NBS(const NBS&) = default;

		INLINE_FUNCTION_HD
		NBS& operator = (const NBS&) = default;

		INLINE_FUNCTION_HD
		~NBS()=default;

	//// - Methods 

	bool enterBoadSearch()const
	{
		return currentIter_%updateFrequency_==0;
	}

	bool performedSearch()const
	{
		return performedSearch_;
	}

	auto getCellIterator()const
	{
		return NBSLevel0_.getCellIterator();
	}

	auto getCells()const
	{
		return NBSLevel0_.getCells();
	}

	bool objectSizeChanged(int32 newSize)
	{
		NBSLevel0_.checkAllocateNext(newSize);
		return true;
	}

	// - Perform the broad search to find pairs
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	//   on all the points in the range of [0,numPoints_)
	template<typename PairsContainer>
	bool broadSearch(PairsContainer& pairs, range activeRange, bool force=false)
	{
		
		if(force) currentIter_ = 0;
		performedSearch_ = false;

		if( !performSearch() ) return true;
		
		
		NBSLevel0_.build(activeRange);
		
		NBSLevel0_.findPairs(pairs);
		
		performedSearch_ = true;		
		return true;
	}

	// - Perform the broad search to find pairs,
	//   ignore particles with incld(i) = true,
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	template<typename PairsContainer, typename IncludeFunction>
	bool broadSearch(PairsContainer& pairs, range activeRange, IncludeFunction incld, bool force = false)
	{
		if(force) currentIter_ = 0;
		performedSearch_ = false;

		if( !performSearch() ) return true;

		NBSLevel0_.build(activeRange, incld);
	
		NBSLevel0_.findPairs(pairs);
		
		performedSearch_ = true;
		return true;
	}

};

}

#endif
