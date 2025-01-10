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
#ifndef __countFields_hpp__
#define __countFields_hpp__


#include "dictionary.hpp"


namespace pFlow
{


class repository;

class countFields
{
protected:

	dictionary 	dict_;
	
	wordList 	variableNames_;

	uint32List 	countedValues_;

public:

	TypeInfo("countFields");

	countFields(const dictionary& dict);

	auto& dict()
	{
		return dict_;
	}

	const auto& dict()const
	{
		return dict_;
	}

	const wordList& variableNames()const
	{
		return variableNames_;
	}
	const uint32List& countedValues()const
	{
		return countedValues_;
	}
	
	// requires a class to read pointField from timefolder 
	bool process(repository& rep);

};


}


#endif //__countFields_hpp__