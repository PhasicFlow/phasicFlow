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

#include "sphereParticles.hpp"
#include "systemControl.hpp"
#include "vocabs.hpp"
#include "sphereParticlesKernels.hpp"


bool pFlow::sphereParticles::initializeParticles()
{
	
	using exeSpace = typename realPointField_D::execution_space;
	using policy = Kokkos::RangePolicy<
			exeSpace,
			Kokkos::IndexType<uint32>>;
	
	auto [minIndex, maxIndex] = minMax(shapeIndex().internal());		

	if( !spheres_.indexValid(maxIndex) )
	{
		fatalErrorInFunction<< 
		"the maximum value of shapeIndex is "<< maxIndex << 
		" which is not valid."<<endl;
		return false;
	}

	auto aPointsMask 	= dynPointStruct().activePointsMaskDevice();
	auto aRange 		= aPointsMask.activeRange();
	
	auto field_shapeIndex 	= shapeIndex().deviceView();
	auto field_diameter 	= diameter_.deviceView();
	auto field_mass 		= mass_.deviceView();
	auto field_propId 		= propertyId_.deviceView();
	auto field_I 			= I_.deviceView();

	// get info from spheres shape 
	realVector_D d("diameter", spheres_.boundingDiameter());
	realVector_D mass("mass",spheres_.mass());
	uint32Vector_D propId("propId", spheres_.shapePropertyIds());
	realVector_D  I("I", spheres_.Inertia());

	auto d_d 		= d.deviceView();
	auto d_mass 	= mass.deviceView();
	auto d_propId 	= propId.deviceView();
	auto d_I 		= I.deviceView();
		
	Kokkos::parallel_for(
		"particles::initInertia",
		policy(aRange.start(), aRange.end()),
		LAMBDA_HD(uint32 i)
		{
			if(aPointsMask(i))
			{
				uint32 index = field_shapeIndex[i];
				field_I[i] = d_I[index];
				field_diameter[i] = d_d[index];
				field_mass[i] = d_mass[index];
				field_propId[i] = d_propId[index];
			}
		});
	Kokkos::fence();
	
	return true;
}

bool
pFlow::sphereParticles::getParticlesInfoFromShape(
  const wordVector& shapeNames,
  uint32Vector&     propIds,
  realVector&       diams,
  realVector&       m,
  realVector&       Is,
  uint32Vector&     shIndex
)
{
	auto numNew = static_cast<uint32>(shapeNames.size());

	propIds.clear();
	propIds.reserve(numNew);

	diams.clear();
	diams.reserve(numNew);

	m.clear();
	m.reserve(numNew);

	Is.clear();
	Is.reserve(numNew);

	shIndex.clear();
	shIndex.reserve(numNew);
	

	for(const auto& name:shapeNames)
	{
		uint32 indx;
		if(spheres_.shapeNameToIndex(name,indx))
		{
			shIndex.push_back(indx);
			Is.push_back( spheres_.Inertia(indx));
			m.push_back(spheres_.mass(indx));
			diams.push_back(spheres_.boundingDiameter(indx));
			propIds.push_back( spheres_.propertyId(indx));
		}
		else
		{	
			fatalErrorInFunction<<"Shape name "<< name << 
			"does not exist. The list is "<<spheres_.shapeNameList()<<endl;
			return false;
		}
	}
	
	return true;
}

