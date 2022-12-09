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

#include "cylinderRegion.hpp"


pFlow::cylinderRegion::cylinderRegion
(
	const dictionary& dict
)
:
	cylinder_(dict),
	random_()
{

}

bool pFlow::cylinderRegion::isInside
(
	const realx3& p
) const
{
	return cylinder_.isInside(p);
}

pFlow::realx3 pFlow::cylinderRegion::peek()const
{
	for(int32 i=0; i<100;i++)
	{
		auto p = 
		random_.randomNumber(cylinder_.minPoint(), cylinder_.maxPoint());
		if( cylinder_.isInside(p)) return p;
	}

	fatalErrorInFunction<<
	"cannot peek a random point from cylinderRegion. \n";
	fatalExit;
	return 0;
}


bool pFlow::cylinderRegion::read
(
	const dictionary& dict
)
{
	return cylinder_.read(dict);
}

bool pFlow::cylinderRegion::write
(
	dictionary& dict
)const
{
	return cylinder_.write(dict);
}