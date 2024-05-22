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
	,
	inter_("inter"),
	send_("send"),
	recv_("recv"),
	add_("add")
{
	if(masterInteraction_)
	{
		this->allocatePPPairs();
		this->allocatePWPairs();
	}
		
}

template <typename cFM, typename gMM>
bool pFlow::MPI::processorBoundarySphereInteraction<cFM, gMM>::sphereSphereInteraction
(
	real dt,
	const ContactForceModel &cfModel,
	uint32 step
)
{
	
	// master processor calculates the contact force/torque and sends data back to the 
	// neighbor processor (slave processor).
	// slave processor recieves the data and adds the data to the internalField 
	if(masterInteraction_) 
	{
		if(step==1)return true;

		const auto & sphPar = this->sphParticles();
		uint32 thisIndex = this->boundary().thisBoundaryIndex();
		
		const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&> (
		sphPar.contactForce().BoundaryField(thisIndex));

		const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
		sphPar.contactTorque().BoundaryField(thisIndex));

		if(step == 2 )
		{
			iter++;
			inter_.start();
			pFlow::MPI::processorBoundarySIKernels::sphereSphereInteraction(
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
				cfBndry.neighborProcField().deviceViewAll(),
				ctBndry.neighborProcField().deviceViewAll()
			);
			inter_.end();
			return true;
		}		
		else if(step == 3 )
		{
			send_.start();
			cfBndry.sendBackData();
			ctBndry.sendBackData();
			send_.end();
			return true;
		}
		
		if(iter % 1000 == 0u)
		{
			pOutput<<"inter "<< inter_.totalTime()<<endl;
			pOutput<<"send "<< send_.totalTime()<<endl<<endl;;
		}
		return false;	
	}
	else
	{
		
		const auto & sphPar = this->sphParticles();
		uint32 thisIndex = this->boundary().thisBoundaryIndex();
		const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&>(
		sphPar.contactForce().BoundaryField(thisIndex));
		const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
		sphPar.contactTorque().BoundaryField(thisIndex));
		if(step==1)
		{
			recv_.start();
			cfBndry.recieveBackData();
			ctBndry.recieveBackData();
			recv_.end();
			return false;
		}
		else if(step == 2)
		{
			iter++;
			return true;
		}
		else if(step == 3)
		{
			add_.start();
			cfBndry.addBufferToInternalField();
			ctBndry.addBufferToInternalField();
			add_.end();
			return true;
		}
		
		if(iter % 1000 == 0u)
		{
			pOutput<<"recive "<< recv_.totalTime()<<endl;
			pOutput<<"add "<< add_.totalTime()<<endl<<endl;
		}

		return false;
	}

	return false;
}