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


#ifndef __setFieldList_hpp__
#define __setFieldList_hpp__

#include "List.hpp"
#include "setFieldEntry.hpp"
#include "dictionary.hpp"

namespace pFlow
{

class setFieldList
:
	public List<setFieldEntry>
{
protected:
	
	// setFields dictionary 
	dictionary dict_;

	bool readSetFieldList(const dictionary& dict);

	bool writeSetFieldList(dictionary& dict)const;

public:

	setFieldList(const dictionary& dict);

	setFieldList(const setFieldList&) = default;

	setFieldList(setFieldList&&) = default;


	setFieldList& operator=(const setFieldList&) = default;

	setFieldList& operator=(setFieldList&&) = default;

	auto clone()const
	{
		return makeUnique<setFieldList>(*this);
	}

	setFieldList* clonePtr()const
	{
		return new setFieldList(*this);
	}

	~setFieldList() = default;
	
	bool read(const dictionary& dict);

	bool write(dictionary& dict)const;

};

}



#endif //__setFieldList_hpp__
