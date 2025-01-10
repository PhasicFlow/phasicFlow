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


#ifndef __dynamicPointStructure_hpp__
#define __dynamicPointStructure_hpp__

#include "Time.hpp"
#include "pointFields.hpp"
#include "integration.hpp"
#include "uniquePtr.hpp"

namespace pFlow
{

class systemControl;

class dynamicPointStructure
:
	public pointStructure
{
private:

	realx3PointField_D  	velocity_;

	uniquePtr<integration>  integrationPos_ = nullptr;

	uniquePtr<integration>  integrationVel_ = nullptr;

	Timer 					velocityUpdateTimer_;

	/// @brief integration method for velocity and position
	word 					integrationMethod_;

public:

	TypeInfo("dynamicPointStructure");

	explicit dynamicPointStructure(systemControl& control);

	dynamicPointStructure(const dynamicPointStructure& ps) = delete;
		
	// - no move construct 
	dynamicPointStructure(dynamicPointStructure&&) = delete;

	/// 
	dynamicPointStructure& operator=(const dynamicPointStructure&) = delete;

	// - no move assignment 
	dynamicPointStructure& operator=(dynamicPointStructure&&) = delete;

	// - destructor 
	~dynamicPointStructure() override = default;


	inline 
	const realx3PointField_D& velocity()const
	{
		return velocity_;
	}

	inline 
	realx3PointField_D& velocity()
	{
		return velocity_;
	}

	/// In the time loop before iterate
	bool beforeIteration() override;

	/// @brief This is called in time loop. Perform the main calculations 
	/// when the component should evolve along time.
	bool iterate() override;
	
	/// prediction step (if any), is called in beforeIteration	
	bool predict(real dt, realx3PointField_D& acceleration);

	/// correction step, is called in iterate 
	bool correct(real dt, realx3PointField_D& acceleration);

};

}


#endif
