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

#ifndef __cellsWallLevel0_hpp__
#define __cellsWallLevel0_hpp__

#include "types.hpp"
#include "KokkosTypes.hpp"	
#include "cells.hpp"
#include "iBox.hpp"



namespace pFlow
{

template<
	typename executionSpace
	>
class cellsWallLevel0
:
	public cells<int32>
{
public:

	using IdType		= int32;

	using IndexType 	= int32;

	using Cells 		= cells<IndexType>;

	using CellType 		= typename Cells::CellType;

	using execution_space = executionSpace;

	using memory_space 	= typename execution_space::memory_space;

	using iBoxType 		= iBox<IndexType>;	

	class TagFindCellRange2{};

protected:

	// - box extent
	real 			cellExtent_ = 0.5;

	// - number of triangle elements 
	int32			numElements_ = 0;

	// - number of points
	int32 			numPoints_ = 0;

	// - ref to vectices (borrowed)
	ViewType1D<int32x3, memory_space> 	vertices_;

	// - ref to points in the trisurface (borrowed)
	ViewType1D<realx3, memory_space> 	points_;

	// cell range of element/triangle bounding box
	ViewType1D<iBoxType, memory_space> 	elementBox_;
	

	using tpPWContactSearch = Kokkos::TeamPolicy<
			execution_space,
			Kokkos::Schedule<Kokkos::Dynamic>,
			Kokkos::IndexType<int32>
			>;

	using rpFindCellRange2Type =
		Kokkos::RangePolicy<TagFindCellRange2, execution_space, Kokkos::IndexType<int32>>;

	
	FUNCTION_H
	void allocateArrays()
	{
		reallocNoInit( elementBox_, numElements_);
	}

public:

	TypeInfoNV("cellsWallLevel0");

	INLINE_FUNCTION_HD
	cellsWallLevel0(){}

	FUNCTION_H
	cellsWallLevel0(
		const Cells& 		ppCells,
		real 						cellExtent,
		int32 					numPoints,
		int32 					numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<int32x3,memory_space>& 	vertices
		)
	:
		Cells(ppCells),
		cellExtent_( max(cellExtent, 0.5 ) ),
		numElements_(numElements),
		numPoints_(numPoints),
		vertices_(vertices),
		points_(points)
	{

		allocateArrays();
	}

	
	// - host call
	//   reset triangle elements if they have changed
	bool resetElements( 
		int32 			numElements,
		int32       numPoints,
		ViewType1D<realx3, memory_space>& points, 
		ViewType1D<int32x3, memory_space>& vertices	)
	{

		numElements_ = numElements;
		numPoints_ = numPoints;
		points_ 	= points;
		vertices_ 	= vertices;
		
		allocateArrays();

		return true;
	}

	INLINE_FUNCTION_HD
	iBoxType elementBox(int32 i)const
	{
		return elementBox_[i];
	}

	INLINE_FUNCTION_HD
	int32 numElements()const
	{
		return numElements_;
	}

	
		
	template<typename PairsContainer, typename particleMapType>
	bool broadSearch(PairsContainer& pairs, particleMapType& particleMap)
	{
	
		// map walls onto the cells
		this->build();
		
		this->particleWallFindPairs(pairs, particleMap);
		
		return true;
	}

	bool build()
	{
		Kokkos::parallel_for(
			"cellsSimple::findcellrange2",
			rpFindCellRange2Type(0,numElements_),
			*this);
		Kokkos::fence();
		return true;
	}

	template<typename PairsContainer, typename particleMapType>
	bool particleWallFindPairs(PairsContainer& pairs, particleMapType& particleMap)
	{

		int32 getFull = 1;

		while (getFull)
		{
			
			getFull = findPairsElementRangeCount(pairs, particleMap.getCellIterator(0));
			
			if(getFull)
			{
				// - resize the container
				//   note that getFull now shows the number of failed insertions.
				uint32 len = max(getFull, 50);
				auto oldCap = pairs.capacity();
				pairs.increaseCapacityBy(len);

				Info<<"Contact pair container capacity increased from "<<
						oldCap << " to " 
						<< pairs.capacity() <<" in cellsWallLevel0."<<endInfo;
				
				Kokkos::fence();
			}
		}
		
		return true;
	}


	template<typename PairsContainer, typename CellIteratorType>
	int32 findPairsElementRangeCount(PairsContainer& pairs, CellIteratorType cellIter) 
	{
		int32 getFull =0;
		
		const auto pwPairs = pairs;
		const auto elementBox = elementBox_;
		
		Kokkos::parallel_reduce(
			"cellsSimple::findPairsElementRangeModified2",
			tpPWContactSearch(numElements_, Kokkos::AUTO),
			LAMBDA_HD(
				const typename tpPWContactSearch::member_type & teamMember,
				int32& valueToUpdate){
				
				const int32 iTri = teamMember.league_rank();

				const auto triBox = elementBox[iTri];
				
				int32 getFull2 = 0;

				auto bExtent = boxExtent(triBox);
				int32 numCellBox = bExtent.x()*bExtent.y()*bExtent.z();

				Kokkos::parallel_reduce( 
					Kokkos::TeamThreadRange( teamMember, numCellBox ),
					[&] ( const int32 linIndex, int32 &innerUpdate )
						{
							
							CellType cell;
							indexToCell(linIndex, triBox, cell);

							int32 n = cellIter.start(cell.x(),cell.y(),cell.z());
						
							while( n>-1)
							{
								// id is wall id the pair is (particle id, wall id)
								if( pairs.insert(static_cast<IdType>(n), iTri) < 0 )
									innerUpdate++;
								n = cellIter.getNext(n);
							}

						},
					getFull2
				);

				if ( teamMember.team_rank() == 0 ) valueToUpdate += getFull2;
			},
			getFull
			);

		return getFull;
	}

	INLINE_FUNCTION_HD
	void operator()(TagFindCellRange2, int32 i) const
	{
		auto v = vertices_[i];
		auto p1 = points_[v.x()];
		auto p2 = points_[v.y()];
		auto p3 = points_[v.z()];
		
		realx3 minP, maxP;

		this->extendBox(p1, p2, p3, cellExtent_, minP, maxP);
		elementBox_[i] = iBoxType(this->pointIndex(minP), this->pointIndex(maxP));

	}

}; // cellsWallLevel0

} // pFlow


#endif // __cellsWallLevel0_hpp__
