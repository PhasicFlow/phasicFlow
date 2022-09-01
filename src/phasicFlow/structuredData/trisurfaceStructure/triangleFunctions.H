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

#ifndef __triangleFunctions_H__
#define __triangleFunctions_H__

#include "types.H"

namespace pFlow::triangleFunctions
{

INLINE_FUNCTION_HD
real triangleSurface( const realx3& p1, const realx3& p2, const realx3& p3)
{
	realx3 V1 = p2 - p1;
	realx3 V2 = p3 - p1;
	return abs((cross(V1,V2)).length() / static_cast<real>(2.0));	
}

}

#endif 
