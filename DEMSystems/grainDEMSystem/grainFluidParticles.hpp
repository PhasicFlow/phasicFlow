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

/*! 
   @class pFlow::grainFluidParticles
    
   @brief Class for managing grain particles with fluid interactions 
   
   This is a top-level class that contains the essential components for
   defining grain prticles with fluid interactions in a DEM simulation.
 */

#ifndef __grainFluidParticles_hpp__
#define __grainFluidParticles_hpp__ 

#include "grainParticles.hpp"


namespace pFlow
{

class grainFluidParticles
:
	public grainParticles 
{
protected:
	
	/// pointField of rotational acceleration of particles on device  
	realx3PointField_D 		fluidForce_;

	hostViewType1D<realx3> 	fluidForceHost_;

	realx3PointField_D 		fluidTorque_;

	hostViewType1D<realx3>	fluidTorqueHost_;

	hostViewType1D<real> 	courseGrainFactorHost_;


	void checkHostMemory();

public:

	/// construct from systemControl and property 
	grainFluidParticles(systemControl &control, const property& prop);

	~grainFluidParticles() override = default;
	
	/// before iteration step 
	bool beforeIteration() override;
	
	/// iterate particles 
	bool iterate() override;	

	auto& fluidForce()
	{
		return fluidForce_;
	}

	auto& fluidTorque()
	{
		return fluidTorque_;
	}

	auto& fluidForceHost()
	{
		return fluidForceHost_; 
	}

	auto& fluidTorqueHost()
	{
		return fluidTorqueHost_;
	}

	auto& courseGrainFactorHost()
	{
		return courseGrainFactorHost_;
	}

	void fluidForceHostUpdatedSync();

	void fluidTorqueHostUpdatedSync();
	
}; //grainFluidParticles

} // pFlow

#endif //__sphereFluidParticles_hpp__
