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

#include "periodicBoundarySIKernels.hpp"

template <typename cFM, typename gMM>
pFlow::periodicBoundaryGrainInteraction<cFM, gMM>::periodicBoundaryGrainInteraction(
	const boundaryBase &boundary,
	const grainParticles &grnPrtcls,
	const GeometryMotionModel &geomMotion)
	: boundaryGrainInteraction<cFM,gMM>(boundary, grnPrtcls, geomMotion),
	transferVec_(boundary.mirrorBoundary().displacementVectroToMirror())
{
	if(boundary.thisBoundaryIndex()%2==1)
    {
    	masterInteraction_ = true;
		this->allocatePPPairs();
		this->allocatePWPairs();
	   
    }
    else
    {
        masterInteraction_ = false;
    }
}

template <typename cFM, typename gMM>
bool pFlow::periodicBoundaryGrainInteraction<cFM, gMM>::grainGrainInteraction
(
	real dt,
	const ContactForceModel &cfModel,
	uint32 step
)
{
	if(!masterInteraction_) return false;
	
	pFlow::periodicBoundarySIKernels::grainGrainInteraction(
		dt,
		this->ppPairs(),
		cfModel,
		transferVec_,
		this->boundary().thisPoints(),
		this->mirrorBoundary().thisPoints(),
		this->grnParticles().diameter().deviceViewAll(),
		this->grnParticles().coarseGrainFactor().deviceViewAll(),
		this->grnParticles().propertyId().deviceViewAll(),
		this->grnParticles().velocity().deviceViewAll(),
		this->grnParticles().rVelocity().deviceViewAll(),
		this->grnParticles().contactForce().deviceViewAll(),
		this->grnParticles().contactTorque().deviceViewAll());

	return false;
}