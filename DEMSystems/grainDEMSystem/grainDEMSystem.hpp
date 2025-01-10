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

#ifndef __grainDEMSystem_hpp__
#define __grainDEMSystem_hpp__

#include "DEMSystem.hpp"
#include "property.hpp"
#include "uniquePtr.hpp"
#include "geometry.hpp"
#include "grainFluidParticles.hpp"
#include "interaction.hpp"
#include "Insertions.hpp"
#include "domainDistribute.hpp"


namespace pFlow
{

class grainDEMSystem
:
	public DEMSystem
{
protected:
	
	// protected members 

	uniquePtr<property> 			property_ = nullptr;

	uniquePtr<geometry> 			geometry_ = nullptr;

	uniquePtr<grainFluidParticles>  particles_ = nullptr;

	uniquePtr<grainInsertion> 		insertion_ = nullptr;

	uniquePtr<interaction> 			interaction_ = nullptr;

	uniquePtr<domainDistribute> 	particleDistribution_=nullptr;

	// to be used for CPU communications 
	ViewType1D<realx3, HostSpace> 	velocityHost_;

	ViewType1D<realx3, HostSpace> 	positionHost_;

	ViewType1D<real, HostSpace> 	diameterHost_;

	bool 							requireRVel_ = false;

	ViewType1D<realx3, HostSpace> 	rVelocityHost_;


//  protected member functions
	auto& Property()
	{
		return property_();
	}

	auto& Geometry()
	{
		return geometry_();
	}

	auto& Particles()
	{
		return particles_();
	}

	auto& Interaction()
	{
		return interaction_();
	} 

	bool loop();

public:

	TypeInfo("grainDEMSystem");

	grainDEMSystem(
		word  demSystemName,
		const std::vector<box>& domains,
		int argc, 
		char* argv[],
		bool requireRVel=false);

	virtual ~grainDEMSystem();

	grainDEMSystem(const grainDEMSystem&)=delete;

	grainDEMSystem& operator = (const grainDEMSystem&)=delete;

	add_vCtor(
		DEMSystem,
		grainDEMSystem,
		word);


	bool updateParticleDistribution(real extentFraction, const std::vector<box> domains) override;
	 
	int32 numParInDomain(int32 di)const override;
	
	std::vector<int32> numParInDomains()const override;

	span<const int32> parIndexInDomain(int32 di)const override;

	span<real> diameter() override;

	span<real> courseGrainFactor() override;

	span<realx3> acceleration() override;

	span<realx3> velocity() override;

	span<realx3> position() override;

	span<realx3> rAcceleration() override;

	span<realx3> rVelocity() override;

	span<realx3> rPosition() override;

	span<realx3> parFluidForce() override;

	span<realx3> parFluidTorque() override;

	bool sendFluidForceToDEM() override;

	bool sendFluidTorqueToDEM() override;

	bool beforeIteration() override;

	bool iterate(
		real upToTime, 
		real timeToWrite, 
		word timeName) override;

	bool iterate(real upToTime) override;
	
	real maxBounndingSphereSize()const override;

};


} // pFlow

#endif