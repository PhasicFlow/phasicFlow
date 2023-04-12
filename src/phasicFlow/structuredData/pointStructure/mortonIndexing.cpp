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

#include "mortonIndexing.hpp"
#include "cells.hpp"


bool pFlow::getSortedIndex(
	box boundingBox,
	real dx,
	range activeRange, 
	ViewType1D<realx3> pos, 
	ViewType1D<int8> flag,
	int32IndexContainer& sortedIndex)
{

	// obtain the morton code of the particles
	cells<size_t> allCells( boundingBox, dx);
	int32IndexContainer index(activeRange.first, activeRange.second);

	ViewType1D<uint64_t> mortonCode("mortonCode", activeRange.second);

	using rpMorton = 
		Kokkos::RangePolicy<Kokkos::IndexType<int32>>;
	int32 numActive = 0;
	Kokkos::parallel_reduce
	(
		"mortonIndexing::getIndex::morton",
		rpMorton(activeRange.first, activeRange.second),
		LAMBDA_HD(int32 i, int32& sumToUpdate){
			if( flag[i] == 1 ) // active point 
			{
				auto cellInd = allCells.pointIndex(pos[i]);
				mortonCode[i] = xyzToMortonCode64(cellInd.x(), cellInd.y(), cellInd.z());
				sumToUpdate++;
			}else
			{
				mortonCode[i] = xyzToMortonCode64(-1,-1,-1);
			}
		},
		numActive
	);
	

	permuteSort(
		mortonCode, 
		activeRange.first, 
		activeRange.second,
		index.deviceView(),
		0 );
	index.modifyOnDevice();
	index.setSize(numActive);
	index.syncViews();

	sortedIndex = index;

	return true;
}