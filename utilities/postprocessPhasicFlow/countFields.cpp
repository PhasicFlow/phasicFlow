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

#include "countFields.hpp"
#include "countField.hpp"
#include "repository.hpp"
#include "includeMask.hpp"


pFlow::countFields::countFields(const dictionary& dict)
:
	dict_(dict),
	variableNames_(dict.dictionaryKeywords()),
	countedValues_(variableNames_.size(), 0u)
{

}


bool pFlow::countFields::process(repository& rep)
{
	uint32List counted;

	for(const auto& name: variableNames_)
	{
		const dictionary& varDict = dict_.subDict(name);
		uint32 count;
		countField cField(varDict, rep);
		cField.process(count);
		counted.push_back(count);
	}

	countedValues_ = counted;

	return true;
}
