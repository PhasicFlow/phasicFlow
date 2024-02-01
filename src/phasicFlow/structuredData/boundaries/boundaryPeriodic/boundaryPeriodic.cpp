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

#include "boundaryPeriodic.hpp"
#include "internalPoints.hpp"
#include "dictionary.hpp"


pFlow::boundaryPeriodic::boundaryPeriodic
(
	const dictionary& dict,
	const plane&      bplane,
	internalPoints&   internal
)
:
	boundaryBase(dict, bplane, internal),
	mirrorBoundaryIndex_(dict.getVal<uint32>("mirrorBoundaryIndex"))
{
	extendedPlane_ = boundaryBase::boundaryPlane().parallelPlane(-boundaryBase::neighborLength());
}

pFlow::real pFlow::boundaryPeriodic::neighborLength() const
{
    return 2.0*boundaryBase::neighborLength();
}

pFlow::realx3 pFlow::boundaryPeriodic::boundaryExtensionLength() const
{
    return -neighborLength()*boundaryBase::boundaryPlane().normal();
}

const pFlow::plane &pFlow::boundaryPeriodic::boundaryPlane() const
{
    return extendedPlane_;
}

bool pFlow::boundaryPeriodic::beforeIteration(
    uint32 iterNum,
    real t,
    real dt)
{
	// nothing have to be done
	if(empty())
	{
		return true;
	}

	uint32 s = size();
	deviceViewType1D<uint32> transferFlags("transferFlags",s+1); 
	fill(transferFlags, 0, s+1, 0u);
	
	auto points = thisPoints();
	auto p = boundaryPlane().infPlane();
	uint32 numTransfered = 0;

	Kokkos::parallel_reduce
	(
		"boundaryPeriodic::beforeIteration",
		deviceRPolicyStatic(0u,s),
		LAMBDA_HD(uint32 i, uint32& trnasToUpdate)
		{
			if(p.pointInNegativeSide(points(i)))
			{
				transferFlags(i)=1;
				trnasToUpdate++;
			}
		}, 
		numTransfered
	);
	
	// no point to be transfered 
	if(numTransfered == 0 )
	{
		return true;
	}
	
	// to obtain the transfer vector 
	const auto& thisP = boundaryPlane();
	const auto& mirrorP = internal().boundary(mirrorBoundaryIndex_).boundaryPlane();
	realx3 transferVec = thisP.normal()*(thisP.d() + mirrorP.d());

	return transferPoints
	(
		numTransfered,
		transferFlags, 
		mirrorBoundaryIndex_, 
		transferVec
	);

}

bool pFlow::boundaryPeriodic::iterate
(
	uint32 iterNum, 
	real t
)
{
	return true;
}

bool pFlow::boundaryPeriodic::afterIteration
(
	uint32 iterNum, 
	real t
)
{
	return true;
}