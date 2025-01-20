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

#ifndef __AdamsBashforth4_hpp__
#define __AdamsBashforth4_hpp__


#include "AdamsBashforth3.hpp"
#include "pointFields.hpp"
#include "boundaryIntegrationList.hpp"


namespace pFlow
{

/**
 * Fourth order Adams-Bashforth integration method for solving ODE
 * 
 * This is a one-step integration method and does not have prediction step.
 */
class AdamsBashforth4
:
	public AdamsBashforth3
{
private:

	friend class processorAB4BoundaryIntegration;

	realx3PointField_D 		dy3_;

protected:

	const auto& dy3()const
	{
		return dy3_;
	}

	auto& dy3()
	{
		return dy3_;
	}

public:

	/// Type info
	TypeInfo("AdamsBashforth4");

	// - Constructors
		
		/// Construct from components
		AdamsBashforth4(
			const word& baseName,
			pointStructure& pStruct,
			const word& method,
			const realx3Field_D& initialValField);
		
		

		/// Destructor
		~AdamsBashforth4() override =default;

		/// Add a this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth4,
			word);


	// - Methods
		
		void updateBoundariesSlaveToMasterIfRequested()override;

		/// return integration method 
		word method()const override
		{
			return "AdamsBashforth4";
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

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) override;

		bool needSetInitialVals()const override
		{
			return false;
		}

		

};


/*template<typename activeFunctor>
bool pFlow::AdamsBashforth4::intRange(
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
		"AdamsBashforth4::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				
				d_y[i] += dt*( 
							static_cast<real>(55.0 / 24.0) * d_dy[i]
						- static_cast<real>(59.0 / 24.0) * d_history[i].dy1_ 
						+ static_cast<real>(37.0 / 24.0) * d_history[i].dy2_
						- static_cast<real>( 9.0 / 24.0) * d_history[i].dy3_
						);
				d_history[i].dy3_ = d_history[i].dy2_;
				d_history[i].dy2_ = d_history[i].dy1_;
				d_history[i].dy1_ = d_dy[i];
			}
		});
	Kokkos::fence();

	return true;
}*/

} // pFlow

#endif //__integration_hpp__
