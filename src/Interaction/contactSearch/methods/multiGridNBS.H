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


#ifndef __multiGridNBS_H__
#define __multiGridNBS_H__

#include "NBSLevels.H"

namespace pFlow
{


template<typename executionSpace>
class multiGridNBS
{
public:

	using NBSLevelsType	= NBSLevels<executionSpace>;

	using cellIterator 	= typename NBSLevelsType::cellIterator;

	using IdType 		= typename NBSLevelsType::IdType;
	
	using IndexType 	= typename NBSLevelsType::IndexType;

	using Cells 		= typename NBSLevelsType::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space 	= typename NBSLevelsType::execution_space;

	using memory_space 		= typename NBSLevelsType::memory_space;


protected:

	real 		sizeRatio_ = 1.0;

	int32 		updateFrequency_= 1;

	int32 		currentIter_ 	= 0;

	bool 		performedSearch_ = false;

	NBSLevelsType NBSLevels_;

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

	TypeNameNV("multiGridNBS");

		multiGridNBS(
			const 	dictionary& dict,
			const 	box& domain,
			real 	minSize,
			real 	maxSize,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam)
		:
			sizeRatio_( 
				max(
					dict.getVal<real>("sizeRatio"),
					1.0
					)),
			updateFrequency_(
				max( 
					dict.getVal<int32>("updateFrequency"),
					1
					)),
			NBSLevels_(
				domain, 
				minSize,
				maxSize,
				sizeRatio_,
				position,
				diam)
		{}
			
		INLINE_FUNCTION_HD
		multiGridNBS(const multiGridNBS&) = default;

		INLINE_FUNCTION_HD
		multiGridNBS& operator = (const multiGridNBS&) = default;

		INLINE_FUNCTION_HD
		~multiGridNBS()=default;

	//// - Methods 

	bool enterBoadSearch()const
	{
		return currentIter_%updateFrequency_==0;
	}

	bool performedSearch()const
	{
		return performedSearch_;
	}

	int32 numLevels()const
	{
		return NBSLevels_.numLevels();
	}

	auto getCellsLevels()const
	{
		Vector<Cells> cellsLvl("cells", numLevels(), numLevels(), RESERVE());

		for(int32 lvl=0; lvl<numLevels(); lvl++)
		{
			cellsLvl[lvl] = NBSLevels_.getCells(lvl) ;
		}

		return cellsLvl;
	}

	auto getCells(int32 lvl)const
	{
		return NBSLevels_.getCells(lvl);
	}

	auto getCellIterator(int32 lvl)const
	{
		return NBSLevels_.getCellIterator(lvl);	
	}

	bool objectSizeChanged(int32 newSize)
	{
		NBSLevels_.checkAllocateNext(newSize);
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
		
		
		NBSLevels_.build(activeRange);
		
		NBSLevels_.findPairs(pairs);
		
		
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

		NBSLevels_.build(activeRange, incld);
	
		NBSLevels_.findPairs(pairs);
		
		performedSearch_ = true;
		return true;
	}

};

}

#endif
