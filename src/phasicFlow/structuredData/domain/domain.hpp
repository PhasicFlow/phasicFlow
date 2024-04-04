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
private:
	
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
		domain() = default;

		FUNCTION_H 
		explicit domain(const box& db);
		
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

		INLINE_FUNCTION_H
		const auto& boundaryPlane(uint32 i)const 
		{
			switch (i)
			{
				case 0:
					return left_;
				case 1:
					return right_;
				case 2:
					return bottom_;
				case 3:
					return top_;
				case 4:
					return rear_;
				case 5:
					return front_;
				default:
					fatalErrorInFunction;
					return front_;
			}
		}

		INLINE_FUNCTION_HD
		const auto& minPoint()const
		{
			return domainBox_.minPoint();
		}

		INLINE_FUNCTION_HD
		const auto& maxPoint()const
		{
			return domainBox_.maxPoint();
		}
		
}; // domain


INLINE_FUNCTION_HD
bool equal(const domain& d1, const domain& d2, real tol=smallValue)
{
	return equal(d1.domainBox(), d2.domainBox(), tol);
}

INLINE_FUNCTION_HD
bool operator ==(const domain& d1, const domain& d2)
{
	return equal(d1, d2);
}

FUNCTION_H
iIstream& operator >>(iIstream& is, domain& d);

FUNCTION_H
iOstream& operator << (iOstream& os, const domain& d);


}


#endif
