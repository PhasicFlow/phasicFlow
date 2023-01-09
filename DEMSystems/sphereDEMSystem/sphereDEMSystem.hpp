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

#ifndef __sphereDEMSystem_hpp__
#define __sphereDEMSystem_hpp__

#include "DEMSystem.hpp"
#include "property.hpp"
#include "uniquePtr.hpp"
#include "geometry.hpp"
#include "sphereFluidParticles.hpp"
#include "interaction.hpp"
#include "Insertions.hpp"
#include "domainDistribute.hpp"


namespace pFlow
{

class sphereDEMSystem
:
	public DEMSystem
{
protected:
	
	// protected members 

	uniquePtr<property> 		property_ = nullptr;

	uniquePtr<geometry> 		geometry_ = nullptr;

	uniquePtr<sphereFluidParticles> 	particles_ = nullptr;

	uniquePtr<sphereInsertion> 	insertion_ = nullptr;

	uniquePtr<interaction> 		interaction_ = nullptr;

	uniquePtr<domainDistribute> particleDistribution_=nullptr;

	// to be used for CPU communications 
	ViewType1D<realx3, HostSpace>  parVelocity_;

	ViewType1D<realx3, HostSpace>  parPosition_;

	ViewType1D<real, HostSpace> parDiameter_;

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

	void loop();

public:

	TypeInfo("sphereDEMSystem");

	sphereDEMSystem(
		word  demSystemName,
		const std::vector<box>& domains,
		int argc, 
		char* argv[]);

	virtual ~sphereDEMSystem();

	sphereDEMSystem(const sphereDEMSystem&)=delete;

	sphereDEMSystem& operator = (const sphereDEMSystem&)=delete;

	add_vCtor(
		DEMSystem,
		sphereDEMSystem,
		word);


	bool updateParticleDistribution(real extentFraction, const std::vector<box> domains) override;
	 
	int32 numParInDomain(int32 di)const override;
	
	std::vector<int32> numParInDomain()const override;

	span<const int32> parIndexInDomain(int32 di)const override;

	span<real> parDiameter() override;

	span<realx3> parVelocity() override;

	span<realx3> parPosition() override;

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