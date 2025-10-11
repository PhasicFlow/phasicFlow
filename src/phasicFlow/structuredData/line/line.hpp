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


#ifndef __line_hpp__
#define __line_hpp__

#include "types.hpp"
#include "typeInfo.hpp"

namespace pFlow
{


class dictionary;


// pLine 
class line
{
protected:
	
	// point 1
	realx3 p1_;

	// vector pointing from 1 to 2
	realx3 v21_; 

public:

	TypeInfoNV("line");

	//// - Constructors 
		FUNCTION_HD
		line()
		: v21_(1, 0, 0), p1_(0, 0, 0)
		{}

		FUNCTION_HD
		line(const realx3 &lp1, const realx3 &lp2);

		INLINE_FUNCTION_HD
		line(const realx3 &v21, const realx3 &p1, bool)
		:
			v21_(v21),
			p1_(p1)
		{}


		FUNCTION_H
		line(const dictionary& dict);

		INLINE_FUNCTION_HD
		line(const line& src) = default;

		INLINE_FUNCTION_HD
		line(line&& src) = default;

		INLINE_FUNCTION_HD
		line& operator = (const line&) = default;

		INLINE_FUNCTION_HD
		line& operator = (line&&) = default;

	
	//// - Methods 
		
		// set two points of line 
		INLINE_FUNCTION_HD
		void set(const realx3 &lp1, const realx3 &lp2)
		{
			v21_ = lp2 - lp1;
			if(v21_.length() < verySmallValue)
			{
				v21_ = realx3(1, 0, 0);
			}
			p1_ = lp1;
		}

		// return point 1
		INLINE_FUNCTION_HD
		realx3 point1()const { return p1_; }

		// return point 2
		INLINE_FUNCTION_HD
		realx3 point2()const { return point(1.0); }

		// get a point on the line based on input  0<= t <= 1
		INLINE_FUNCTION_HD
		realx3 point(real t)const  { return v21_ * t + p1_; }

		// length of line 
		INLINE_FUNCTION_HD
		real length()const { return pFlow::length(v21_); }

		// get unit vector of the line direction vector  
		INLINE_FUNCTION_HD
		realx3 unitVector() const{ return normalize(v21_); }

		// return the projected point of point p on line
		INLINE_FUNCTION_HD
		realx3 projectPoint(const realx3 &p) const
		{
			return point(projectNormalizedLength(p));
		}

		// calculates the normalized distance between projected p and p1 
		INLINE_FUNCTION_HD
		real projectNormalizedLength(realx3 p) const 
		{
			realx3 w = p - p1_;
			return dot(w, v21_) / dot(v21_, v21_);
		}

	//// - IO operation 
		FUNCTION_H
		bool read(const dictionary& dict);

		FUNCTION_H
		bool write(dictionary& ditc) const;
		
		FUNCTION_H
		bool read(iIstream& is);

		FUNCTION_H
		bool write(iOstream& os)const;


};


}


#endif // 