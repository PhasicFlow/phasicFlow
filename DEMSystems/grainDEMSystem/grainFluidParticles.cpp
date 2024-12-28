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

#include "grainFluidParticles.hpp"
#include "grainFluidParticlesKernels.hpp"


pFlow::grainFluidParticles::grainFluidParticles(
	systemControl &control,
	const property& prop
)
:
	grainParticles(control, prop),
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

bool pFlow::grainFluidParticles::beforeIteration()
{
	grainParticles::beforeIteration();
	
	// copy the data (if required) from device to host
	courseGrainFactorHost_ = coarseGrainFactor().hostView();

	return true;
}


bool pFlow::grainFluidParticles::iterate() 
{

	accelerationTimer().start();
		pFlow::grainFluidParticlesKernels::acceleration(
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

void pFlow::grainFluidParticles::fluidForceHostUpdatedSync()
{
	copy(fluidForce_.deviceView(), fluidForceHost_);
	return;
}

void pFlow::grainFluidParticles::fluidTorqueHostUpdatedSync()
{
	copy(fluidTorque_.deviceView(), fluidTorqueHost_);
	return;
}