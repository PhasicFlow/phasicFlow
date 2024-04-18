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

#ifndef __AdamsMoulton4_hpp__
#define __AdamsMoulton4_hpp__


#include "integration.hpp"
#include "pointFields.hpp"

namespace pFlow
{

/**
 * Fourth order Adams-Moulton integration method for solving ODE
 * 
 * This is a predictor-corrector integration method.
 */
class AdamsMoulton4
:
	public integration
{
protected:

	/// y at time t
	realx3PointField_D& y0_;

	/// dy at time t
	realx3PointField_D& dy0_;

	/// dy at time t-dt
	realx3PointField_D& dy1_;

	/// dy at time t-2*dt
	realx3PointField_D& dy2_;

	/// Range policy for integration kernel 
	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;
public:

	/// Type info
	TypeInfo("AdamsMoulton4");

	// - Constructors
		
		/// Construct from components
		AdamsMoulton4(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsMoulton4>(*this);
		}

		/// Destructor 
		virtual ~AdamsMoulton4()=default;

		// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsMoulton4,
			word);


	// - Methods

		bool predict(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy) override;

		bool correct(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy) override;

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) override;

		bool needSetInitialVals()const override
		{
			return true;
		}

		/// Prediction step on all points in the active range
		bool predictAll(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy, 
			range activeRng);

		/// Prediction step on active points in the active range
		template<typename activeFunctor>
		bool predictRange(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy, 
			activeFunctor activeP);

		/// Integrate on all points in the active range
		bool intAll(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy, 
			range activeRng);

		/// Integrate on active points in the active range
		template<typename activeFunctor>
		bool intRange(
			real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy, 
			activeFunctor activeP );

};


template<typename activeFunctor>
bool AdamsMoulton4::predictRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{
	auto d_dy = dy.deviceViewAll();
	auto d_y  = y.deviceViewAll();

	auto d_y0 = y0_.deviceViewAll();
	auto d_dy0 = dy0_.deviceViewAll();
	auto d_dy1 = dy1_.deviceViewAll();
	auto d_dy2 = dy2_.deviceViewAll();
	
	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsMoulton4::predictRange",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if(activeP(i))
			{
				d_dy0[i] = d_dy[i];
				d_y[i] = d_y0[i] + dt*(
				  static_cast<real>(23.0 /12.0 ) * d_dy[i] 
				- static_cast<real>(16.0 / 12.0) * d_dy1[i]
				+ static_cast<real>( 5.0 / 12.0) * d_dy2[i]);
			}
		});
	Kokkos::fence();

	return true;

}


template<typename activeFunctor>
bool pFlow::AdamsMoulton4::intRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{

	auto d_dy = dy.deviceViewAll();
	auto d_y  = y.deviceViewAll();

	auto d_dy0 = dy0_.deviceViewAll();
	auto d_y0  = y0_.deviceViewAll();
	auto d_dy1 = dy1_.deviceViewAll();
	auto d_dy2 = dy2_.deviceViewAll();

	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsMoulton4::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				auto corrct_y = d_y0[i] + dt*(
					  static_cast<real>(9.0/24.0)*d_dy[i] 
					+ static_cast<real>(19.0/24.0)*d_dy0[i] 
					- static_cast<real>( 5.0/24.0)*d_dy1[i]
					+ static_cast<real>( 1.0/24.0)*d_dy2[i]);
				
				d_dy2[i]= d_dy1[i];
				d_dy1[i]= d_dy0[i];
				d_y0[i] = corrct_y;
				d_y[i]  = corrct_y;
			}
		});
	Kokkos::fence();


	return true;
}

} // pFlow

#endif //
