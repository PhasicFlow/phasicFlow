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

#include "peakableRegion.hpp"
#include "dictionary.hpp"

pFlow::peakableRegion::peakableRegion
(
	const word& type, 
	const dictionary& dict
)
{
	CONSUME_PARAM(type);
	CONSUME_PARAM(dict);
}


pFlow::uniquePtr<pFlow::peakableRegion> pFlow::peakableRegion::create
(
	const word& type,
	const dictionary& dict
)
{ 
	word regionType = angleBracketsNames("peakable", type);

	if( wordvCtorSelector_.search(regionType) )
	{
		return wordvCtorSelector_[regionType] (type, dict);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< regionType << " does not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}