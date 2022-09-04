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

#ifndef __pLine_H__
#define __pLine_H__

#include "types.H"

namespace pFlow::sphTriInteraction
{

struct pLine
{

	realx3 p1_; // point 1
	realx3 p2_; // piont 2
	realx3 v_;  // direction vector 
	real  L_;  // line lenght

	INLINE_FUNCTION_HD
	pLine(){}

	INLINE_FUNCTION_HD 
	pLine(const realx3 &p1, const realx3 &p2)
	:
		p1_(p1),
		p2_(p2),
		v_(p2-p1),
		L_(length(v_))
	{}
	 	
	// get a point on the line based on input  0<= t <= 1
	INLINE_FUNCTION_HD 
	realx3 point(real t)const {
		return v_ * t + p1_;
		}

	// return the projected point of point p on line
	INLINE_FUNCTION_HD
	realx3 projectPoint(const realx3 &p) const
	{
		return point(projectNormLength(p));
	}

	// calculates the normalized distance between projected p and p1 
	INLINE_FUNCTION_HD
	real projectNormLength(realx3 p) const 
	{
		realx3 w = p - p1_;
		return dot(w,v_) / (L_*L_);
	}

	INLINE_FUNCTION_HD
	bool lineSphereCheck(
		const realx3 pos,
		real Rad,
		realx3 &nv,
		realx3 &cp,
		real &ovrlp)const
	{

		
		real t = projectNormLength(pos);

		if(t >= 0.0 && t <= 1.0) 			 	 cp = point(t);
		else if(t >= (-Rad / L_) && t < 0.0) 	 cp = point(0.0);
		else if(t>1.0 && t >= (1.0 + Rad / L_)) cp = point(1.0);
		else return false;
		
		realx3 vec = pos - cp; // from cp to pos

		real dist = length(vec);
		ovrlp = Rad - dist;

		if (ovrlp >= 0.0)
		{
			if (dist > 0)
				nv = vec / dist;
			else
				nv = v_;
			return true;
		}

		return false;
	}
};

} //pFlow::sphTriInteractio

#endif 
