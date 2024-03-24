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

#include "mapperNBS.hpp"
#include "contactSearchGlobals.hpp"

#include "streams.hpp"

namespace pFlow::NBSLevel0Kernels
{

using mdrPolicyFindPairs = 
		Kokkos::MDRangePolicy<
			Kokkos::IndexType<uint32>,
			Kokkos::Rank<3>,
			Kokkos::Schedule<Kokkos::Dynamic>,
			DefaultExecutionSpace>;

template<typename T>
INLINE_FUNCTION_HD
void Swap(T& x, T& y)
{
	T tmp = x;
	x = y;
	y = tmp;
}

INLINE_FUNCTION_HD
bool sphereSphereCheck(const realx3& p1, const realx3 p2, real d1, real d2)
{
	return length(p2-p1) < 0.5*(d2+d1);
}

inline
uint32 findPairsCount
(
    csPairContainerType& pairs,
    real 	sizeRatio,
    const deviceViewType1D<realx3>& pointPos,
    const pFlagTypeDevice& flags,
    const deviceViewType1D<real>& diameter,
    mapperNBS::CellIterator cellIter
)
{

	auto nCells = cellIter.numCells();
	
	mdrPolicyFindPairs
			mdrPolicy(
				{0,0,0},
				{nCells.x(), nCells.y(), nCells.z()} );

	uint32 notInsertedPairs = 0u;
	
		
	Kokkos::parallel_reduce	(
		"pFlow::NBSLevel0Kernels::findPairsCount",
		mdrPolicy,
		LAMBDA_HD(uint32 i, uint32 j, uint32 k, uint32& getFullUpdate){
			#include "NBSLoop.hpp"
		}, notInsertedPairs);
		
	return notInsertedPairs;
}

}