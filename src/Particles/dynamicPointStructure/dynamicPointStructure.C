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

#include "dynamicPointStructure.H"


pFlow::dynamicPointStructure::dynamicPointStructure
(
	Time& time,
	const word& integrationMethod
)
:
	time_(time),
	integrationMethod_(integrationMethod),
	pStruct_(
		time_.emplaceObject<pointStructure>(
			objectFile(
				pointStructureFile__,
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS			
				)
			)
		),
	velocity_(
		time_.emplaceObject<realx3PointField_D>(
			objectFile(
				"velocity",
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_ALWAYS
				),
			pStruct_,
			zero3
			)
		)

{

	Report(1)<< "Creating integration method "<<
		greenText(integrationMethod_)<<" for dynamicPointStructure."<<endReport;

	integrationPos_ = integration::create(
		"pStructPosition",
		time_.integration(),
		pStruct_,
		integrationMethod_);
	
	integrationVel_ = integration::create(
		"pStructVelocity",
		time_.integration(),
		pStruct_,
		integrationMethod_);

	if( !integrationPos_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for dynamicPointStructure (position). \n";
		fatalExit;
	}

	if( !integrationVel_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for dynamicPointStructure (velocity). \n";
		fatalExit;
	}

}

bool pFlow::dynamicPointStructure::predict
(
	real dt,
	realx3PointField_D& acceleration
)
{
	auto& pos = pStruct_.pointPosition();

	if(!integrationPos_().predict(dt, pos.VectorField(), velocity_.VectorField() ))return false;
	if(!integrationVel_().predict(dt, velocity_.VectorField(), acceleration.VectorField()))return false;

	return true;
}

bool pFlow::dynamicPointStructure::correct
(
	real dt,
	realx3PointField_D& acceleration
)
{
	auto& pos = pStruct_.pointPosition();
	
	if(!integrationPos_().correct(dt, pos.VectorField(), velocity_.VectorField() ))return false;
	
	if(!integrationVel_().correct(dt, velocity_.VectorField(), acceleration.VectorField()))return false;

	return true;	
}