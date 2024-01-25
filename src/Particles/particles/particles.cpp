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

bool pFlow::particles::initMassDiameter()const 
{

	auto [minIndex, maxIndex] = minMax(shapeIndex_.internal());

	const auto& shp = getShapes();

	if( !shp.indexValid(maxIndex) )
	{
		fatalErrorInFunction<< 
		"the maximum value of shape index is "<< maxIndex << 
		" which is not valid."<<endl;
		return false;
	}

	realVector_D d("diameter", shp.boundingDiameter());
	realVector_D mass("mass",shp.mass());
	uint8Vector_D propId("propId", shp.shapePropertyIds());

	
	auto aPointsMask = dynPointStruct_.activePointsMaskDevice();
	auto aRange = aPointsMask.activeRange();

	using exeSpace = typename realPointField_D::execution_space;
	using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32>>;

	auto field_diameter = diameter_.fieldDevice();
	auto field_mass = mass_.fieldDevice();
	auto field_propId = propertyId_.fieldDevice();
	auto field_shapeIndex = shapeIndex_.fieldDevice();

	auto d_d = d.deviceVector();
	auto d_mass = mass.deviceVector();
	auto d_propId = propId.deviceVector();

	Kokkos::parallel_for(
		"particles::initMassDiameter",
		policy(aRange.start(), aRange.end()),
		LAMBDA_HD(uint32 i)
		{
			if(aPointsMask(i))
			{
				uint32 index = field_shapeIndex[i];
				field_diameter[i] = d_d[index];
				field_mass[i] = d_mass[index];
				field_propId[i] = d_propId[index];
			}
		});
	Kokkos::fence();

    return true;
}

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
	propertyId_
	(
		objectFile
		(
			"propertyId",
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		dynPointStruct_,
		static_cast<int8>(0)
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
	diameter_
	(
		objectFile(
			"diameter",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct_,
		0.00000000001
	),
	mass_
	(
		objectFile(
			"mass",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct_,
		0.0000000001
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

	if(!initMassDiameter())
	{
		fatalExit;
	}

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