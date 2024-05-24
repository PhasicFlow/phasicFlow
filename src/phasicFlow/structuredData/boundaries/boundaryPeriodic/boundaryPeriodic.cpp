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
	mirrorBoundaryIndex_(dict.getVal<uint32>("mirrorBoundaryIndex")),
	extensionLength_(dict.getVal<real>("boundaryExtntionLengthRatio"))
{
	extensionLength_ = max(extensionLength_, static_cast<real>(0.1));
}

pFlow::real pFlow::boundaryPeriodic::neighborLength() const
{
    return (1+extensionLength_)*neighborLengthIntoInternal();
}

pFlow::realx3 pFlow::boundaryPeriodic::boundaryExtensionLength() const
{
    return -extensionLength_*neighborLengthIntoInternal()*boundaryBase::boundaryPlane().normal();
}


bool pFlow::boundaryPeriodic::beforeIteration(
	uint32 step,
    uint32 iterNum,
    real t,
    real dt)
{
	if(step!=2)return true;
	// nothing have to be done
	if(empty())
	{
		return true;
	}

	if( !boundaryListUpdate(iterNum))return true;
	
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
	realx3 transferVec = displacementVectroToMirror();
	
	return transferPointsToMirror
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