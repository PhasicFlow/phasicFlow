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


#include "setFieldList.hpp"

bool pFlow::setFieldList::readSetFieldList(const dictionary& dict)
{
	this->clear();
	wordList Keys = dict.dataEntryKeywords();

	for(const auto& key:Keys)
	{
		this->emplace_back( dict.dataEntryRef(key));
	}

	return true;
}

bool pFlow::setFieldList::writeSetFieldList
(
	dictionary& dict
)const
{
	dict = dict_;
	return true;
}

pFlow::setFieldList::setFieldList
(
	const dictionary& dict
)
:
	dict_(dict)
{
	if(!readSetFieldList(dict_))
	{
		fatalExit;
	}
}


bool pFlow::setFieldList::read
(
	const dictionary& dict
)
{
	dict_ = dict;
	return readSetFieldList(dict);
}

bool pFlow::setFieldList::write
(
	dictionary& dict
)const
{
	return writeSetFieldList(dict);
}
