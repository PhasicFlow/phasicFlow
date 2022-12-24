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

#ifndef __multiGridMapping_hpp__
#define __multiGridMapping_hpp__

#include "cellsWallLevels.hpp"
#include "dictionary.hpp"


namespace pFlow
{

template<
	typename executionSpace
	>
class multiGridMapping
{
public:

	using CellsWallLevelType = cellsWallLevels<executionSpace>;

	using IdType 			= typename CellsWallLevelType::IdType;

	using IndexType 	= typename CellsWallLevelType::IndexType;

	using Cells 			= typename CellsWallLevelType::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space = typename CellsWallLevelType::execution_space;

	using memory_space 		= typename CellsWallLevelType::memory_space;

	using iBoxType = iBox<IndexType>;	

	
protected:

	// - update frequency 
	int32 			updateFrequency_=1;

	real 				cellExtent_;

	int32 			currentIter_ = 0;

	/// a broad search has been occured during last pass?
	bool 				performedSearch_ = false;

	CellsWallLevelType  cellsWallLevle_;
		
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

	TypeInfoNV("multiGridMapping");

	multiGridMapping(
		const dictionary&  			dict,
		int32 					numLevels,
		const Vector<Cells>& 	ppCells,
		int32 					numPoints,
		int32 					numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<int32x3,memory_space>& 	vertices
		)
	:
		updateFrequency_(
			max(
				dict.getVal<int32>("updateFrequency"),
				1)),
		cellExtent_(
			max(
				dict.getVal<real>("cellExtent"),
				0.5)),
		cellsWallLevle_(
			numLevels,
			ppCells,
			cellExtent_,
			numPoints,
			numElements,
			points,
			vertices
			)
	{

		REPORT(3)<<"Multi-grid wall mapping with "<< 
			yellowText(numLevels)<<" levels has been created."<<endREPORT;
	}


	bool enterBoadSearch()const
	{
		return currentIter_%updateFrequency_==0;
	}

	bool performedSearch()const
	{
		return performedSearch_;
	}
		
	template<typename PairsContainer, typename particleMapType>
	bool broadSearch(PairsContainer& pairs, particleMapType& particleMap, bool force=false)
	{
		if(force) currentIter_ = 0;
		performedSearch_= false;
		if(!performSearch())return true;


		cellsWallLevle_.broadSearch(pairs, particleMap);


		performedSearch_ = true;
		return true;
	}

}; // multiGridMapping

} // pFlow


#endif 
