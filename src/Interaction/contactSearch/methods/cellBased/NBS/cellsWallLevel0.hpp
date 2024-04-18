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

#include "contactSearchGlobals.hpp"
#include "contactSearchFunctions.hpp"
#include "mapperNBS.hpp"
#include "iBox.hpp"



namespace pFlow
{

class cellsWallLevel0
{
public:

	using execution_space = csExecutionSpace;

	using memory_space 	= typename execution_space::memory_space;

	using iBoxType 		= iBox<int32>;	

	class TagFindCellRange2{};

private:

	// - box extent
	real 			cellExtent_ = 0.5;

	// - number of triangle elements 
	uint32			numElements_ = 0;

	// - number of points
	uint32 			numPoints_ = 0;

	// - ref to vectices (borrowed)
	ViewType1D<uint32x3, memory_space> 	vertices_;

	// - ref to points in the trisurface (borrowed)
	ViewType1D<realx3, memory_space> 	points_;

	// - ref to normal vectors of triangles (borrowed)
	ViewType1D<realx3, memory_space> 	normals_;

	// cell range of element/triangle bounding box
	ViewType1D<iBoxType, memory_space> 	elementBox_;
	

	using tpPWContactSearch = Kokkos::TeamPolicy<
			execution_space,
			Kokkos::Schedule<Kokkos::Dynamic>,
			Kokkos::IndexType<uint32>>;
	
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
		real 					cellExtent,
		uint32 					numPoints,
		uint32 					numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<uint32x3,memory_space>& 	vertices,
		const ViewType1D<realx3, memory_space>& normals);
	

	
	// - host call
	//   reset triangle elements if they have changed
	bool resetElements( 
		uint32 		numElements,
		uint32      numPoints,
		const ViewType1D<realx3, memory_space>& points, 
		const ViewType1D<uint32x3, memory_space>& vertices,
		const ViewType1D<realx3, memory_space>& normals);
	

	INLINE_FUNCTION_HD
	iBoxType elementBox(uint32 i)const
	{
		return elementBox_[i];
	}

	INLINE_FUNCTION_HD
	uint32 numElements()const
	{
		return numElements_;
	}

	bool broadSearch(
		csPairContainerType& pairs,
		const cells& searchBox, 
		const mapperNBS::CellIterator& particleMap,
		const deviceViewType1D<realx3>& pPoints,
		const deviceViewType1D<real>&  pDiams,
		real sizeRatio);
	

	bool build(const cells& searchBox);
	
	bool particleWallFindPairs(
		csPairContainerType& pairs, 
		const mapperNBS::CellIterator& particleMap,
		const deviceViewType1D<realx3>& pPoints,
		const deviceViewType1D<real>&  pDiams,
		real sizeRatio);

	int32 findPairsElementRangeCount(
		csPairContainerType& pairs, 
		const mapperNBS::CellIterator& particleMap,
		const deviceViewType1D<realx3>& pPoints,
		const deviceViewType1D<real>&  pDiams,
		real sizeRatio); 
	


}; // cellsWallLevel0

} // pFlow


#endif // __cellsWallLevel0_hpp__
