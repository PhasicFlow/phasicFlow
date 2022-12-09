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

#ifndef __Logical_hpp__
#define __Logical_hpp__

#include "builtinTypes.hpp"
#include "bTypesFunctions.hpp"
#include "typeInfo.hpp"

namespace pFlow
{

class iIstream;
class iOstream;

// allias for bool
class Logical
{
protected:
	bool s_ = false;

	int  yesNoSet_ = 0;

	inline static const word YesNo__[4][2] = {{"Yes", "No"},{"on","off"},{"true","false"}, {"Ok","No"}};

	inline explicit Logical(bool s, int yns)
	:	
		s_(s),
		yesNoSet_(yns)
	{}

public:

	TypeInfoNV("Logical");
	

	inline Logical(){}
		

	inline explicit Logical(bool s)
	:
	 	s_(s),
	 	yesNoSet_(0)
	{}

	Logical(const word& l);
	
	Logical(const char* ch);

	Logical(const Logical&) = default;

	Logical(Logical&&) = default;

	Logical& operator=(const Logical&) = default;

	Logical& operator=(Logical&&) = default;

	inline Logical& operator=(const bool& b)
	{ 
		s_ = b;
		yesNoSet_ = 0;
		return *this;
	}
	
	inline bool operator()() const
	{
		return s_;
	}
	
	inline explicit operator bool() const
	{
		return s_;
	}

	inline Logical operator!()const
	{
		return Logical(!s_, yesNoSet_);
	}

	//// IO operations
	bool read(iIstream& is);

	bool write(iOstream& os)const; 

	bool static evaluteWord(const word& l, bool& b, int& yesNoSet );
	

};

iIstream& operator>>( iIstream& is, Logical& L);

iOstream& operator<<( iOstream& os, const Logical& L);

} // pFlow

#endif // __Logical_hpp__
