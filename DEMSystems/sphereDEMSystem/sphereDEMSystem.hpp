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
#include "sphereParticles.hpp"
#include "interaction.hpp"
#include "Insertions.hpp"
#include "domainDistribute.hpp"


namespace pFlow::coupling
{

class sphereDEMSystem
:
	public DEMSystem
{
protected:
	
	// protected members 

	uniquePtr<property> 		property_ = nullptr;

	uniquePtr<geometry> 		geometry_ = nullptr;

	uniquePtr<sphereParticles> 	particles_ = nullptr;

	uniquePtr<sphereInsertion> 	insertion_ = nullptr;

	uniquePtr<interaction> 		interaction_ = nullptr;

	uniquePtr<domainDistribute> particleDistribution_=nullptr;

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

public:

	TypeInfo("sphereDEMSystem");

	sphereDEMSystem(
		word  demSystemName,
		int32 numDomains, 
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


	 
	int32 numParInDomain(int32 di)const override;

	
	std::vector<int32> numParInDomain()const override;
	
	
	virtual 
	bool iterate(int32 n, real timeToWrite, word timeName) override;
	
	virtual 
	real maxBounndingSphereSize()const override;

};


} // pFlow

#endif