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


#ifndef __triSurfaceKernels_H__
#define __triSurfaceKernels_H__

#include "Fields.H"
#include "triangleFunctions.H"

namespace pFlow::triSurfaceKernels
{

INLINE_FUNCTION_H
bool calculateArea(const realx3Field_D& points, const int32x3Field_D& vertices, realField_D& area)
{
	auto numTri = vertices.size();
	auto areaD = area.deviceVectorAll();
	auto pointsD = points.deviceVectorAll();
	auto verticesD = vertices.deviceVectorAll();

	Kokkos::parallel_for(
		"pFlow::triSurfaceKernels::calculateArea",
		numTri,
		LAMBDA_HD(int32 i){
			auto v = verticesD[i];
			areaD[i] = pFlow::triangleFunctions::triangleSurface(
				pointsD[v.x()],
				pointsD[v.y()],
				pointsD[v.z()]);
		});

	return true;
}


}

#endif 
