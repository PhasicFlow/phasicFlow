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

#ifndef __AdamsBashforth3_hpp__
#define __AdamsBashforth3_hpp__


#include "AdamsBashforth2.hpp"
#include "pointFields.hpp"
#include "boundaryIntegrationList.hpp"


namespace pFlow
{


/**
 * Third order Adams-Bashforth integration method for solving ODE
 * 
 * This is a one-step integration method and does not have prediction step.
 */
class AdamsBashforth3
:
	public AdamsBashforth2
{
private:
	
	friend class processorAB3BoundaryIntegration;

	realx3PointField_D 		dy2_;

protected:

	const auto& dy2()const
	{
		return dy2_;
	}

	auto& dy2()
	{
		return dy2_;
	}
	

public:

	// type info
	TypeInfo("AdamsBashforth3");

	// - Constructors
		
		/// Construct from components
		AdamsBashforth3(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField,
			bool  keepHistory);
		

		/// Destructor 
		~AdamsBashforth3() override =default;

		/// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth3,
			word);


	// - Methods

		void updateBoundariesSlaveToMasterIfRequested()override;

		/// return integration method 
		word method()const override
		{
			return "AdamsBashforth3";
		}

		

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


/*template<typename activeFunctor>
bool pFlow::AdamsBashforth3::intRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{
	auto d_dy = dy.deviceViewAll();
	auto d_y  = y.deviceViewAll();
	auto d_history = history_.deviceViewAll();
	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth3::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				auto ldy = d_dy[i];
				d_y[i] += dt*( static_cast<real>(23.0 / 12.0) * ldy 
						- static_cast<real>(16.0 / 12.0) * d_history[i].dy1_ 
						+ static_cast<real>(5.0 / 12.0) * d_history[i].dy2_);
				d_history[i] = {ldy ,d_history[i].dy1_};
			}
		});
	Kokkos::fence();

	return true;
}*/

} // pFlow

#endif //__integration_hpp__