pFlow::sphereParticles::sphereParticles(
	systemControl &control,
	const sphereShape& shpShape
)
:
	particles(control, shpShape),
	spheres_(shpShape),
	propertyId_
	(
		objectFile
		(
			"propertyId",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		dynPointStruct(),
		0u
	),
	diameter_
	(
		objectFile(
			"diameter",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct(),
		0.00000000001
	),
	mass_
	(
		objectFile(
			"mass",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER),
		dynPointStruct(),
		0.0000000001
	),
	I_
	(
		objectFile
		(
			"I",
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		dynPointStruct(),
		static_cast<real>(0.0000000001)
	),
	rVelocity_
	(
		objectFile
		(
			"rVelocity",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		dynPointStruct(),
		zero3
	),
	rAcceleration_
	(
		objectFile(
			"rAcceleration",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
			),
		dynPointStruct(),
		zero3
	),
	boundarySphereParticles_
	(
		dynPointStruct().boundaries(),
		*this
	),
	accelerationTimer_(
		"Acceleration", &this->timers() ),
	intPredictTimer_(
		"Integration-predict", &this->timers() ),
	intCorrectTimer_(
		"Integration-correct", &this->timers() ),
	fieldUpdateTimer_(
		"fieldUpdate", &this->timers() )
{
	
	auto intMethod = control.settingsDict().getVal<word>("integrationMethod");
	REPORT(1)<<"Creating integration method "<<Green_Text(intMethod)
		  << " for rotational velocity."<<END_REPORT;
		  
	rVelIntegration_ = integration::create
		(
			"rVelocity",
			dynPointStruct(),
			intMethod,
			rAcceleration_.field(),
			control.keepIntegrationHistory()
		);

	if( !rVelIntegration_ )
	{
		fatalErrorInFunction<<
		"  error in creating integration object for rVelocity. \n";
		fatalExit;
	}

	if(!initializeParticles())
	{
		fatalErrorInFunction;
		fatalExit;
	}
	
} 

bool pFlow::sphereParticles::beforeIteration()
{
	particles::beforeIteration();
	intPredictTimer_.start();
		auto dt = this->dt();
		dynPointStruct().predict(dt);
		rVelIntegration_().predict(dt,rVelocity_, rAcceleration_);
	intPredictTimer_.end();

	fieldUpdateTimer_.start();
	propertyId_.updateBoundariesSlaveToMasterIfRequested();
	diameter_.updateBoundariesSlaveToMasterIfRequested();
	mass_.updateBoundariesSlaveToMasterIfRequested();
	I_.updateBoundariesSlaveToMasterIfRequested();
	rVelocity_.updateBoundariesSlaveToMasterIfRequested();
	rAcceleration_.updateBoundariesSlaveToMasterIfRequested();
	rVelIntegration_().updateBoundariesSlaveToMasterIfRequested();
	fieldUpdateTimer_.end();
	
	return true;
}

bool pFlow::sphereParticles::iterate() 
{

	const timeInfo ti = TimeInfo();
	const realx3 g = control().g();

	particles::iterate();
	accelerationTimer_.start();
		pFlow::sphereParticlesKernels::acceleration(
			g,
			mass().deviceViewAll(),
			contactForce().deviceViewAll(),
			I().deviceViewAll(),
			contactTorque().deviceViewAll(),
			dynPointStruct().activePointsMaskDevice(),
			acceleration().deviceViewAll(),
			rAcceleration().deviceViewAll()
			);
		ForAllActiveBoundaries(i,boundarySphereParticles_)
		{
			boundarySphereParticles_[i].acceleration(ti, g);
		}

	accelerationTimer_.end();
	
	intCorrectTimer_.start();
	
		if(!dynPointStruct().correct(ti.dt()))
		{
			return false;
		}
		real damping = dynPointStruct().dampingFactor(ti);
		if(!rVelIntegration_().correct(
			ti.dt(), 
			rVelocity_, 
			rAcceleration_,
			damping))
		{
			return false;
		}
	
	intCorrectTimer_.end();
	
	return true;
}

bool pFlow::sphereParticles::insertParticles
(
	const realx3Vector &position, 
	const wordVector &shapesNames, 
	const anyList &setVarList
)
{
	anyList newVarList(setVarList);

	realVector mass("mass");
	realVector I("I");
	realVector diameter("diameter");
	uint32Vector propId("propId");
	uint32Vector shapeIdx("shapeIdx");

	if(!getParticlesInfoFromShape(
		shapesNames,
		propId,
		diameter,
		mass,
		I,
		shapeIdx))
	{
		return false;
	}

	newVarList.emplaceBack(
		mass_.name()+"Vector",
		std::move(mass));

	newVarList.emplaceBack(
		I_.name()+"Vector",
		std::move(I));
	
	newVarList.emplaceBack(
		diameter_.name()+"Vector",
		std::move(diameter));

	newVarList.emplaceBack(
		propertyId_.name()+"Vector",
		std::move(propId));

	newVarList.emplaceBack(
		shapeIndex().name()+"Vector",
		std::move(shapeIdx));
	
	if(!dynPointStruct().insertPoints(position, newVarList))
	{
		return false;
	}

    return true;
}

pFlow::word pFlow::sphereParticles::shapeTypeName()const
{
	return "sphere";
}

const pFlow::shape &pFlow::sphereParticles::getShapes() const
{
    return spheres_;
}

void pFlow::sphereParticles::boundingSphereMinMax
(
	real & minDiam, 
	real& maxDiam
)const
{
	minDiam = spheres_.minBoundingSphere();
	maxDiam = spheres_.maxBoundingSphere();
}
