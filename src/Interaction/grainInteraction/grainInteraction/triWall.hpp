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

#ifndef __triWall_hpp__
#define __triWall_hpp__

#include "types.hpp"

namespace pFlow::grnTriInteraction
{

struct triWall
{
	
	realx3 n_;
	real   offset_;

	INLINE_FUNCTION_H
	triWall(const realx3& p1, const realx3& p2, const realx3& p3)
	{
		if(!makeWall(p1,p2,p3, n_, offset_))
		{
			fatalErrorInFunction<<
			"bad input for the wall.\n";
			fatalExit;
		}
	}

	INLINE_FUNCTION_HD
	triWall(bool, const realx3& p1, const realx3& p2, const realx3& p3)
	{
		makeWall(p1,p2,p3,n_,offset_);
	}

	INLINE_FUNCTION_HD
	triWall(const realx3x3& tri)
	{
		makeWall(tri.x_, tri.y_, tri.z_, n_, offset_);
	}

	INLINE_FUNCTION_HD
	triWall(const triWall&) = default;

	INLINE_FUNCTION_HD
	triWall& operator=(const triWall&) = default;

	INLINE_FUNCTION_HD
	triWall(triWall&&) = default;

	INLINE_FUNCTION_HD
	triWall& operator=(triWall&&) = default;

	INLINE_FUNCTION_HD
	~triWall()=default;

	
	INLINE_FUNCTION_HD 
	real normalDistFromWall(const realx3 &p) const
	{
		return dot(n_, p) + offset_;
	}

	INLINE_FUNCTION_HD
	realx3 nearestPointOnWall(const realx3 &p) const
	{
		real t = -(dot(n_, p) + offset_);
		return realx3(n_.x_*t + p.x_, n_.y_*t + p.y_, n_.z_*t + p.z_);
	}

	INLINE_FUNCTION_HD static 
	bool makeWall(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		realx3& n, real& offset)
	{
		n = cross(p2 - p1, p3 - p1);
		real len = length(n);
	
		if (len < 0.00000000001) return false;
		n /= len;
		offset = -dot(n, p1);
		return true;
	}


}; 

}

#endif
