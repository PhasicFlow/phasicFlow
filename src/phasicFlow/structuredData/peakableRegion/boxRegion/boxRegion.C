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

#include "boxRegion.H"


pFlow::boxRegion::boxRegion
(
	const dictionary& dict
)
:
	box_(dict),
	random_()
{

}

bool pFlow::boxRegion::isInside
(
	const realx3& p
) const
{
	return box_.isInside(p);
}

pFlow::realx3 pFlow::boxRegion::peek()const
{
	return random_.randomNumber(box_.minPoint(), box_.maxPoint());
}


bool pFlow::boxRegion::read
(
	const dictionary& dict
)
{
	return box_.read(dict);
}

bool pFlow::boxRegion::write
(
	dictionary& dict
)const
{
	return box_.write(dict);
}