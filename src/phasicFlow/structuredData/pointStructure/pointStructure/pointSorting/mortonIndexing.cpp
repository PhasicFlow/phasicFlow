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


pFlow::uint32IndexContainer pFlow::getSortedIndices(
	box boundingBox,
	real dx,  
	const ViewType1D<realx3>& pos, 
	const pFlagTypeDevice& flag
)
{
	if(flag.numActive() == 0u)return uint32IndexContainer();

	// obtain the morton code of the particles
	cells allCells( boundingBox, dx);
	auto aRange = flag.activeRange();

	uint32IndexContainer sortedIndex(aRange.start(), aRange.end());

	ViewType1D<uint64_t> mortonCode("mortonCode", aRange.end());


	Kokkos::parallel_for
	(
		"mortonIndexing::getIndex::morton",
		deviceRPolicyStatic(aRange.start(), aRange.end()),
		LAMBDA_HD(uint32 i){
			if( flag.isActive(i)) // active point 
			{
				auto cellInd = allCells.pointIndex(pos[i]);
				mortonCode[i] = xyzToMortonCode64(cellInd.x(), cellInd.y(), cellInd.z());
			}else
			{
				mortonCode[i] = xyzToMortonCode64
				(
					largestPosInt32,
					largestPosInt32,
					largestPosInt32
				);
			}
		}
	);
	
	Kokkos::fence();

	permuteSort(
		mortonCode, 
		aRange.start(), 
		aRange.end(),
		sortedIndex.deviceView(),
		0 );

	sortedIndex.modifyOnDevice();
	sortedIndex.syncViews(flag.numActive());

	return sortedIndex;
}