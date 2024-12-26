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
    	objectFile(
        	"fluidForce",
    		"",
    		objectFile::READ_IF_PRESENT,
        	objectFile::WRITE_ALWAYS
      	),
      dynPointStruct(),
      zero3
    ),
	fluidTorque_(
		objectFile(
			"fluidTorque",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_NEVER
      	),
    	dynPointStruct(),
    	zero3
	)
{} 

bool pFlow::sphereFluidParticles::beforeIteration()
{
	sphereParticles::beforeIteration();
	return true;
}


bool pFlow::sphereFluidParticles::iterate() 
{

	accelerationTimer().start();
		pFlow::sphereFluidParticlesKernels::acceleration(
			control().g(),
			mass().deviceViewAll(),
			contactForce().deviceViewAll(),
			fluidForce_.deviceViewAll(),
			I().deviceViewAll(),
			contactTorque().deviceViewAll(),
			fluidTorque_.deviceViewAll(),
			pStruct().activePointsMaskDevice(),
			accelertion().deviceViewAll(),
			rAcceleration().deviceViewAll()
			);
	accelerationTimer().end();
	
	intCorrectTimer().start();
	
		dynPointStruct().correct(this->dt(), accelertion());
	
		rVelIntegration().correct(this->dt(), rVelocity(), rAcceleration());
	
	intCorrectTimer().end();
	
	return true;
}

void pFlow::sphereFluidParticles::fluidForceHostUpdatedSync()
{
	copy(fluidForce_.deviceView(), fluidForceHost_);
	return;
}

void pFlow::sphereFluidParticles::fluidTorqueHostUpdatedSync()
{
	copy(fluidTorque_.deviceView(), fluidTorqueHost_);
	return;
}