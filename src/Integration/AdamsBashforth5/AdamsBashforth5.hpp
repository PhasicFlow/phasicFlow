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

#ifndef __AdamsBashforth5_hpp__
#define __AdamsBashforth5_hpp__


#include "integration.hpp"
#include "pointFields.hpp"

namespace pFlow
{

struct AB5History
{
	TypeInfoNV("AB5History");

	realx3 dy1_={0,0,0};
	realx3 dy2_={0,0,0};
	realx3 dy3_={0,0,0};
	realx3 dy4_={0,0,0};
};


INLINE_FUNCTION
iIstream& operator>>(iIstream& str, AB5History& ab5)
{
	str.readBegin("AB5History");

    str >> ab5.dy1_;
    str >> ab5.dy2_;
    str >> ab5.dy3_;
    str >> ab5.dy4_;
    
    str.readEnd("AB5History");

    str.check(FUNCTION_NAME);

	return str;
	
}

INLINE_FUNCTION
iOstream& operator<<(iOstream& str, const AB5History& ab5)
{
	str << token::BEGIN_LIST << ab5.dy1_
	    << token::SPACE << ab5.dy2_
	    << token::SPACE << ab5.dy3_
	    << token::SPACE << ab5.dy4_
		<< token::END_LIST;	    

	str.check(FUNCTION_NAME);

	return str;
}

/**
 * Fifth order Adams-Bashforth integration method for solving ODE
 * 
 * This is a one-step integration method and does not have prediction step.
 */
class AdamsBashforth5
:
	public integration
{
protected:

	/// Integration history
	pointField<VectorSingle,AB5History>& history_;

	/// Range policy for integration kernel
	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;

public:

	/// Type info
	TypeInfo("AdamsBashforth5");

	// - Constructors
		
		AdamsBashforth5(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsBashforth5>(*this);
		}

		virtual ~AdamsBashforth5()=default;

		/// Add this to the virtual constructor table 
		add_vCtor(
			integration,
			AdamsBashforth5,
			word);


	// - Methods
		
		bool predict(
			real UNUSED(dt),
			realx3Vector_D & UNUSED(y),
			realx3Vector_D& UNUSED(dy)) override;

		bool correct(
			real dt, 
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
bool pFlow::AdamsBashforth5::intRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{
	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_history = history_.deviceVectorAll();
	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth5::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				
				d_y[i] += dt*( 
							static_cast<real>(1901.0 / 720.0) * d_dy[i]
						- static_cast<real>(2774.0 / 720.0) * d_history[i].dy1_ 
						+ static_cast<real>(2616.0 / 720.0) * d_history[i].dy2_
						- static_cast<real>(1274.0 / 720.0) * d_history[i].dy3_
						+ static_cast<real>( 251.0 / 720.0) * d_history[i].dy4_
						);
				d_history[i] = {d_dy[i] ,d_history[i].dy1_, d_history[i].dy2_, d_history[i].dy3_};
			}
		});
	Kokkos::fence();

	return true;
}

} // pFlow

#endif //
