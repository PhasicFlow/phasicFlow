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

#include "boundaryExit.hpp"
#include "dictionary.hpp"

pFlow::boundaryExit::boundaryExit
(
	const dictionary& dict,
	const plane&      bplane,
	internalPoints&   internal
)
:
	boundaryBase(dict, bplane, internal)
{
	exitMarginLength_ = max( 
		dict.getValOrSet("exitMarginLength",0.0), 0.0);
	checkForExitInterval_ = max(
		dict.getValOrSet("checkForExitInterval", 1), 1);
}

bool pFlow::boundaryExit::beforeIteratoin
(
	uint32 iterNum, 
	real t
)
{
	return true;
}

bool pFlow::boundaryExit::iterate
(
	uint32 iterNum, 
	real t
)
{
	return true;
}

bool pFlow::boundaryExit::afterIteration
(
	uint32 iterNum, 
	real t
)
{
	return true;
}