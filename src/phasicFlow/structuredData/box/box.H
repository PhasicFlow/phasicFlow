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

#ifndef __box_H__
#define __box_H__

#include "types.H"
#include "dictionary.H"
#include "iIstream.H"
#include "iOstream.H"

namespace pFlow
{

class box
{
protected:
	
	// - min point
	realx3 	min_;
	
	// - max point 
	realx3 	max_;

public:

	// - type info
	TypeNameNV("box");

	//// - Constructors 
		FUNCTION_HD 
		box(const realx3& minP, const realx3& maxP);

		FUNCTION_H
		box(const dictionary& dict);

		FUNCTION_H
		box(iIstream& is);

		FUNCTION_HD
		box(const box&) = default;

		FUNCTION_HD
		box(box&&) = default;

		FUNCTION_HD
		box& operator=(const box&) = default;

		FUNCTION_HD
		box& operator=(box&&) = default;

		~box()=default;

	//// - Methods 

		INLINE_FUNCTION_HD
		bool isInside(const realx3& point)const
		{
			return point > min_ && point <max_;
		}

		INLINE_FUNCTION_HD
		const realx3& minPoint()const
		{
			return min_;
		}

		INLINE_FUNCTION_HD
		const realx3& maxPoint()const
		{
			return max_;
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
iIstream& operator << (iIstream& is, box& b);

FUNCTION_H
iOstream& operator >> (iOstream& os, const box& b);


}


#endif
