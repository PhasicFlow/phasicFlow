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

#ifndef __AdamsBashforth2_hpp__
#define __AdamsBashforth2_hpp__


#include "integration.hpp"
#include "pointFields.hpp"

namespace pFlow
{

/**
 * Second order Adams-Bashforth integration method for solving ODE
 * 
 * This is a one-step integration method and does not have prediction step.
 * 
 */
class AdamsBashforth2
:
	public integration,
	public realx3PointField_D
{
private:

	auto& dy1()
	{
		return static_cast<realx3PointField_D&>(*this);
	}
	
public:

	/// Type info
	TypeInfo("AdamsBashforth2");

	// - Constructors

		/// Construct from components
		AdamsBashforth2(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField);

		/// Destructor 
		~AdamsBashforth2()final = default;

		/// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth2,
			word);


	// - Methods
		/// return integration method 
		word method()const override
		{
			return "AdamsBashforth2";
		}

		bool predict(
			real UNUSED(dt), 
			realx3PointField_D& UNUSED(y), 
			realx3PointField_D& UNUSED(dy)) final;
		
		bool predict(
			real dt, 
			realx3Field_D& y, 
			realx3PointField_D& dy) final;

		bool correct(
			real dt, 
			realx3PointField_D& y, 
			realx3PointField_D& dy) final;

		bool correct(
			real dt, 
			realx3Field_D& y, 
			realx3PointField_D& dy);
		

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) final;

		bool needSetInitialVals()const final
		{
			return false;
		}

};


} // pFlow

#endif //__integration_hpp__
