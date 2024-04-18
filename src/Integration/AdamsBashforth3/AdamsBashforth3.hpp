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


#include "integration.hpp"
#include "pointFields.hpp"

namespace pFlow
{

struct AB3History
{
	TypeInfoNV("AB3History");

	realx3 dy1_={0,0,0};
	realx3 dy2_={0,0,0};
};


INLINE_FUNCTION
iIstream& operator>>(iIstream& str, AB3History& ab3)
{
	str.readBegin("AB3History");

    str >> ab3.dy1_;
    str >> ab3.dy2_;
    
    str.readEnd("AB3History");

    str.check(FUNCTION_NAME);

	return str;
	
}

INLINE_FUNCTION
iOstream& operator<<(iOstream& str, const AB3History& ab3)
{
	str << token::BEGIN_LIST << ab3.dy1_
	    << token::SPACE << ab3.dy2_
		<< token::END_LIST;	    

	str.check(FUNCTION_NAME);

	return str;
}

/**
 * Third order Adams-Bashforth integration method for solving ODE
 * 
 * This is a one-step integration method and does not have prediction step.
 */
class AdamsBashforth3
:
	public integration
{
protected:
	
	/// Integration history
	pointField<VectorSingle,AB3History>& history_;

	/// Range policy for integration kernel
	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;

public:

	// type info
	TypeInfo("AdamsBashforth3");

	// - Constructors
		
		/// Construct from components
		AdamsBashforth3(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);
		
		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsBashforth3>(*this);
		}

		/// Destructor 
		virtual ~AdamsBashforth3()=default;

		/// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth3,
			word);


	// - Methods

		bool predict(
			real UNUSED(dt),
			realx3Vector_D & UNUSED(y),
			realx3Vector_D& UNUSED(dy)) override;

		bool correct(real dt, 
			realx3Vector_D & y,
			realx3Vector_D& dy) override;

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) override;

		bool needSetInitialVals()const override
		{
			return false;
		}

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
}

} // pFlow

#endif //__integration_hpp__
