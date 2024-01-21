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

#include "AdamsBashforth2.hpp"
#include "pointStructure.hpp"
#include "Time.hpp"
#include "vocabs.hpp"

namespace pFlow
{

/// Range policy for integration kernel (alias)
using rpIntegration = Kokkos::RangePolicy<
		DefaultExecutionSpace,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<uint32>
		>;

bool intAllActive(
	real dt, 
	realx3PointField_D& y, 
	realx3PointField_D& dy,
	realx3PointField_D& dy1)
{

	auto d_dy = dy.fieldDevice();
	auto d_y  = y.fieldDevice();
	auto d_dy1= dy1.fieldDevice();
	auto activeRng = dy1.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(int32 i){
			d_y[i] +=  dt*(static_cast<real>(1.5) * d_dy[i] - static_cast<real>(0.5) * d_dy1[i]);
			d_dy1[i] = d_dy[i];
		});
	Kokkos::fence();

	return true;	
}

bool intScattered
(
	real dt, 
	realx3PointField_D& y,
	realx3PointField_D& dy,
	realx3PointField_D& dy1
)
{

	auto d_dy 		= dy.fieldDevice();
	auto d_y  		= y.fieldDevice();
	auto d_dy1		= dy1.fieldDevice();
	auto activeRng 	= dy1.activeRange();
	const auto& activeP = dy1.activePointsMaskDevice();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(int32 i){
			if( activeP(i))
			{
				d_y[i] +=  dt*(static_cast<real>(1.5) * d_dy[i] - static_cast<real>(0.5) * d_dy1[i]);
				d_dy1[i] = d_dy[i];
			}
		});
	Kokkos::fence();


	return true;
}

}



pFlow::AdamsBashforth2::AdamsBashforth2
(
	const word& baseName,
	pointStructure& pStruct,
	const word& method,
	const realx3Field_D& initialValField
)
:
	integration(baseName, pStruct, method, initialValField),
	realx3PointField_D
	(
		objectFile
		(
			groupNames(baseName,"dy1"),
			pStruct.time().integrationFolder(),
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		pStruct,
		zero3
	)
{}

bool pFlow::AdamsBashforth2::predict
(
	real UNUSED(dt),
	realx3PointField_D& UNUSED(y),
	realx3PointField_D& UNUSED(dy)
)
{

	return true;
}

bool pFlow::AdamsBashforth2::correct
(
	real dt,
	realx3PointField_D& y,
	realx3PointField_D& dy
)
{
	auto& dy1l = dy1();

	if(dy1l.isAllActive())
	{
		return intAllActive(dt, y, dy, dy1l);
	}
	else
	{
		return intScattered(dt, y, dy, dy1l);
	}

	return true;
}

bool pFlow::AdamsBashforth2::setInitialVals(
	const int32IndexContainer& newIndices,
	const realx3Vector& y)
{
	return true;
}

