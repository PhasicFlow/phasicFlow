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

#ifndef __triSurfaceFields_H__ 
#define __triSurfaceFields_H__

#include "VectorSingle.H"
#include "VectorDual.H"
#include "triSurfaceField.H"
#include "types.H"

namespace pFlow
{

using realTriSurfaceField_D 	= triSurfaceField<VectorSingle, real> ;

using realTriSurfaceField_H 	= triSurfaceField<VectorSingle, real, HostSpace> ;

using realx3TriSurfaceField_D 	= triSurfaceField<VectorSingle, realx3> ;

using realx3TriSurfaceField_H 	= triSurfaceField<VectorSingle, realx3, HostSpace> ;

using realTriSurfaceField_HD 	= triSurfaceField<VectorDual, real> ;

using realx3TriSurfaceField_HD 	= triSurfaceField<VectorDual, realx3> ;

using realTriSurfaceField 		= triSurfaceField<Vector, real, vecAllocator<real>> ;

using realx3TriSurfaceField 	= triSurfaceField<Vector, realx3, vecAllocator<realx3>> ;

using int8TriSurfaceField_D 	= triSurfaceField<VectorSingle, int8> ;

using int8TriSurfaceField_H 	= triSurfaceField<VectorSingle, int8, HostSpace> ;

using int8TriSurfaceField_HD 	= triSurfaceField<VectorDual, int8> ;

using int8TriSurfaceField 		= triSurfaceField<Vector, int8, vecAllocator<real>> ;


}

#endif //__trieSurfaceField_H__
