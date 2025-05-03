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

#include "AdamsBashforth4.hpp"

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
	realx3PointField_D& dy1,
	realx3PointField_D& dy2,
	realx3PointField_D& dy3,
	real damping = 1.0)
{

	auto d_dy = dy.deviceView();
	auto d_y  = y.deviceView();
	auto d_dy1= dy1.deviceView();
	auto d_dy2= dy2.deviceView();
	auto d_dy3= dy3.deviceView();
	auto activeRng = dy1.activeRange();

	Kokkos::parallel_for(
		"AdamsBashforth4::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
			d_y[i] += damping * dt*( 
				  static_cast<real>(55.0 / 24.0) * d_dy[i]
				- static_cast<real>(59.0 / 24.0) * d_dy1[i] 
				+ static_cast<real>(37.0 / 24.0) * d_dy2[i]
				- static_cast<real>( 9.0 / 24.0) * d_dy3[i]);

			d_dy3[i] = d_dy2[i];
			d_dy2[i] = d_dy1[i];
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
	realx3PointField_D& dy1,
	realx3PointField_D& dy2,
	realx3PointField_D& dy3,
	real damping = 1.0
)
{

	auto d_dy 		= dy.deviceView();
	auto d_y  		= y.deviceView();
	auto d_dy1		= dy1.deviceView();
	auto d_dy2		= dy2.deviceView();
	auto d_dy3		= dy3.deviceView();
	auto activeRng 	= dy1.activeRange();
	const auto& activeP = dy1.activePointsMaskDevice();

	Kokkos::parallel_for(
		"AdamsBashforth4::correct",
		rpIntegration (activeRng.start(), activeRng.end()),
		LAMBDA_HD(uint32 i){
			if( activeP(i))
			{
				d_y[i] += damping*  dt*( 
				  static_cast<real>(55.0 / 24.0) * d_dy[i]
				- static_cast<real>(59.0 / 24.0) * d_dy1[i] 
				+ static_cast<real>(37.0 / 24.0) * d_dy2[i]
				- static_cast<real>( 9.0 / 24.0) * d_dy3[i] );
				d_dy3[i] = d_dy2[i];
				d_dy2[i] = d_dy1[i];
				d_dy1[i] = d_dy[i];
			}
		});
	Kokkos::fence();


	return true;
}

}

pFlow::AdamsBashforth4::AdamsBashforth4
(
	const word& baseName,
	pointStructure& pStruct,
	const word& method,
	const realx3Field_D& initialValField,
	bool keepHistory
)
:
	AdamsBashforth3(baseName, pStruct, method, initialValField, keepHistory),
	dy3_
	(
		objectFile
		(
			groupNames(baseName,"dy3"),
			pStruct.time().integrationFolder(),
			objectFile::READ_IF_PRESENT,
			keepHistory?objectFile::WRITE_ALWAYS:objectFile::WRITE_NEVER
		),
		pStruct,
		zero3,
		zero3
	)
{}

void pFlow::AdamsBashforth4::updateBoundariesSlaveToMasterIfRequested()
{
	AdamsBashforth3::updateBoundariesSlaveToMasterIfRequested();
	dy3_.updateBoundariesSlaveToMasterIfRequested();
}

bool pFlow::AdamsBashforth4::correct
(
	real dt, 
	realx3PointField_D& y, 
	realx3PointField_D& dy,
	real damping
)
{
	
	bool success = false;
	if(y.isAllActive())
	{
		success = intAllActive(dt, y.field(), dy, dy1(), dy2(), dy3(), damping);
	}
	else
	{
		success = intScattered(dt, y.field(), dy, dy1(), dy2(), dy3(), damping);
	}

	success = success && boundaryList().correct(dt, y, dy);

	return success;
}

bool pFlow::AdamsBashforth4::correctPStruct(
	real dt, 
	pointStructure &pStruct, 
	realx3PointField_D &vel)
{
	
	bool success = false;
	if(dy2().isAllActive())
	{
		success = intAllActive(dt, pStruct.pointPosition(), vel, dy1(), dy2(), dy3());
	}
	else
	{
		success = intScattered(dt, pStruct.pointPosition(), vel, dy1(), dy2(), dy3());
	}

	success = success && boundaryList().correctPStruct(dt, pStruct, vel);

	return success;
}

/*bool pFlow::AdamsBashforth4::hearChanges
(
	const timeInfo &ti, 
	const message &msg, 
	const anyList &varList
)
{
	if(msg.equivalentTo(message::ITEMS_INSERT))
	{
		return insertValues(varList, initialValField().deviceViewAll(), dy1()) &&
		insertValues(varList, initialValField().deviceViewAll(), dy2()) && 
		insertValues(varList, initialValField().deviceViewAll(), dy3());
	}
	else
	{
		return realx3PointField_D::hearChanges(ti, msg, varList);
	}
}*/
