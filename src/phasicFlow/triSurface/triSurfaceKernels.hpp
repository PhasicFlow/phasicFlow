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


#ifndef __triSurfaceKernels_hpp__
#define __triSurfaceKernels_hpp__

#include "Fields.hpp"
#include "triangleFunctions.hpp"

namespace pFlow::triSurfaceKernels
{

INLINE_FUNCTION_H
bool calculateArea(
	const realx3Field_D& points, 
	const uint32x3Field_D& vertices, 
	realField_D& area)
{
	auto numTri = vertices.size();
	auto& areaD = area.deviceViewAll();
	auto& pointsD = points.deviceViewAll();
	auto& verticesD = vertices.deviceViewAll();

	Kokkos::parallel_for(
		"pFlow::triSurfaceKernels::calculateArea",
		numTri,
		LAMBDA_HD(uint32 i){
			auto v = verticesD[i];
			areaD[i] = pFlow::triangle::surface(
				pointsD[v.x()],
				pointsD[v.y()],
				pointsD[v.z()]);
		});

	return true;
}

INLINE_FUNCTION_H
bool calculateNormals(
	const realx3Field_D& points, 
	const uint32x3Field_D& vertices, 
	realx3Field_D& normals)
{
	auto numTri = vertices.size();
	auto& normalsD = normals.deviceViewAll();
	auto& pointsD = points.deviceViewAll();
	auto& verticesD = vertices.deviceViewAll();

	Kokkos::parallel_for(
		"pFlow::triSurfaceKernels::calculateNormals",
		numTri,
		LAMBDA_HD(uint32 i){
			auto v = verticesD[i];
			normalsD[i] = pFlow::triangle::normal(
				pointsD[v.x()],
				pointsD[v.y()],
				pointsD[v.z()]);
		});

	return true;
}


}

#endif 
