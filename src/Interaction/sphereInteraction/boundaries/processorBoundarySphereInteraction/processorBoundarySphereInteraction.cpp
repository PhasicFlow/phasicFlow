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

#include "processorBoundarySIKernels.hpp"

template <typename cFM, typename gMM>
pFlow::MPI::processorBoundarySphereInteraction<cFM, gMM>::processorBoundarySphereInteraction(
	const boundaryBase &boundary,
	const sphereParticles &sphPrtcls,
	const GeometryMotionModel &geomMotion)
: 
	boundarySphereInteraction<cFM,gMM>(
		boundary, 
		sphPrtcls, 
		geomMotion
	),
	masterInteraction_(boundary.isBoundaryMaster())
{
	pOutput<<"Processor boundayrCondition for "<< boundary.name()<<endl;
}

template <typename cFM, typename gMM>
bool pFlow::MPI::processorBoundarySphereInteraction<cFM, gMM>::sphereSphereInteraction
(
	real dt,
	const ContactForceModel &cfModel
)
{
	if(!masterInteraction_) return true;
	
	const auto & sphPar = this->sphParticles();
	uint32 thisIndex = this->boundary().thisBoundaryIndex();
	const auto& a = sphPar.diameter().BoundaryField(thisIndex).neighborProcField().deviceViewAll();

	/*pFlow::MPI::processorBoundarySIKernels::sphereSphereInteraction(
		dt,
		this->ppPairs(),
		cfModel,
		this->boundary().thisPoints(),
		sphPar.diameter().deviceViewAll(),
		sphPar.propertyId().deviceViewAll(),
		sphPar.velocity().deviceViewAll(),
		sphPar.rVelocity().deviceViewAll(),
		sphPar.contactForce().deviceViewAll(),
		sphPar.contactTorque().deviceViewAll(),
		this->boundary().neighborProcPoints().deviceViewAll(),
		sphPar.diameter().BoundaryField(thisIndex).neighborProcField().deviceViewAll(),
		sphPar.propertyId().BoundaryField(thisIndex).neighborProcField().deviceViewAll(),
		sphPar.velocity().BoundaryField(thisIndex).neighborProcField().deviceViewAll(),
		sphPar.rVelocity().BoundaryField(thisIndex).neighborProcField().deviceViewAll(),
		sphPar.contactForce().BoundaryField(thisIndex).neighborProcField().deviceViewAll(),
		sphPar.contactTorque().BoundaryField(thisIndex).neighborProcField().deviceViewAll()
	);*/

	return true;
}