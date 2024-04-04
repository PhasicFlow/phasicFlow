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
pFlow::periodicBoundarySphereInteraction<cFM, gMM>::periodicBoundarySphereInteraction(
	const boundaryBase &boundary,
	const sphereParticles &sphPrtcls,
	const GeometryMotionModel &geomMotion)
	: boundarySphereInteraction<cFM,gMM>(boundary, sphPrtcls, geomMotion),
	transferVec_(boundary.mirrorBoundary().displacementVectroToMirror())
{
	if(boundary.thisBoundaryIndex()%2==1)
    {
       masterInteraction_ = true;
    }
    else
    {
        masterInteraction_ = false;
    }
}

template <typename cFM, typename gMM>
bool pFlow::periodicBoundarySphereInteraction<cFM, gMM>::sphereSphereInteraction
(
	real dt,
	const ContactForceModel &cfModel
)
{
	if(!masterInteraction_) return true;
	
	pFlow::periodicBoundarySIKernels::sphereSphereInteraction(
		dt,
		this->ppPairs(),
		cfModel,
		transferVec_,
		this->boundary().thisPoints(),
		this->mirrorBoundary().thisPoints(),
		this->sphParticles().diameter().deviceViewAll(),
		this->sphParticles().propertyId().deviceViewAll(),
		this->sphParticles().velocity().deviceViewAll(),
		this->sphParticles().rVelocity().deviceViewAll(),
		this->sphParticles().contactForce().deviceViewAll(),
		this->sphParticles().contactTorque().deviceViewAll());

	return true;
}