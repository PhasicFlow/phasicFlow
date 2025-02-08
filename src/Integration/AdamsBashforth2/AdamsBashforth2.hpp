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
#include "boundaryIntegrationList.hpp"

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

	const realx3Field_D& initialValField_;

	boundaryIntegrationList boundaryList_;

	friend class processorAB2BoundaryIntegration;

protected:
	
	const auto& dy1()const
	{
		return static_cast<const realx3PointField_D&>(*this);
	}

	auto& dy1()
	{
		return static_cast<realx3PointField_D&>(*this);
	}

	auto& initialValField()
	{
		return initialValField_;
	}

	boundaryIntegrationList& boundaryList()
	{
		return boundaryList_;
	}
	
public:

	/// Class info
	ClassInfo("AdamsBashforth2");

	// - Constructors

		/// Construct from components
		AdamsBashforth2(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField);

		/// Destructor 
		~AdamsBashforth2()override = default;

		/// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth2,
			word);


	// - Methods

		void updateBoundariesSlaveToMasterIfRequested()override;

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
			realx3PointField_D& dy,
			real damping = 1.0) override;

		bool correctPStruct(
			real dt, 
			pointStructure& pStruct, 
			realx3PointField_D& vel) override;
		
		/*bool hearChanges
		(
			const timeInfo& ti,
			const message& msg, 
			const anyList& varList
		) override;*/

};


} // pFlow

#endif //__integration_hpp__
