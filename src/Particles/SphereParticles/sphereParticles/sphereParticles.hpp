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

#ifndef __sphereParticles_hpp__
#define __sphereParticles_hpp__ 

#include "systemControl.hpp"
#include "particles.hpp"
#include "sphereShape.hpp"
#include "property.hpp"
#include "indexContainer.hpp"



namespace pFlow
{

class sphereParticles
:
	public particles 
{
protected:

	/// reference to material properties
	const property& 	property_;

	/// reference to shapes 
	sphereShape& 		shapes_;

	/// pointField of inertial of particles 
	realPointField_D&     	I_;

	/// pointField of rotational Velocity of particles on device 
	realx3PointField_D& 	rVelocity_;

	/// pointField of rotational acceleration of particles on device  
	realx3PointField_D& 	rAcceleration_;	

	/// rotational velocity integrator 
	uniquePtr<integration>  rVelIntegration_ = nullptr;

	/// timer for acceleration computations 
	Timer 					accelerationTimer_;

	/// timer for integration computations (prediction step)
	Timer 					intPredictTimer_;

	/// timer for integration computations (correction step)
	Timer 					intCorrectTimer_;

	bool diameterMassInertiaPropId(const word& shName, real& diam, real& mass, real& I, int8& propIdx);

	bool initializeParticles();

	bool insertSphereParticles(const wordVector& names, const int32IndexContainer& indices);

	virtual uniquePtr<List<eventObserver*>> getFieldObjectList()const override;

public:

	/// construct from systemControl and property 
	sphereParticles(systemControl &control, const property& prop);

	/// no copy constructor 
	sphereParticles(const sphereParticles&) = delete;

	/// move constructor 
	sphereParticles(sphereParticles&&) = default;

	/// no copy-assignement
	sphereParticles& operator=(const sphereParticles&) = delete;

	/// move-assignement
	sphereParticles& operator=(sphereParticles&&) = default;

	virtual ~sphereParticles()=default;

	/**
	 * Insert new particles in position with specified shapes
	 * 
	 * This function is involked by inserted object to insert new set of particles
	 * into the simulation. 
	 * \param position position of new particles
	 * \param shape shape of new particles
	 * \param setField initial value of the selected fields for new particles
	 */
	bool insertParticles
	(
		const realx3Vector& position,
	 	const wordVector&  shapes,
	 	const setFieldList& setField
	) override ;

	/// const reference to shapes object 
	const auto& shapes()const
	{
		return shapes_;
	}

	/// const reference to inertia pointField
	const auto& I()const
	{
		return I_;
	}

	/// reference to inertia pointField 
	auto& I()
	{
		return I_;
	}
	
	const realx3Vector_D rVelocity()const 
	{	
		return rVelocity_;
	}

	const realVector_D& boundingSphere()const override
	{
		return this->diameter();
	}

	word shapeTypeName() const override
	{
		return "sphere";
	}

	void boundingSphereMinMax(
		real& minDiam,
		real& maxDiam )const override
	{
		shapes_.diameterMinMax(minDiam, maxDiam);
	}

	bool update(const eventMessage& msg) override;
	
	
	realx3PointField_D& rAcceleration() override
	{
		return rAcceleration_;
	}
	
	const realx3PointField_D& rAcceleration() const override
	{
		return rAcceleration_;
	}

	/// before iteration step 
	bool beforeIteration() override;

	/// iterate particles 
	bool iterate() override;	

	/// after iteration step
	bool afterIteration() override;
	

	
}; //sphereParticles

} // pFlow

#endif //__sphereParticles_hpp__
