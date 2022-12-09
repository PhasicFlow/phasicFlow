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


class AdamsBashforth2
:
	public integration
{
protected:

	realx3PointField_D& dy1_;

	using rpIntegration = Kokkos::RangePolicy<
			DefaultExecutionSpace,
			Kokkos::Schedule<Kokkos::Static>,
			Kokkos::IndexType<int32>
			>;
public:

	// type info
	TypeInfo("AdamsBashforth2");

	//// - Constructors
		AdamsBashforth2(
			const word& baseName,
			repository& owner,
			const pointStructure& pStruct,
			const word& method);

		virtual ~AdamsBashforth2()=default;

		// - add a virtual constructor 
		add_vCtor(
			integration,
			AdamsBashforth2,
			word);


	//// - Methods
		bool predict(real UNUSED(dt), realx3Vector_D& UNUSED(y), realx3Vector_D& UNUSED(dy)) override;

		bool correct(real dt, realx3Vector_D& y, realx3Vector_D& dy) override;

		bool setInitialVals(
			const int32IndexContainer& newIndices,
			const realx3Vector& y) override;

		bool needSetInitialVals()const override
		{
			return false;
		}

		uniquePtr<integration> clone()const override
		{
			return makeUnique<AdamsBashforth2>(*this);
		}

		bool intAll(real dt, realx3Vector_D& y, realx3Vector_D& dy, range activeRng);

		template<typename activeFunctor>
		bool intRange(real dt, realx3Vector_D& y, realx3Vector_D& dy, activeFunctor activeP );

};

template<typename activeFunctor>
bool pFlow::AdamsBashforth2::intRange(
	real dt, 
	realx3Vector_D& y,
	realx3Vector_D& dy,
	activeFunctor activeP )
{

	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_dy1= dy1_.deviceVectorAll();
	auto activeRng = activeP.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				d_y[i] +=  dt*(static_cast<real>(3.0 / 2.0) * d_dy[i] - static_cast<real>(1.0 / 2.0) * d_dy1[i]);
				d_dy1[i] = d_dy[i];
			}
		});
	Kokkos::fence();


	return true;
}

} // pFlow

#endif //__integration_hpp__
