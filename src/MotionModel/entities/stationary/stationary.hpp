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

#ifndef __stationary_hpp__
#define __stationary_hpp__


#include "types.hpp"
#include "typeInfo.hpp"
#include "streams.hpp"

namespace pFlow
{

// forward
class dictionary;


/**
 * stationary model for a wall
 * 
 */
class stationary
{
public:

	TypeInfoNV("stationary");

	FUNCTION_HD
	stationary()=default;

	FUNCTION_H
	explicit stationary(const dictionary& dict);
	

	FUNCTION_HD
	stationary(const stationary&) = default;

	stationary& operator=(const stationary&) = default;

	INLINE_FUNCTION_HD
	void setTime(real t)
	{}

	INLINE_FUNCTION_HD
	realx3 linVelocityPoint(const realx3 &, const realx3&)const
	{
		return realx3(0);
	}

	INLINE_FUNCTION_HD
	realx3 transferPoint(const realx3& p, real)const
	{
		return p;
	}

	// - IO operation
	FUNCTION_H 
	bool read(const dictionary& dict);

	FUNCTION_H
	bool write(dictionary& dict) const;

	FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;

};

inline iOstream& operator <<(iOstream& os, const stationary& obj)
{
	
	return os;
}

inline iIstream& operator >>(iIstream& is, stationary& obj)
{
	
	return is;
}

}


#endif
