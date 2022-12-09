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

#include <array>

#include "systemControl.hpp"
#include "property.hpp"
#include "uniquePtr.hpp"
#include "geometry.hpp"
#include "sphereParticles.hpp"
#include "interaction.hpp"
#include "Insertions.hpp"
#include "readControlDict.hpp"




namespace pFlow
{

class sphereDEMSystem
{
protected:
	
	readControlDict 						ControlDict_;

	uniquePtr<systemControl>		Control_ = nullptr;

	uniquePtr<property> 				property_ = nullptr;

	uniquePtr<geometry> 			  geometry_ = nullptr;

	uniquePtr<sphereParticles> 	particles_ = nullptr;

	uniquePtr<sphereInsertion> 	insertion_ = nullptr;

	uniquePtr<interaction> 			interaction_ = nullptr;

	

	auto& Control()
	{
		return Control_();
	}

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

	sphereDEMSystem(int argc, char* argv[]);

	~sphereDEMSystem();

	sphereDEMSystem(const sphereDEMSystem&)=delete;

	sphereDEMSystem& operator = (const sphereDEMSystem&)=delete;

	
	std::array<double,3> g()const
	{
		return {
			Control_->g().x(), 
			Control_->g().y(), 
			Control_->g().z()};
	}

};


} // pFlow

#endif