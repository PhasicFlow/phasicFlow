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

#ifndef __cellsWallLevels_H__
#define __cellsWallLevels_H__

#include "cellsWallLevel0.H"

namespace pFlow
{

template<
	typename executionSpace
	>
class cellsWallLevels
{
public:

	using cellsWallLevel0Type = cellsWallLevel0<executionSpace>;

	using IdType 			= typename cellsWallLevel0Type::IdType;

	using IndexType 	= typename cellsWallLevel0Type::IndexType;

	using Cells 			= typename cellsWallLevel0Type::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space = typename cellsWallLevel0Type::execution_space;

	using memory_space 		= typename cellsWallLevel0Type::memory_space;

	using iBoxType = iBox<IndexType>;

protected:

	int32 			numLevles_=1;
	

	Vector<cellsWallLevel0Type> 			cellsWallLevels_;	

public:

	TypeNameNV("cellsWallLevels");

	FUNCTION_H
	cellsWallLevels(
		int32 					numLevels,
		const Vector<Cells>&	cellsLevels,
		real 					cellExtent,
		int32 					numPoints,
		int32 					numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<int32x3,memory_space>& 	vertices
		)
	:
		numLevles_(numLevels),
		cellsWallLevels_("cellsWallLevels",numLevels, numLevels, RESERVE())
	{

		

		for(int32 lvl=0; lvl<numLevles_; lvl++)
		{
			cellsWallLevels_[lvl] = 
				cellsWallLevel0Type(
					cellsLevels[lvl],
					cellExtent,
					numPoints,
					numElements,
					points,
					vertices);
		}

	}
	
		
	template<typename PairsContainer, typename particleMapType>
	bool broadSearch(PairsContainer& pairs, particleMapType& particleMap)
	{
	
		// map walls onto the cells
		for(int32 lvl=0; lvl<numLevles_; lvl++)
		{
			cellsWallLevels_[lvl].build();
		}
		
		this->particleWallFindPairs(pairs, particleMap);
		
		return true;
	}

	template<typename PairsContainer, typename particleMapType>
	bool particleWallFindPairs(PairsContainer& pairs, particleMapType& particleMap)
	{

		int32 getFull = 1;

		while (getFull)
		{
			getFull = 0;
			for(int32 lvl=0; lvl<numLevles_; lvl++)
			{
				getFull += 
					cellsWallLevels_[lvl].findPairsElementRangeCount(
						pairs, 
						particleMap.getCellIterator(lvl));
			}
			
			if(getFull)
			{
				// - resize the container
				//   note that getFull now shows the number of failed insertions.
				uint32 len = max(getFull, 5000);
				auto oldCap = pairs.capacity();
				pairs.increaseCapacityBy(len);

				Info<<"Contact pair container capacity increased from "<<
						oldCap << " to " 
						<< pairs.capacity() <<" in cellsWallLevels."<<endInfo;
				
				Kokkos::fence();
			}
		}
		
		return true;
	}


}; // cellsWallLevels

} // pFlow


#endif // __cellsWallLevels_H__
