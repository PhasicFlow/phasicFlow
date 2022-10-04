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

#ifndef __cellsSimple_H__
#define __cellsSimple_H__

#include "types.H"
#include "KokkosTypes.H"	
#include "cells.H"
#include "iBox.H"
#include "dictionary.H"


namespace pFlow
{

template<
	typename executionSpace,
	typename idType,
	typename indexType = int32
	>
class cellsSimple
:
	public cells<indexType>
{
public:

	using IdType 		= idType;

	using IndexType 	= indexType;

	using Cells 		= cells<IndexType>;

	using CellType 		= typename Cells::CellType;

	using ExecutionSpace = executionSpace;

	using memory_space 	= typename ExecutionSpace::memory_space;

	using iBoxType = iBox<IndexType>;	

	bool constexpr static LOOP_ELEMENT_RANGE = true;
	
	class TagFindCellRange2{};
protected:

	// - box extent
	real 			cellExtent_ = 0.6;

	// - update frequency 
	int32 			updateFrequency_=1;


	int32 			currentIter_ = 0;

	// - number of triangle elements 
	int32			numElements_ = 0;

	// - number of points
	int32 			numPoints_ = 0;

	/// a broad search has been occured during last pass?
	bool 			performedSearch_ = false;

	// - ref to vectices 
	ViewType1D<int32x3, memory_space> 	vertices_;

	// - ref to points in the trisurface 
	ViewType1D<realx3, memory_space> 	points_;

	// cell range of element/triangle bounding box
	ViewType1D<iBoxType, memory_space> 	elementBox_;
	

	using tpPWContactSearch = Kokkos::TeamPolicy<
			ExecutionSpace,
			Kokkos::Schedule<Kokkos::Dynamic>,
			Kokkos::IndexType<int32>
			>;

	using rpFindCellRange2Type =
		Kokkos::RangePolicy<TagFindCellRange2, ExecutionSpace, Kokkos::IndexType<int32>>;

	
	FUNCTION_H
	void allocateArrays()
	{
		Kokkos::realloc( elementBox_, numElements_);
	}

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

	TypeNameNV("cellsSimple");

	FUNCTION_H
	cellsSimple(
		Cells 		ppCells,
		real 		cellExtent,
		int32 		numPoints,
		int32 		numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<int32x3,memory_space>& 	vertices
		)
	:
		Cells(ppCells),
		cellExtent_( max(cellExtent, 0.6 ) ),
		numElements_(numElements),
		numPoints_(numPoints),
		vertices_(vertices),
		points_(points)
	{

		allocateArrays();
	}

	cellsSimple(
		dictionary  dict,
		Cells 		ppCells,
		int32 		numPoints,
		int32 		numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<int32x3,memory_space>& 	vertices
		)
	:
		Cells(ppCells),
		numElements_(numElements),
		numPoints_(numPoints),
		vertices_(vertices),
		points_(points)
	{

		updateFrequency_ = dict.getVal<int32>(
			"updateFrequency" );
		updateFrequency_ = max(updateFrequency_,1);

		cellExtent_ = dict.getVal<real>(
			"cellExtent");

		cellExtent_ = max(cellExtent_,0.6);

		allocateArrays();
		
	}

	constexpr bool loopElementRange()const
	{
		return LOOP_ELEMENT_RANGE;
	}

	// - host call
	//   reset triangle elements if they have changed
	FUNCTION_H
	bool resetElements( 
		int32 		numElements,
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
	
		// map walls onto the cells
		this->build();
		
		this->particleWallFindPairs(pairs, particleMap);
		
		performedSearch_ = true;
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
			
			getFull = findPairsElementRangeModified(pairs, particleMap);
			
			if(getFull)
			{
				// - resize the container
				//   note that getFull now shows the number of failed insertions.
				uint32 len = max(getFull, 50);
				auto oldCap = pairs.capacity();
				pairs.increaseCapacityBy(len);

				Info<<"Contact pair container capacity increased from "<<
						oldCap << " to " 
						<< pairs.capacity() <<" in cellsSimple."<<endInfo;
				
				
				
				Kokkos::fence();
			}
		}
		
		return true;
	}

	/*template<typename PairsContainer, typename particleMapType>
	int32 findPairsElementRange(PairsContainer& pairs, particleMapType& particleMap) 
	{
		int32 getFull =0;
		
		const auto pwPairs = pairs;
		const auto elementBox = elementBox_;

		
		Kokkos::parallel_reduce(
			"cellsSimple::findPairsElementRange",
			tpPWContactSearch(numElements_, Kokkos::AUTO),
			LAMBDA_HD(
				const typename tpPWContactSearch::member_type & teamMember,
				int32& valueToUpdate){
				int32 i  = teamMember.league_rank();
				printf("league size %d , league rank %d , team size %d and team rank %d \n", teamMember.league_size(), i, teamMember.team_size(), teamMember.team_rank());
				IdType id = i;
				const auto triBox = elementBox[i];
				
				valueToUpdate +=
					particleMap.addPointsInBoxToList(
						teamMember,
						id,
						triBox,
						pwPairs
						);
			},
			getFull
			);

		return getFull;
	}*/

	template<typename PairsContainer, typename particleMapType>
	int32 findPairsElementRangeModified(PairsContainer& pairs, particleMapType& particleMap)
	{
		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Dynamic>,
			ExecutionSpace> rPolicy(0,numElements_);

		int32 getFull =0;
		
		const auto pwPairs = pairs;
		const auto elementBox = elementBox_;
		//printf("we are in modified version \n");
		Kokkos::parallel_reduce(
			"cellsSimple::findPairsElementRangeModified",
			rPolicy,
			LAMBDA_HD(
				int32 i,
				int32& valueToUpdate){
				
				IdType id = i;
				const auto triBox = elementBox[i];
				
				valueToUpdate +=
					particleMap.addPointsInBoxToListModified(
						id,
						triBox,
						pwPairs
						);
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
		//output<< minP << " maxP "<< maxP<<endl;
		elementBox_[i] = iBoxType(this->pointIndex(minP), this->pointIndex(maxP));

	}

};

}


#endif 
