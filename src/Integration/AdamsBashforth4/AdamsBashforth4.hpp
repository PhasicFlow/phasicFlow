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


#include "integration.hpp"
#include "pointFields.hpp"

namespace pFlow
{

struct AB4History
{
	realx3 dy1_={0,0,0};
	realx3 dy2_={0,0,0};
	realx3 dy3_={0,0,0};

	TypeInfoNV("AB4History");
};


INLINE_FUNCTION
iIstream& operator>>(iIstream& str, AB4History& ab4)
{
	str.readBegin("AB4History");

    str >> ab4.dy1_;
    str >> ab4.dy2_;
    str >> ab4.dy3_;
    
    str.readEnd("AB4History");

    str.check(FUNCTION_NAME);

	return str;
	
}

INLINE_FUNCTION
iOstream& operator<<(iOstream& str, const AB4History& ab4)
{
	str << token::BEGIN_LIST << ab4.dy1_
	    << token::SPACE << ab4.dy2_
	    << token::SPACE << ab4.dy3_
		<< token::END_LIST;	    

	str.check(FUNCTION_NAME);

	return str;
}


class AdamsBashforth4
:
	public integration
{
protected:

	using HistoryFieldType = pointField<VectorSingle,AB4History>;
	//realx3PointField_D& dy1_;

	//realx3PointField_D& dy2_;



	//  this is a device 
	HistoryFieldType& history_;

	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;

public:

	// type info
	TypeInfo("AdamsBashforth4");

	//// - Constructors
		AdamsBashforth4(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		virtual ~AdamsBashforth4()=default;

		// - add a virtual constructor 
		add_vCtor(
			integration,
			AdamsBashforth4,
			word);


	//// - Methods
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

		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsBashforth4>(*this);
		}

		bool intAll(real dt, 
			realx3Vector_D& y, 
			realx3Vector_D& dy, 
			range activeRng);

		template<typename activeFunctor>
		bool intRange(real dt,
			realx3Vector_D& y,
			realx3Vector_D& dy,
			activeFunctor activeP );

};


template<typename activeFunctor>
bool pFlow::AdamsBashforth4::intRange(
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
}

} // pFlow

#endif //__integration_hpp__
