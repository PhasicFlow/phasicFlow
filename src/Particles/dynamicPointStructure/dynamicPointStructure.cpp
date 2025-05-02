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

#include "dynamicPointStructure.hpp"
#include "systemControl.hpp"


pFlow::dynamicPointStructure::dynamicPointStructure
(
	systemControl& control,
	real maxBSphere
)
:
	pointStructure(control, maxBSphere),
	velocity_
	(
		objectFile(
			"velocity",
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
			),
		*this,
		zero3
	),
	acceleration_(
      objectFile(
        "acceleration",
        "",
        objectFile::READ_IF_PRESENT,
        objectFile::WRITE_ALWAYS
      ),
      *this,
      zero3
    ),
	velocityUpdateTimer_("velocity boundary update", &timers()),
	integrationMethod_
	(
		control.settingsDict().getVal<word>("integrationMethod")
	)
{
	REPORT(1)<< "Creating integration method "<<
		Green_Text(integrationMethod_)<<" for dynamicPointStructure."<<END_REPORT;
	
	integrationPos_ = integration::create
	(
		"pStructPosition",
		*this,
		integrationMethod_,
		velocity_.field(),
		control.keepIntegrationHistory()
	);

	if( !integrationPos_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for dynamicPointStructure (position). \n";
		fatalExit;
	}
	
	integrationVel_ = integration::create
	(
		"pStructVelocity",
		*this,
		integrationMethod_,
		acceleration_.field(),
		control.keepIntegrationHistory()
	);

	if( !integrationVel_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for dynamicPointStructure (velocity). \n";
		fatalExit;
	}

	if(control.settingsDict().containsDictionay("globalDamping"))
	{
		REPORT(1)<<"Reading globalDamping dictionary ..."<<END_REPORT;
		velDamping_ = makeUnique<globalDamping>(control);
	}
}


bool pFlow::dynamicPointStructure::beforeIteration()
{
	if(!pointStructure::beforeIteration())return false;
	velocityUpdateTimer_.start();
	velocity_.updateBoundariesSlaveToMasterIfRequested();
	acceleration_.updateBoundariesSlaveToMasterIfRequested();
	integrationPos_->updateBoundariesSlaveToMasterIfRequested();
	integrationVel_->updateBoundariesSlaveToMasterIfRequested();
	velocityUpdateTimer_.end();
	return true;
}

bool pFlow::dynamicPointStructure::iterate()
{
	return pointStructure::iterate();
	
}

bool pFlow::dynamicPointStructure::afterIteration()
{
		//const auto ti = TimeInfo();
		
		auto succs = pointStructure::afterIteration();

		return succs;
}

bool pFlow::dynamicPointStructure::predict(real dt)
{

	if(!integrationPos_().predict(dt, pointPosition(), velocity_ ))return false;
	if(!integrationVel_().predict(dt, velocity_, acceleration_))return false;

	return true;
}

bool pFlow::dynamicPointStructure::correct(real dt)
{
	const auto& ti = TimeInfo();
	
	if(!integrationPos_().correctPStruct(dt, *this, velocity_) )return false;
	if(!integrationVel_().correct(dt, velocity_, acceleration_, dampingFactor(ti)))return false;

	return true;	
}
