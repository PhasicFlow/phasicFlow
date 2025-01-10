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

#ifndef __wallBoundaryContactSearch_hpp__
#define __wallBoundaryContactSearch_hpp__


#include "contactSearchGlobals.hpp"
#include "contactSearchFunctions.hpp"
#include "scatteredFieldAccess.hpp"
#include "iBox.hpp"
#include "cells.hpp"



namespace pFlow
{

class wallBoundaryContactSearch
{
public:

	using execution_space = csExecutionSpace;

	using memory_space 	= typename execution_space::memory_space;

	using iBoxType 		= iBox<int32>;	

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

    ViewType1D<uint8, memory_space>     validBox_;
	
	
	FUNCTION_H
	void allocateArrays()
	{
		reallocNoInit( elementBox_, numElements_);
        reallocNoInit( validBox_, numElements_);
	}

public:

	TypeInfoNV("wallBoundaryContactSearch");

	INLINE_FUNCTION_HD
	wallBoundaryContactSearch()=default;

	FUNCTION_H
	wallBoundaryContactSearch(
		real 					cellExtent,
		uint32 					numPoints,
		uint32 					numElements,
		const ViewType1D<realx3,memory_space>& 		points, 
		const ViewType1D<uint32x3,memory_space>& 	vertices,
		const ViewType1D<realx3, memory_space>& normals);
	


	INLINE_FUNCTION_HD
	uint32 numElements()const
	{
		return numElements_;
	}

	
	bool build(const cells& searchBox, const realx3& transferVec);
	
	bool broadSearch(
		csPairContainerType& pairs, 
		const cells &searchCells, 
		const deviceScatteredFieldAccess<realx3>& thisPoints,
		const deviceScatteredFieldAccess<real>&  thisDiams,
        const deviceScatteredFieldAccess<realx3>& mirrorPoints,
		const deviceScatteredFieldAccess<real>&  mirroDiams,
		const realx3& transferVec,
		real sizeRatio);

	uint32 findPairsElementRangeCount(
		csPairContainerType &pairs, 
        const cells &searchCells, 
        const deviceScatteredFieldAccess<realx3> &pPoints, 
        const deviceScatteredFieldAccess<real> &pDiams, 
        const realx3 &transferVec, 
        uint  baseTriIndex);
	


}; // wallBoundaryContactSearch

} // pFlow


#endif // __wallBoundaryContactSearch_hpp__
