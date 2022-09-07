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

#ifndef __sphere_H__
#define __sphere_H__

#include "types.H"
#include "dictionary.H"
#include "iIstream.H"
#include "iOstream.H"

namespace pFlow
{

class sphere
{
protected:
	
	// - center
	realx3 	center_;
	
	// - radius^2
	real  	radius2_;


public:

	// - type info
	TypeNameNV("sphere");

	//// - Constructors 
		FUNCTION_H 
		sphere(const realx3& center, const real radius);

		FUNCTION_H
		sphere(const dictionary& dict);

		FUNCTION_H
		sphere(iIstream& is);

		FUNCTION_HD
		sphere(const sphere&) = default;

		FUNCTION_HD
		sphere(sphere&&) = default;

		FUNCTION_HD
		sphere& operator=(const sphere&) = default;

		FUNCTION_HD
		sphere& operator=(sphere&&) = default;

		~sphere()=default;

	//// - Methods 

		INLINE_FUNCTION_HD
		bool isInside(const realx3& point)const
		{
			auto cPoint = point-center_;
			auto dist2 = dot(cPoint,cPoint);
			return dist2 < radius2_;

		}

		INLINE_FUNCTION_HD
		const realx3& center()const
		{
			return center_;
		}

		
		INLINE_FUNCTION_HD
		realx3 minPoint()const
		{
			return center_ - realx3(radius());
		}

		INLINE_FUNCTION_HD
		realx3 maxPoint()const
		{
			return center_ + realx3(radius());
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
iIstream& operator >>(iIstream& is, sphere& b);

FUNCTION_H
iOstream& operator << (iOstream& os, const sphere& b);


} // pFlow


#endif // __sphere_H__
