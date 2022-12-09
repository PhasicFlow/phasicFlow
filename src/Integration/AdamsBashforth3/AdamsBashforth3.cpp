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

#include "AdamsBashforth3.hpp"

//const real AB3_coef[] = { 23.0 / 12.0, 16.0 / 12.0, 5.0 / 12.0 };

pFlow::AdamsBashforth3::AdamsBashforth3
(
	const word& baseName,
	repository& owner,
	const pointStructure& pStruct,
	const word& method
)
:
	integration(baseName, owner, pStruct, method),
	/*dy1_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy1"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3)),
	dy2_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy2"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3))*/
	history_(
		owner.emplaceObject<HistoryFieldType>(
			objectFile(
				groupNames(baseName,"AB3History"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			AB3History({zero3,zero3})))

{
	
}

bool pFlow::AdamsBashforth3::predict
(
	real UNUSED(dt),
	realx3Vector_D& UNUSED(y),
	realx3Vector_D& UNUSED(dy)
)
{

	return true;
}

bool pFlow::AdamsBashforth3::correct
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

bool pFlow::AdamsBashforth3::setInitialVals(
	const int32IndexContainer& newIndices,
	const realx3Vector& y)
{
	return true;
}

bool pFlow::AdamsBashforth3::intAll(
	real dt, 
	realx3Vector_D& y, 
	realx3Vector_D& dy, 
	range activeRng)
{
	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_history = history_.deviceVectorAll();

	Kokkos::parallel_for(
		"AdamsBashforth3::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			auto ldy = d_dy[i];
			d_y[i] += dt*( static_cast<real>(23.0 / 12.0) * ldy 
				- static_cast<real>(16.0 / 12.0) * d_history[i].dy1_ 
				+ static_cast<real>(5.0 / 12.0) * d_history[i].dy2_);
			d_history[i] = {ldy ,d_history[i].dy1_};
		});
	Kokkos::fence();

	return true;	
}