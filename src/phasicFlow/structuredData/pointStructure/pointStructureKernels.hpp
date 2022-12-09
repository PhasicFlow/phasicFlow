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


#ifndef __pointStructureKernels_hpp__
#define __pointStructureKernels_hpp__

#include "pointStructure.hpp"
#include "box.hpp"

namespace pFlow::pointStructureKernels
{


int32 markDeleteOutOfBox(
	box 		domain,
	int32 		start,
	int32 		end,
	int8 		deleteFlag,
	deviceViewType1D<realx3> 	points,
	deviceViewType1D<int8> 		flags,
	pointStructure::activePointsDevice activePoint,
	int32 & minRange,
	int32 & maxRange
	)
{
	
	using rpMark = 
		Kokkos::RangePolicy<Kokkos::IndexType<int32>>;

	int32 numMarked = 0;
	int32 minR = start-1, maxR = end+1;

	if(start<end)
	{
		
		Kokkos::parallel_reduce(
			"pointStructureKernels::markDeleteOutOfBox",
			rpMark(start,end),
			LAMBDA_HD(int32 i, int32& minUpdate, int32& maxUpdate, int32& valToUpdate){
				if(activePoint(i))
				{
					if( !domain.isInside(points[i]) )
					{
						flags[i] = deleteFlag;
						valToUpdate++;
					}
					else
					{
						minUpdate = min(minUpdate,i);
						maxUpdate = max(maxUpdate,i);
					}
				}
			},
			Kokkos::Min<int32>(minR), 
			Kokkos::Max<int32>(maxR),
			numMarked);

	}
	
	// means either range was empty or all points have been deleted.
	if(minR<start || maxR>end)
	{
		minRange = 0;
		maxRange = 0;
	}
	else
	{
		minRange = minR;
		maxRange = maxR+1; // add one to make it half
	}
	return numMarked;
}

int32 scanPointFlag(
	int32 start,
	int32 end,
	int8 		activeFlag,
	deviceViewType1D<int8> 		flags,
	int32 & minRange,
	int32 & maxRange
	)
{
	
	using rpScanFlag = 
		Kokkos::RangePolicy<Kokkos::IndexType<int32>>;

	int32 numActive = 0;

	if(start < end )
		Kokkos::parallel_reduce(
			"pointStructureKernels::scanPointFlag",
			rpScanFlag(start, end),
			LAMBDA_HD(
				int32 i, 
				int32& minUpdate, 
				int32& maxUpdate, 
				int32& sumToUpdate){
				if(flags[i] == activeFlag)
				{
					sumToUpdate++;
					minUpdate = min(minUpdate,i);
					maxUpdate = max(maxUpdate,i);
				}
			},
			Kokkos::Min<int32>(minRange), 
			Kokkos::Max<int32>(maxRange),
			numActive);

	if(numActive==0)
	{
		minRange = 0;
		maxRange = 0;
	}
	else
	{
		// add one to maxRange to make it half-open
		maxRange ++;
	}

	return numActive;
}

}

#endif

