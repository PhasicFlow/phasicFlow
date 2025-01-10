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

#include "sphereRegion.hpp"
#include "dictionary.hpp"

pFlow::sphereRegion::sphereRegion
(
	const dictionary& dict
)
:
	sphere_(dict),
	random_()
{

}

bool pFlow::sphereRegion::isInside
(
	const realx3& p
) const
{
	return sphere_.isInside(p);
}

pFlow::realx3 pFlow::sphereRegion::peek()const
{
	for(int32 i=0; i<100; ++i)
	{
		auto p = random_.randomNumber(
				sphere_.center() - realx3(sphere_.radius()),
				sphere_.center() + realx3(sphere_.radius()));
		if(sphere_.isInside(p)) return p;
	}
		
	fatalErrorInFunction<<
	"cannot peek a random point from sphereRegion. \n";
	fatalExit;
	return 0;

}


bool pFlow::sphereRegion::read
(
	const dictionary& dict
)
{
	sphere_.read(dict);
	
	return true;
}

bool pFlow::sphereRegion::write
(
	dictionary& dict
)const
{
	if(!sphere_.write(dict))
	{
		fatalErrorInFunction<<
		"  error in writing sphere data to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	return true;
}