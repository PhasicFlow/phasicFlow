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

#ifndef __cylinder_hpp__
#define __cylinder_hpp__

#include "types.hpp"
#include "dictionary.hpp"
#include "iIstream.hpp"
#include "iOstream.hpp"

namespace pFlow
{

class cylinder
{
protected:
	
	// - begin point
	realx3 	p1_;
	
	// - end point 
	realx3 	p2_;

	// - radius^2
	real  	radius2_;

	realx3  axisVector_;

	real 		axisVector2_;

	realx3 	minPoint_;

	realx3 	maxPoint_;

	FUNCTION_H
	bool calculateParams();

public:

	// - type info
	TypeInfoNV("cylinder");

	//// - Constructors 
		FUNCTION_H 
		cylinder(const realx3& p1, const realx3& p2, const real radius);

		FUNCTION_H
		cylinder(const dictionary& dict);

		FUNCTION_H
		cylinder(iIstream& is);

		FUNCTION_HD
		cylinder(const cylinder&) = default;

		FUNCTION_HD
		cylinder(cylinder&&) = default;

		FUNCTION_HD
		cylinder& operator=(const cylinder&) = default;

		FUNCTION_HD
		cylinder& operator=(cylinder&&) = default;

		~cylinder()=default;

	//// - Methods 

		INLINE_FUNCTION_HD
		bool isInside(const realx3& point)const
		{
			auto p1Point = point-p1_;
			auto H = cross(p1Point , axisVector_);
			auto H2 = dot(H,H);

			// the shell itslef is considered as inside point 
			if( H2 <= radius2_*axisVector2_)
			{
				real t = dot(p1Point, axisVector_)/axisVector2_;
				if(t >= 0.0 && t <= 1.0)
					return true;
				else
					return false;
			}
			else
			{
				return false;
			}	

		}

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
		realx3 minPoint()const
		{
			return minPoint_;
		}

		INLINE_FUNCTION_HD
		realx3 maxPoint()const
		{
			return maxPoint_;
		}

		INLINE_FUNCTION_HD
		real radius()const
		{
			return sqrt(radius2_);
		}

	//// - IO operation 
		FUNCTION_H
		bool read(iIstream & is);

		FUNCTION_H
		bool write(iOstream& os)const;

		FUNCTION_H
		bool read(const dictionary& dict);

		FUNCTION_H
		bool write(dictionary& dict)const;
};

FUNCTION_H
iIstream& operator >>(iIstream& is, cylinder& b);

FUNCTION_H
iOstream& operator << (iOstream& os, const cylinder& b);


} // pFlow


#endif // __cylinder_hpp__
