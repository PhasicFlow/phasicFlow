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
	realx3Field_D& y, 
	realx3PointField_D& dy,
	realx3PointField_D& dy1)
{

	auto d_dy = dy.deviceView();
	auto d_y  = y.deviceView();
	auto d_dy1= dy1.deviceView();
	auto activeRng = dy1.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
			d_y[i] +=  dt*(static_cast<real>(1.5) * d_dy[i] - static_cast<real>(0.5) * d_dy1[i]);
			d_dy1[i] = d_dy[i];
		});
	Kokkos::fence();

	return true;	
}

bool intScattered
(
	real dt, 
	realx3Field_D& y,
	realx3PointField_D& dy,
	realx3PointField_D& dy1
)
{

	auto d_dy 		= dy.deviceView();
	auto d_y  		= y.deviceView();
	auto d_dy1		= dy1.deviceView();
	auto activeRng 	= dy1.activeRange();
	const auto& activeP = dy1.activePointsMaskDevice();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
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
		zero3,
		zero3
	),
	boundaryList_(pStruct, method, *this)
{}

void pFlow::AdamsBashforth2::updateBoundariesSlaveToMasterIfRequested()
{
	realx3PointField_D::updateBoundariesSlaveToMasterIfRequested();
}

bool pFlow::AdamsBashforth2::predict(
    real UNUSED(dt),
    realx3PointField_D &UNUSED(y),
    realx3PointField_D &UNUSED(dy))
{
	return true;
}

bool pFlow::AdamsBashforth2::predict
(
	real dt, 
	realx3Field_D &y, 
	realx3PointField_D &dy
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
	bool success = false;
	if(dy1l.isAllActive())
	{
		success = intAllActive(dt, y.field(), dy, dy1l);
	}
	else
	{
		success = intScattered(dt, y.field(), dy, dy1l);
	}

	success = success && boundaryList_.correct(dt, y, dy);

	return success;

}

bool pFlow::AdamsBashforth2::correctPStruct(
	real dt, 
	pointStructure &pStruct, 
	realx3PointField_D &vel)
{
	auto& dy1l = dy1();
	bool success = false;
	if(dy1l.isAllActive())
	{
		success = intAllActive(dt, pStruct.pointPosition(), vel, dy1l);
	}
	else
	{
		success = intScattered(dt, pStruct.pointPosition(), vel, dy1l);
	}

	success = success && boundaryList_.correctPStruct(dt, pStruct, vel);

	return success;
}


bool pFlow::AdamsBashforth2::setInitialVals(
	const int32IndexContainer& newIndices,
	const realx3Vector& y)
{
	return true;
}

