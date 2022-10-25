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

#ifndef __AdamsMoulton3_H__
#define __AdamsMoulton3_H__


#include "integration.H"
#include "pointFields.H"

namespace pFlow
{


class AdamsMoulton3
:
	public integration
{
protected:

	realx3PointField_D& y0_;

	realx3PointField_D& dy0_;

	realx3PointField_D& dy1_;

	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;
public:

	// type info
	TypeName("AdamsMoulton3");

	//// - Constructors
		AdamsMoulton3(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		virtual ~AdamsMoulton3()=default;

		// - add a virtual constructor 
		add_vCtor(
			integration,
			AdamsMoulton3,
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
			return makeUnique<AdamsMoulton3>(*this);
		}

		bool predictAll(real dt, realx3Vector_D& y, realx3Vector_D& dy, range activeRng);

		template<typename activeFunctor>
		bool predictRange(real dt, realx3Vector_D& y, realx3Vector_D& dy, activeFunctor activeP);

		bool intAll(real dt, realx3Vector_D& y, realx3Vector_D& dy, range activeRng);

		template<typename activeFunctor>
		bool intRange(real dt, realx3Vector_D& y, realx3Vector_D& dy, activeFunctor activeP );

};


template<typename activeFunctor>
bool AdamsMoulton3::predictRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{
	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_y0 = y0_.deviceVectorAll();
	auto d_dy0 = dy0_.deviceVectorAll();
	auto d_dy1= dy1_.deviceVectorAll();
	
	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsMoulton3::predictRange",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if(activeP(i))
			{
				d_dy0[i] = d_dy[i];
				d_y[i] = d_y0[i] + 
				dt*
				(
					static_cast<real>(3.0 / 2.0) * d_dy[i] 
					- static_cast<real>(1.0 / 2.0) * d_dy1[i]
				);
			}
		});
	Kokkos::fence();

	return true;

}


template<typename activeFunctor>
bool pFlow::AdamsMoulton3::intRange(
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

	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsMoulton3::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				auto corrct_y = d_y0[i] + dt*(
					  static_cast<real>(5.0/12.0)*d_dy[i] 
					+ static_cast<real>(8.0/12.0)*d_dy0[i] 
					- static_cast<real>(1.0/12.0)*d_dy1[i]);
				d_y[i]  = corrct_y;
				d_y0[i] = corrct_y;
				d_dy1[i]= d_dy0[i];
			}
		});
	Kokkos::fence();


	return true;
}

} // pFlow

#endif //__integration_H__
