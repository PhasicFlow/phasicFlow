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

#include "boundaryExit.hpp"
#include "internalPoints.hpp"
#include "dictionary.hpp"


pFlow::boundaryExit::boundaryExit
(
	const dictionary& dict,
	const plane&      bplane,
	internalPoints&   internal
)
:
	boundaryBase(dict, bplane, internal)
{
	exitMarginLength_ = max( 
		dict.getValOrSet("exitMarginLength",0.0), 0.0);
	checkForExitInterval_ = max(
		dict.getValOrSet("checkForExitInterval", 1), 1);
}

bool pFlow::boundaryExit::beforeIteration
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	// nothing have to be done
	if(empty())
	{
		return true;
	}

	uint32 s = size();
	deviceViewType1D<uint32> delFlags("delFlags",s+1); 
	deviceViewType1D<uint32> keepFlags("keepFlags", s+1);
	fill(delFlags, 0, s+1, 0u);
	fill(keepFlags, 0, s+1, 0u);

	using policy = Kokkos::RangePolicy<
			pFlow::DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<pFlow::uint32>>;

	auto points = thisPoints();
	uint32 numDeleted = 0;
	auto p = boundaryPlane().infPlane();

	Kokkos::parallel_reduce
	(
		"boundaryExit::beforeIteration",
		policy(0,s),
		LAMBDA_HD(uint32 i, uint32& delToUpdate)
		{
			if(p.pointInNegativeSide(points(i)))
			{
				delFlags(i)=1;
				delToUpdate++;
			}
			else
			{
				keepFlags(i) = 1;
			}
		}, 
		numDeleted
	);
	
	// no point is deleted
	if(numDeleted == 0 )
	{
		return true;
	}

	exclusiveScan(delFlags, 0u, s+1, delFlags, 0u);
	exclusiveScan(keepFlags, 0u, s+1, keepFlags, 0u);
	
	deviceViewType1D<uint32> deleteList("deleteList", numDeleted);

	Kokkos::parallel_for
	(
		"boundaryExit::parllel_for",
		policy(0, size()),
		LAMBDA_HD(uint32 i)
		{
			if(delFlags(i)!= delFlags(i+1))
				deleteList(delFlags(i)) = i;
		}	
	);
	Kokkos::fence();

	deviceScatteredFieldAccess<uint32> deleteIndices(
			numDeleted, 
			deleteList,
			indexList().deviceVectorAll());
	
	// tell internal to remove these points from its list 
	if(!internal().deletePoints(deleteIndices))
	{
		fatalErrorInFunction<<
		"error in deleting points from boundary "<< name()<<endl;
		return false;
	}

	// delete these indices from your list 
	if(numDeleted == s )
	{
		indexList().resize(0u);
	}
	else
	{
		uint newSize = s-numDeleted;
		deviceViewType1D<uint32> newIndices("newIndices", newSize);
		auto oldIndices = indexList().deviceVectorAll();

		Kokkos::parallel_for(
			"fillIndices",
			policy(0,s),
			LAMBDA_HD(uint32 i)
			{
				if(keepFlags(i)!= keepFlags(i+1))
				{
					newIndices(keepFlags(i)) = oldIndices(i);
				}
			}
		);
		Kokkos::fence();
		copy(oldIndices, newIndices);
		indexList().resize(newSize);
	}

	// notify your observers 

	WARNING<<"notify observers in boundary exit "<<END_WARNING;
	
	return true;
}

bool pFlow::boundaryExit::iterate
(
	uint32 iterNum, 
	real t
)
{
	return true;
}

bool pFlow::boundaryExit::afterIteration
(
	uint32 iterNum, 
	real t
)
{
	return true;
}