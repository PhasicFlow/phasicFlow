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


#include "particles.hpp"


pFlow::particles::particles
(
    systemControl &control
)
: 
	observer(defaultMessage_),
	demComponent("particles", control),
	dynPointStruct_(control),
	id_
	(
		objectFile
		(
			"id",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		static_cast<uint32>(-1)
	),
	shapeIndex_
	(
		objectFile
		(
			"shapeIndex",
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct_,
		0
	),
	accelertion_
	(
		objectFile(
			"accelertion",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS),
		dynPointStruct_,
		zero3
	),
	contactForce_(
		objectFile(
			"contactForce",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS),
		dynPointStruct_,
		zero3),
	contactTorque_(
		objectFile(
			"contactTorque",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS),
		dynPointStruct_,
		zero3)
{
	this->addToSubscriber(dynPointStruct_);
}

bool pFlow::particles::beforeIteration() 
{
	
	dynPointStruct_.beforeIteration();
	zeroForce();
	zeroTorque();

	return true;
}

bool pFlow::particles::iterate()
{
	return dynPointStruct_.iterate();
}

bool pFlow::particles::afterIteration() 
{
	return dynPointStruct_.afterIteration();
}

void pFlow::particles::boundingSphereMinMax
(
	real &minDiam, 
	real &maxDiam
) const
{
	auto& shp = getShapes();
	minDiam = shp.minBoundingSphere();
	maxDiam = shp.maxBoundingSphere();
}