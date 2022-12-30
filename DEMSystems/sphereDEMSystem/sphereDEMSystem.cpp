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

#include "sphereDEMSystem.hpp"


pFlow::sphereDEMSystem::sphereDEMSystem(
		word  demSystemName,
		const std::vector<box>& domains,
		int argc, 
		char* argv[])
:
	DEMSystem(demSystemName, domains, argc, argv)
{
  
	REPORT(0)<<"Initializing host/device execution spaces . . . \n";
	REPORT(1)<<"Host execution space is "<< greenText(DefaultHostExecutionSpace::name())<<endREPORT;
	REPORT(1)<<"Device execution space is "<<greenText(DefaultExecutionSpace::name())<<endREPORT;

 	// initialize Kokkos
	Kokkos::initialize( argc, argv );  
  
	

	REPORT(0)<<"\nReading proprties . . . "<<endREPORT;
	property_ = makeUnique<property>(
    	Control().caseSetup().path()+propertyFile__);

	REPORT(0)<< "\nCreating surface geometry for sphereDEMSystem . . . "<<endREPORT;
	geometry_ = geometry::create(Control(), Property());

	REPORT(0)<<"\nReading sphere particles . . ."<<endREPORT;
	particles_ = makeUnique<sphereParticles>(Control(), Property());


	REPORT(0)<<"\nCreating interaction model for sphere-sphere contact and sphere-wall contact . . ."<<endREPORT;
	interaction_ = interaction::create(
		Control(),
		Particles(),
		Geometry());

	real minD, maxD;
	particles_->boundingSphereMinMax(minD, maxD);

	particleDistribution_ = makeUnique<domainDistribute>(domains, maxD);

}

pFlow::sphereDEMSystem::~sphereDEMSystem()
{
	interaction_.reset();
	insertion_.reset();
	particles_.reset();
	geometry_.reset();
	property_.reset();
	Control_.reset();

	output<< "\nFinalizing host/device execution space ...."<<endl;
	Kokkos::finalize();
}



pFlow::int32 
	pFlow::sphereDEMSystem::numParInDomain(int32 di)const
{
	return particleDistribution_().numParInDomain(di);
}

std::vector<pFlow::int32> 
	pFlow::sphereDEMSystem::numParInDomain()const
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
pFlow::sphereDEMSystem::parIndexInDomain(int32 di)const
{
	return particleDistribution_->particlesInDomain(di);
}

bool pFlow::sphereDEMSystem::changeDomainsSizeUpdateParticles(
	const std::vector<box>& domains)
{
	if( !particleDistribution_->changeDomainsSize(domains))
		return false;

	// should update list of particles here 
	//************************************************************************************************
	notImplementedFunction;
	return false;	
}

bool pFlow::sphereDEMSystem::updateParticles()
{
	notImplementedFunction;
	return false;
}

bool pFlow::sphereDEMSystem::iterate(
	int32 n, 
	real timeToWrite, 
	word timeName) 
{
	return true;
}

pFlow::real 
	pFlow::sphereDEMSystem::maxBounndingSphereSize()const
{
	real minD, maxD;
	particles_->boundingSphereMinMax(minD, maxD);

	return maxD;
}