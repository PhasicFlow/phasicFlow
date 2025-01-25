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

#ifndef __triangleFunctions_hpp__
#define __triangleFunctions_hpp__

#include "types.hpp"

namespace pFlow::triangle
{

INLINE_FUNCTION_HD
real surface( const realx3& p1, const realx3& p2, const realx3& p3)
{
	realx3 V1 = p2 - p1;
	realx3 V2 = p3 - p1;
	return abs((cross(V1,V2)).length()/(real)2.0);	
}

INLINE_FUNCTION_HD
realx3 normal(const realx3& p1, const realx3& p2, const realx3& p3)
{
	auto n = cross(p2-p1, p3-p1);
	if( equal(n.length(), zero) )
    	return zero3;
	else
		return normalize(n);
}

INLINE_FUNCTION_HD
bool valid
(
	const realx3& p1, 
	const realx3& p2, 
	const realx3& p3
)
{
  	return !equal(cross(p2-p1, p3-p1).length(), zero);
}

}

#endif 
