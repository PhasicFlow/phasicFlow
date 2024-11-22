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
	if(masterInteraction_)
	{
		this->allocatePPPairs();
		this->allocatePWPairs();
	}
		
}


#ifdef BoundaryModel1

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
			pFlow::MPI::processorBoundarySIKernels::sphereSphereInteraction(
				"ppBoundaryInteraction."+this->boundaryName(),
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
			
			return true;
		}		
		else if(step == 3 )
		{
			cfBndry.sendBackData();
			ctBndry.sendBackData();
			
			return true;
		}
		
		
		return false;	
	}
	else
	{
		
		if(step == 1 )
		{
			const auto & sphPar = this->sphParticles();
			uint32 thisIndex = this->boundary().thisBoundaryIndex();
			const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&>(
			sphPar.contactForce().BoundaryField(thisIndex));
			const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
			sphPar.contactTorque().BoundaryField(thisIndex));

			cfBndry.recieveBackData();
			ctBndry.recieveBackData();
			
			return false;
		}
		else if(step == 11)
		{
			const auto & sphPar = this->sphParticles();
			uint32 thisIndex = this->boundary().thisBoundaryIndex();
			const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&>(
			sphPar.contactForce().BoundaryField(thisIndex));
			const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
			sphPar.contactTorque().BoundaryField(thisIndex));

			cfBndry.addBufferToInternalField();
			ctBndry.addBufferToInternalField();
	
			return true;
		}
		
		
		return false;
	}

	return false;
}
#else

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
			
			pFlow::MPI::processorBoundarySIKernels::sphereSphereInteraction(
				"ppBoundaryInteraction."+this->boundaryName(),
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
		
			return true;
		}		
		else if(step == 3 )
		{
			cfBndry.sendBackData();
			ctBndry.sendBackData();
			return true;
		}
		else if(step == 11 )
		{
			cfBndry.updateBoundaryFromSlave();
			ctBndry.updateBoundaryFromSlave();
			return true;
		}

		return false;	
	}
	else
	{
		
		if(step == 1 )
		{
			const auto & sphPar = this->sphParticles();
			uint32 thisIndex = this->boundary().thisBoundaryIndex();
			const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&>(
			sphPar.contactForce().BoundaryField(thisIndex));
			const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
			sphPar.contactTorque().BoundaryField(thisIndex));

			cfBndry.recieveBackData();
			ctBndry.recieveBackData();
			
			return false;
		}
		else if(step == 11)
		{
			const auto & sphPar = this->sphParticles();
			uint32 thisIndex = this->boundary().thisBoundaryIndex();
			const auto& cfBndry = static_cast<const processorBoundaryField<realx3>&>(
			sphPar.contactForce().BoundaryField(thisIndex));
			const auto& ctBndry = static_cast<const processorBoundaryField<realx3>&> (
			sphPar.contactTorque().BoundaryField(thisIndex));

			cfBndry.addBufferToInternalField();
			cfBndry.updateBoundaryToMaster();

			ctBndry.addBufferToInternalField();
			ctBndry.updateBoundaryToMaster();

			return true;
		}
				
		return false;
	}

	return false;
}

#endif