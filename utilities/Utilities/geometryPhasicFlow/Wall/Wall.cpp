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


#include "Wall.hpp"

bool pFlow::Wall::readCommon(const dictionary& dict)
{
	materialName_ = dict.getVal<word>("material");

	motionName_ = dict.getValOrSet("motion", word("none"));

	name_ = dict.name();

	return true;
}


pFlow::Wall::Wall(const dictionary& dict)
{
	if(!readCommon(dict))
	{
		fatalExit;
	}
}


bool pFlow::Wall::checkTrianlge
(
	const realx3 &p1,
	const realx3 &p2,
	const realx3 &p3
)
{
	realx3 ln = cross(p2 - p1, p3 - p1);
	
	if (ln.length() < smallValue) return false;
	
	return true;
}

pFlow::uniquePtr<pFlow::Wall> 
pFlow::Wall::create
(
	const dictionary& dict
)
{
	word type = dict.getVal<word>("type");

	if( dictionaryvCtorSelector_.search(type) )
	{
		return dictionaryvCtorSelector_[type] (dict);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< type << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}

namespace pFlow
{

bool checkNormalVec(
	const realx3 &p1,
	const realx3 &p2,
	const realx3 &p3,
	realx3& norm )
{
	realx3 ln = cross(p2 - p1, p3 - p1);
	real len = length(ln);
	if (len < smallValue) return false;
	
	norm = ln/len;

	return true;
}

}