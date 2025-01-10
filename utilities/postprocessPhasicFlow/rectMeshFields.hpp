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

#ifndef __rectMeshFields_hpp__
#define __rectMeshFields_hpp__

#include "rectMeshField.hpp"

namespace pFlow
{

template<typename T>
using rectMeshField_H 		= rectMeshField<T>;

using int8RectMeshField_H  = rectMeshField<int8>;

using int32RectMeshField_H 	= rectMeshField<int32>;

using int64RectMeshField_H  = rectMeshField<int64>;

using realRectMeshField_H	= rectMeshField<real>;

using realx3RectMeshField_H	= rectMeshField<realx3>;

}


#endif // __rectMeshFields_hpp__
