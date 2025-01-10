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
   @class pFlow::sphereParticles
    
   @brief Class for managing spherical particles 
   
   This is a top-level class that contains the essential components for
   defining spherical prticles in a DEM simulation.
 */

#ifndef __sphereFluidParticles_hpp__
#define __sphereFluidParticles_hpp__ 

#include "sphereParticles.hpp"


namespace pFlow
{

class sphereFluidParticles
:
	public sphereParticles 
{
protected:
	
	/// pointField of rotational acceleration of particles on device  
	realx3PointField_D 		fluidForce_;

	hostViewType1D<realx3> 	fluidForceHost_;

	realx3PointField_D 		fluidTorque_;

	hostViewType1D<realx3>	fluidTorqueHost_;

	void checkHostMemory();

	/*void zeroFluidForce_H()
	{
		fluidForce_.fillHost(zero3);
	}

	void zeroFluidTorque_H()
	{
		fluidTorque_.fillHost(zero3);
	}*/

public:

	/// construct from systemControl and property 
	sphereFluidParticles(systemControl &control, const property& prop);
	
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

	void fluidForceHostUpdatedSync();

	void fluidTorqueHostUpdatedSync();
	
}; //sphereFluidParticles

} // pFlow

#endif //__sphereFluidParticles_hpp__
