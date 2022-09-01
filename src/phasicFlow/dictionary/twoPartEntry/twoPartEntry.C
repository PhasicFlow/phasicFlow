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


#include "twoPartEntry.H"


pFlow::twoPartEntry::twoPartEntry
(
	dataEntry entry
)
:
	keyword_(entry.keyword()),
	secondPart_(keyword_)
{
	iTstream& iT = entry.stream();

	iT >> firstPart_;

	token t;

	while(true)
	{
		iT.read(t);
		secondPart_.appendToken(t);
		if(iT.eof())break;
	}

}