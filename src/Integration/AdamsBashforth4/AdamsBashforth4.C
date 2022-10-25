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

#include "AdamsBashforth4.H"



pFlow::AdamsBashforth4::AdamsBashforth4
(
	const word& baseName,
	repository& owner,
	const pointStructure& pStruct,
	const word& method
)
:
	integration(baseName, owner, pStruct, method),
	history_(
		owner.emplaceObject<HistoryFieldType>(
			objectFile(
				groupNames(baseName,"AB4History"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			AB4History({zero3,zero3, zero3})))

{

}

bool pFlow::AdamsBashforth4::predict
(
	real UNUSED(dt),
	realx3Vector_D& UNUSED(y),
	realx3Vector_D& UNUSED(dy)
)
{

	return true;
}

bool pFlow::AdamsBashforth4::correct
(
	real dt,
	realx3Vector_D& y,
	realx3Vector_D& dy
)
{
	
	if(this->pStruct().allActive())
	{
		return intAll(dt, y, dy, this->pStruct().activeRange());
	}
	else
	{
		return intRange(dt, y, dy, this->pStruct().activePointsMaskD());
	}

	return true;
}

bool pFlow::AdamsBashforth4::setInitialVals(
	const int32IndexContainer& newIndices,
	const realx3Vector& y)
{
	return true;
}

bool pFlow::AdamsBashforth4::intAll(
	real dt, 
	realx3Vector_D& y, 
	realx3Vector_D& dy, 
	range activeRng)
{
	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_history = history_.deviceVectorAll();

	Kokkos::parallel_for(
		"AdamsBashforth4::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){				
			d_y[i] += dt*( 
						static_cast<real>(55.0 / 24.0) * d_dy[i]
					- static_cast<real>(59.0 / 24.0) * d_history[i].dy1_ 
					+ static_cast<real>(37.0 / 24.0) * d_history[i].dy2_
					- static_cast<real>( 9.0 / 24.0) * d_history[i].dy3_
					);
			d_history[i].dy3_ = d_history[i].dy2_;
			d_history[i].dy2_ = d_history[i].dy1_;
			d_history[i].dy1_ = d_dy[i];
			

		});
	Kokkos::fence();

	return true;	
}