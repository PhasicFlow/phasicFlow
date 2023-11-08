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

#ifndef __domain_hpp__
#define __domain_hpp__


#include "box.hpp"
#include "plane.hpp"

namespace pFlow
{

class domain
{
protected:
	
	box 		domainBox_;

	/// -x
	plane 		left_;

	/// +x
	plane 		right_;

	/// -y
	plane 		bottom_;

	/// +y
	plane		top_;

	/// -z
	plane 		rear_;

	/// +z
	plane 		front_;


public:

	// - type info
	TypeInfoNV("domain");

	//// - Constructors 
		INLINE_FUNCTION_HD 
		domain(){}

		FUNCTION_H 
		domain(const box& db);
		
		INLINE_FUNCTION_HD
		domain(const domain&) = default;

		INLINE_FUNCTION_HD
		domain(domain&&) = default;

		INLINE_FUNCTION_HD
		domain& operator=(const domain&) = default;

		INLINE_FUNCTION_HD
		domain& operator=(domain&&) = default;

		INLINE_FUNCTION_HD
		~domain()=default;

	//// - Methods 

		INLINE_FUNCTION_HD
		const auto& domainBox()const
		{
			return domainBox_;
		}

		INLINE_FUNCTION_HD
		const auto& left()const
		{
			return left_;
		}

		INLINE_FUNCTION_HD
		const auto& right()const
		{
			return right_;
		}

		INLINE_FUNCTION_HD
		const auto& bottom()const
		{
			return bottom_;
		}

		INLINE_FUNCTION_HD
		const auto& top()const
		{
			return top_;
		}

		INLINE_FUNCTION_HD
		const auto& rear()const
		{
			return rear_;
		}

		INLINE_FUNCTION_HD
		const auto& front()const
		{
			return front_;
		}

	
}; // domain

INLINE_FUNCTION_HD
bool equal(const domain& d1, const domain& d2)
{
	return equal(d1.domainBox(), d2.domainBox());
}

INLINE_FUNCTION_HD
bool equal(const domain& d1, const domain& d2, real tol)
{
	return equal(d1.domainBox(), d2.domainBox(), tol);
}

INLINE_FUNCTION_HD
bool operator ==(const domain& d1, const domain& d2)
{
	return equal(d1, d2);
}

/*FUNCTION_H
iIstream& operator >>(iIstream& is, box& b);

FUNCTION_H
iOstream& operator << (iOstream& os, const box& b);

INLINE_FUNCTION_HD
box extendBox(const box& b, const realx3& dl)
{
	return box(b.minPoint()-dl , b.maxPoint()+dl);
}*/

}


#endif
