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

#ifndef __triSurfaceFields_hpp__ 
#define __triSurfaceFields_hpp__

#include "types.hpp"
#include "VectorSingle.hpp"
#include "triSurfaceField.hpp"


namespace pFlow
{

using uint32TriSurfaceField_D 	= triSurfaceField<uint32> ;

using uint32TriSurfaceField_H 	= triSurfaceField<uint32, HostSpace> ;

using realTriSurfaceField_D 	= triSurfaceField<real> ;

using realTriSurfaceField_H 	= triSurfaceField<real, HostSpace> ;

using realx3TriSurfaceField_D 	= triSurfaceField<realx3> ;

using realx3TriSurfaceField_H 	= triSurfaceField<realx3, HostSpace> ;



}

#endif //__trieSurfaceField_hpp__
