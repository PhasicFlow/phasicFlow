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

#ifndef __plane_hpp__
#define __plane_hpp__

#include "infinitePlane.hpp"
#include "line.hpp"

namespace pFlow
{

class plane
:
	public infinitePlane
{
protected:
	
	/// First point 
	realx3 p1_;
	
	/// Second point 
	realx3 p2_;
	
	/// Third point 
	realx3 p3_;
	
	/// Fourth point 
	realx3 p4_;

public:

	INLINE_FUNCTION_HD
	plane(){}

	plane(
		const realx3& p1, 
		const realx3& p2, 
		const realx3& p3, 
		const realx3& p4);

	INLINE_FUNCTION_HD
	plane(const plane&) = default;

	INLINE_FUNCTION_HD
	plane(plane&&) = default;

	INLINE_FUNCTION_HD
	plane& operator = (const plane&) = default;

	INLINE_FUNCTION_HD
	plane& operator =( plane&&)=default;
	
	INLINE_FUNCTION_HD
	~plane()=default;

	INLINE_FUNCTION_HD
	const realx3& p1()const
	{
		return p1_;
	}

	INLINE_FUNCTION_HD
	const realx3& p2()const
	{
		return p2_;
	}

	INLINE_FUNCTION_HD
	const realx3& p3()const
	{
		return p3_;
	}

	INLINE_FUNCTION_HD
	const realx3& p4()const
	{
		return p4_;
	}

	INLINE_FUNCTION_HD
	line line1()const
	{
		return line(p1_,p2_);
	}

	INLINE_FUNCTION_HD
	line line2()const
	{
		return line(p2_,p3_);
	}

	INLINE_FUNCTION_HD
	line line3()const
	{
		return line(p3_,p4_);
	}

	INLINE_FUNCTION_HD
	line line4()const
	{
		return line(p4_,p1_);
	}

	// return the parallel plane to this plane 
	plane parallelPlane(real distance)const;

	infinitePlane infPlane()const
	{
		return infinitePlane(normal(), d());
	}


	static 
	bool validPlane4(
		const realx3& p1, 
		const realx3& p2, 
		const realx3& p3, 
		const realx3& p4);

};

}

#endif //__plane_hpp__