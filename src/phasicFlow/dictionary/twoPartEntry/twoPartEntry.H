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
// based on OpenFOAM dictionary, with some modifications/simplifications
// to be tailored to our needs


#ifndef __twoPartEntry_H__
#define __twoPartEntry_H__



#include "dataEntry.H"


namespace pFlow
{


class twoPartEntry
{
protected:
	word 		keyword_;
	word 		firstPart_;
	mutable iTstream	secondPart_; 
public:

	twoPartEntry(dataEntry entry);

	const word& keyword()const
	{
		return keyword_;
	}

	const word& firstPart()const
	{
		return firstPart_;
	}

	template<typename T>
	T	secondPartVal()const
	{
		T val;
		secondPart_.rewind();
		secondPart_ >> val;
		return val;
	}

};
	

}

#endif
