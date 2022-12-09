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


class AdamsMoulton4
:
	public integration
{
protected:

	realx3PointField_D& y0_;

	realx3PointField_D& dy0_;

	realx3PointField_D& dy1_;

	realx3PointField_D& dy2_;

	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;
public:

	// type info
	TypeInfo("AdamsMoulton4");

	//// - Constructors
		AdamsMoulton4(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		virtual ~AdamsMoulton4()=default;

		// - add a virtual constructor 
		add_vCtor(
			integration,
			AdamsMoulton4,
			word);


	//// - Methods
		bool predict(real dt, realx3Vector_D& y, realx3Vector_D& dy) override;

		bool correct(real dt, realx3Vector_D& y, realx3Vector_D& dy) override;

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) override;

		bool needSetInitialVals()const override
		{
			return true;
		}

		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsMoulton4>(*this);
		}

		bool predictAll(real dt, realx3Vector_D& y, realx3Vector_D& dy, range activeRng);

		template<typename activeFunctor>
		bool predictRange(real dt, realx3Vector_D& y, realx3Vector_D& dy, activeFunctor activeP);

		bool intAll(real dt, realx3Vector_D& y, realx3Vector_D& dy, range activeRng);

		template<typename activeFunctor>
		bool intRange(real dt, realx3Vector_D& y, realx3Vector_D& dy, activeFunctor activeP );

};


template<typename activeFunctor>
bool AdamsMoulton4::predictRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{
	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();

	auto d_y0 = y0_.deviceVectorAll();
	auto d_dy0 = dy0_.deviceVectorAll();
	auto d_dy1 = dy1_.deviceVectorAll();
	auto d_dy2 = dy2_.deviceVectorAll();
	
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

	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();

	auto d_dy0 = dy0_.deviceVectorAll();
	auto d_y0  = y0_.deviceVectorAll();
	auto d_dy1 = dy1_.deviceVectorAll();
	auto d_dy2 = dy2_.deviceVectorAll();

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

#endif //__integration_hpp__
