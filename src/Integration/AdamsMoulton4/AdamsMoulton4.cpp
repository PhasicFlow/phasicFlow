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

#include "AdamsMoulton4.hpp"

//const real AB2_coef[] = { 3.0 / 2.0, 1.0 / 2.0};

pFlow::AdamsMoulton4::AdamsMoulton4
(
	const word& baseName,
	repository& owner,
	const pointStructure& pStruct,
	const word& method
)
:
	integration(baseName, owner, pStruct, method),
	y0_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"y0"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3,
			false
			)
		),
	dy0_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy0"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3
			)
		),
	dy1_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy1"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3
			)
		),
	dy2_(
		owner.emplaceObject<realx3PointField_D>(
			objectFile(
				groupNames(baseName,"dy2"),
				"",
				objectFile::READ_IF_PRESENT,
				objectFile::WRITE_ALWAYS),
			pStruct,
			zero3
			)
		)
{

}

bool pFlow::AdamsMoulton4::predict
(
	real dt,
	realx3Vector_D& y,
	realx3Vector_D& dy
)
{

	if(this->pStruct().allActive())
	{
		return predictAll(dt, y, dy, this->pStruct().activeRange());
	}
	else
	{
		return predictRange(dt, y, dy, this->pStruct().activePointsMaskD());
	}

	return true;
}

bool pFlow::AdamsMoulton4::correct
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

bool pFlow::AdamsMoulton4::setInitialVals(
	const int32IndexContainer& newIndices,
	const realx3Vector& y)
{
	y0_.insertSetElement(newIndices, y);
	
	return true;
}

bool pFlow::AdamsMoulton4::predictAll(
	real dt, 
	realx3Vector_D& y, 
	realx3Vector_D& dy, 
	range activeRng)
{

	auto d_dy  = dy.deviceViewAll();
	auto d_y   = y.deviceViewAll();

	auto d_y0  = y0_.deviceViewAll();
	auto d_dy0 = dy0_.deviceViewAll();
	auto d_dy1 = dy1_.deviceViewAll();
	auto d_dy2 = dy2_.deviceViewAll();

	Kokkos::parallel_for(
		"AdamsMoulton4::predict",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			d_dy0[i] = d_dy[i];
			d_y[i] = d_y0[i] + dt*(
				  static_cast<real>(23.0 /12.0 ) * d_dy[i] 
				- static_cast<real>(16.0 / 12.0) * d_dy1[i]
				+ static_cast<real>( 5.0 / 12.0) * d_dy2[i]);
		});
	Kokkos::fence();

	return true;	
}

bool pFlow::AdamsMoulton4::intAll(
	real dt, 
	realx3Vector_D& y, 
	realx3Vector_D& dy, 
	range activeRng)
{

	auto d_dy = dy.deviceViewAll();
	auto d_y  = y.deviceViewAll();

	auto d_dy0 = dy0_.deviceViewAll();
	auto d_y0  = y0_.deviceViewAll();
	auto d_dy1 = dy1_.deviceViewAll();
	auto d_dy2 = dy2_.deviceViewAll();

	Kokkos::parallel_for(
		"AdamsMoulton4::correct",
		rpIntegration (activeRng.first, activeRng.second),
		LAMBDA_HD(int32 i){
			auto corrct_y = d_y0[i] + dt*(
				  static_cast<real>(9.0/24.0)*d_dy[i] 
				+ static_cast<real>(19.0/24.0)*d_dy0[i] 
				- static_cast<real>( 5.0/24.0)*d_dy1[i]
				+ static_cast<real>( 1.0/24.0)*d_dy2[i]);
			
			d_dy2[i]= d_dy1[i];
			d_dy1[i]= d_dy0[i];
			d_y0[i] = corrct_y;
			d_y[i]  = corrct_y;
		});
	Kokkos::fence();
	
	return true;	
}