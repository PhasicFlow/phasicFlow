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
	const dictionary &dict,
    const plane 	&bplane,
    internalPoints 	&internal,
	boundaryList	&bndrs,
	uint32 			thisIndex
)
:
	boundaryBase(dict, bplane, internal, bndrs, thisIndex),
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
	uint32Vector_D transferFlags("transferFlags",s+1, s+1, RESERVE()); 
	transferFlags.fill(0u);
	
	auto points = thisPoints();
	auto p = boundaryPlane().infPlane();
	const auto & transferD = transferFlags.deviceViewAll();
	uint32 numTransfered = 0;

	Kokkos::parallel_reduce
	(
		"boundaryPeriodic::beforeIteration",
		deviceRPolicyStatic(0u,s),
		LAMBDA_HD(uint32 i, uint32& trnasToUpdate)
		{
			if(p.pointInNegativeSide(points(i)))
			{
				transferD(i)=1;
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
	const auto& mirrorP = mirrorBoundary().boundaryPlane();
	realx3 transferVec = thisP.normal()*(thisP.d() + mirrorP.d());

	return transferPoints
	(
		numTransfered,
		transferFlags, 
		mirrorBoundaryIndex(), 
		transferVec
	);
}

bool pFlow::boundaryPeriodic::iterate
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	return true;
}

bool pFlow::boundaryPeriodic::afterIteration
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	return true;
}