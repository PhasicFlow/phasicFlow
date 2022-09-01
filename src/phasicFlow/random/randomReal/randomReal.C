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


#include "randomReal.H"


pFlow::uniquePtr<pFlow::randomReal> pFlow::randomReal::create
(
	word distribution
)
{
	word dist = angleBracketsNames("randomReal", distribution);

	if( wordvCtorSelector_.search(dist) )
	{
		return wordvCtorSelector_[dist] (distribution);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< dist << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit();
	}

	return nullptr;
}


