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

#include "sphereFluidParticles.hpp"
#include "sphereFluidParticlesKernels.hpp"


pFlow::sphereFluidParticles::sphereFluidParticles(
	systemControl &control,
	const property& prop
)
:
	sphereParticles(control, prop),
	fluidForce_(
		this->time().emplaceObject<realx3PointField_HD>(
			objectFile(
				"fluidForce",
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS
				),
			pStruct(),
			zero3
			)
		),
	fluidTorque_(
		this->time().emplaceObject<realx3PointField_HD>(
			objectFile(
				"fluidTorque",
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS
				),
			pStruct(),
			zero3
			)
		)
{} 



bool pFlow::sphereFluidParticles::iterate() 
{

	accelerationTimer_.start();
		pFlow::sphereFluidParticlesKernels::acceleration(
			control().g(),
			mass().deviceVectorAll(),
			contactForce().deviceVectorAll(),
			fluidForce().deviceVectorAll(),
			I().deviceVectorAll(),
			contactTorque().deviceVectorAll(),
			fluidTorque().deviceVectorAll(),
			pStruct().activePointsMaskD(),
			accelertion().deviceVectorAll(),
			rAcceleration().deviceVectorAll()
			);
	accelerationTimer_.end();
	
	intCorrectTimer_.start();
	
		dynPointStruct_.correct(this->dt(), accelertion_);
	
		rVelIntegration_().correct(this->dt(), rVelocity_, rAcceleration_);
	
	intCorrectTimer_.end();
	
	return true;
}

void pFlow::sphereFluidParticles::fluidForceHostUpdatedSync()
{
	fluidForce_.modifyOnHost();
	fluidForce_.syncViews();
	return;
}

void pFlow::sphereFluidParticles::fluidTorqueHostUpdatedSync()
{
	fluidTorque_.modifyOnHost();
	fluidTorque_.syncViews();
	return;
}