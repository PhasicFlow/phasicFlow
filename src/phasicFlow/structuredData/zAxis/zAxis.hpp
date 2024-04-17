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


/*
class name: zAxis
a class for transfering points to a cylinderical coordinates defined by points
p1 and p2
*/

#ifndef __zAxis_hpp__
#define __zAxis_hpp__

#include "types.hpp"
#include "array2D.hpp"

namespace pFlow
{


class zAxis
{

private:
	
	using ArrayType = array2D<real,3uL,3uL>;

	/// the origin of the transformed coord
	realx3 			p1_;

	/// the direction vector of rotated coordinates 
	realx3 			p2_;

	/// transformation matrix to rotate original coordinates
	/// to rotated coordinates 
	ArrayType 		rotMat_;

	/// rotation matrix to rotate back from rotated coordinates
	/// to the original axis
	ArrayType 		invRotMat_;

	void makeTransMatrix(const realx3& v);

public:
	// constructors 
	zAxis(const realx3 &lp1, const realx3 &lp2);
	
	inline
	real length()const
	{
		return pFlow::length(p2_-p1_);
	}

	realx3 transferToZ(const realx3 & p)const;

	realx3 transferBackZ(const realx3 & p)const;


};


}

#endif

