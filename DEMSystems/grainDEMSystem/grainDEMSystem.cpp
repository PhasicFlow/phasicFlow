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

#include "grainDEMSystem.hpp"
#include "vocabs.hpp"

bool pFlow::grainDEMSystem::loop()
{

	do 
	{
		//
		if(! insertion_().insertParticles( 
			Control().time().currentIter(),
			Control().time().currentTime(),
			Control().time().dt()	) )
		{
			fatalError<<
			"particle insertion failed in grainDEMSystem.\n";
			return false;
		}	

		geometry_->beforeIteration();
		
		interaction_->beforeIteration();

		particles_->beforeIteration();
		
		
		interaction_->iterate();

		particles_->iterate();

		geometry_->iterate();

		particles_->afterIteration();
		
		geometry_->afterIteration();

	}while(Control()++);

	return true;
}

pFlow::grainDEMSystem::grainDEMSystem(
		word  demSystemName,
		const std::vector<box>& domains,
		int argc, 
		char* argv[],
		bool requireRVel)
:
	DEMSystem(demSystemName, domains, argc, argv),
	requireRVel_(requireRVel)
{ 
	
	REPORT(0)<<"\nReading proprties . . . "<<END_REPORT;
	property_ = makeUnique<property>(
		propertyFile__,
    	Control().caseSetup().path());

	REPORT(0)<< "\nCreating surface geometry for grainDEMSystem . . . "<<END_REPORT;
	geometry_ = geometry::create(Control(), Property());

	REPORT(0)<<"Reading shape dictionary ..."<<END_REPORT;
	grains_ = makeUnique<grainShape>(
		pFlow::shapeFile__,
		&Control().caseSetup(),
		Property() );	

	REPORT(0)<<"\nReading grain particles . . ."<<END_REPORT;
	particles_ = makeUnique<grainFluidParticles>(Control(), grains_());


	insertion_ = makeUnique<grainInsertion>( 
		particles_(), 
		particles_().grains());
	
	REPORT(0)<<"\nCreating interaction model for grain-grain contact and grain-wall contact . . ."<<END_REPORT;
	interaction_ = interaction::create(
		Control(),
		Particles(),
		Geometry());

	auto maxD = maxBounndingSphereSize();

	particleDistribution_ = makeUnique<domainDistribute>(domains, maxD);

}


pFlow::grainDEMSystem::~grainDEMSystem()
{
	
}

bool pFlow::grainDEMSystem::updateParticleDistribution(
	real extentFraction,
	const std::vector<box> domains)
{

	if(!particleDistribution_->changeDomainsSize(
		extentFraction,
		maxBounndingSphereSize(),
		domains))
	{
		fatalErrorInFunction<<
		"cannot change the domain size"<<endl;
		return false;
	}

	if(!particleDistribution_->locateParticles(
			positionHost_, 
			particles_->pStruct().activePointsMaskHost()))
	{
		fatalErrorInFunction<<
		"error in locating particles among sub-domains"<<endl;
		return false;
	}

	return true;
}

pFlow::int32 
	pFlow::grainDEMSystem::numParInDomain(int32 di)const
{
	return particleDistribution_().numParInDomain(di);
}

std::vector<pFlow::int32> 
	pFlow::grainDEMSystem::numParInDomains()const
{
	const auto& distribute = particleDistribution_();
	int32 numDomains = distribute.numDomains();
	std::vector<int32> nums(numDomains);
	for(int32 i=0; i<numDomains; i++)
	{
		nums[i] = distribute.numParInDomain(i);
	}

	return nums;
}

pFlow::span<const pFlow::int32> 
pFlow::grainDEMSystem::parIndexInDomain(int32 di)const
{
	return particleDistribution_->particlesInDomain(di);
}

pFlow::span<pFlow::uint32> pFlow::grainDEMSystem::particleId()
{
	return span<uint32>(particleIdHost_.data(), particleIdHost_.size());
}


pFlow::span<pFlow::real> pFlow::grainDEMSystem::diameter() 
{	
	return span<real>(diameterHost_.data(), diameterHost_.size());
}

pFlow::span<pFlow::real> pFlow::grainDEMSystem::courseGrainFactor()
{
    return span<real>(particles_->courseGrainFactorHost().data(), particles_->courseGrainFactorHost().size());
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::acceleration()
{
    return span<realx3>(nullptr, 0);
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::velocity()  
{
	return span<realx3>(velocityHost_.data(), velocityHost_.size());
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::position()  
{
	return span<realx3>(positionHost_.data(), positionHost_.size());
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::rAcceleration()
{
    return span<realx3>(nullptr, 0);
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::rVelocity()
{
    return span<realx3>(rVelocityHost_.data(), rVelocityHost_.size());
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::rPosition()
{
    return span<realx3>(nullptr, 0);
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::parFluidForce() 
{
	auto& hVec =  particles_->fluidForceHost();
	
	return span<realx3>(hVec.data(), hVec.size());
}

pFlow::span<pFlow::realx3> pFlow::grainDEMSystem::parFluidTorque() 
{
	auto& hVec =  particles_->fluidTorqueHost();
	return span<realx3>(hVec.data(), hVec.size());
}

bool pFlow::grainDEMSystem::sendFluidForceToDEM() 
{
	particles_->fluidForceHostUpdatedSync();
	return true;
}

bool pFlow::grainDEMSystem::sendFluidTorqueToDEM()
{
	particles_->fluidTorqueHostUpdatedSync();
	return true;
}

bool pFlow::grainDEMSystem::beforeIteration()
{
	velocityHost_ = std::as_const(particles_()).velocity().hostView();
	positionHost_ = std::as_const(particles_()).pointPosition().hostView();
	diameterHost_ = particles_->diameter().hostView();
	particleIdHost_ = particles_->particleId().hostView();

	if(requireRVel_)
		rVelocityHost_ = std::as_const(particles_()).rVelocity().hostView();
	

	return true;
}


bool pFlow::grainDEMSystem::iterate(
	real upToTime, 
	real timeToWrite, 
	word timeName) 
{

	Control().time().setStopAt(upToTime);
	Control().time().setOutputToFile(timeToWrite, timeName);
	
	return loop();

	return true;
}

bool pFlow::grainDEMSystem::iterate(real upToTime)
{
	Control().time().setStopAt(upToTime);
	return loop();
	return true;
}

pFlow::real 
	pFlow::grainDEMSystem::maxBounndingSphereSize()const
{
	real minD, maxD;
	particles_->boundingSphereMinMax(minD, maxD);

	return maxD;
}