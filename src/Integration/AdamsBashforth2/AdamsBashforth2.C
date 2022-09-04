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

#include "AdamsBashforth2.H"

//const real AB2_coef[] = { 3.0 / 2.0, 1.0 / 2.0};

pFlow::AdamsBashforth2::AdamsBashforth2
(
	const word& baseName,
	repository& owner,
	const pointStructure& pStruct,
	const word& method
)
:
	integration(baseName, owner, pStruct, method),
	dy1_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy1"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3))
{

}

bool pFlow::AdamsBashforth2::predict
(
	real UNUSED(dt),
	realx3Vector_D& UNUSED(y),
	realx3Vector_D& UNUSED(dy)
)
{

	return true;
}

bool pFlow::AdamsBashforth2::correct
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

bool pFlow::AdamsBashforth2::intAll(
	real dt, 
	realx3Vector_D& y, 
	realx3Vector_D& dy, 
	range activeRng)
{

	auto d_dy = dy.deviceVectorAll();
	auto d_y  = y.deviceVectorAll();
	auto d_dy1= dy1_.deviceVectorAll();

	Kokkos::parallel_for(
		"AdamsBashforth2::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			d_y[i] +=  dt*(static_cast<real>(3.0 / 2.0) * d_dy[i] - static_cast<real>(1.0 / 2.0) * d_dy1[i]);
			d_dy1[i] = d_dy[i];
		});
	Kokkos::fence();

	return true;	
}