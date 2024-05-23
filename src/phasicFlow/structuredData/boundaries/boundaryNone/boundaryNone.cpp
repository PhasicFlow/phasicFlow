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

#include "boundaryNone.hpp"

pFlow::boundaryNone::boundaryNone
(
	const dictionary &dict,
    const plane 	&bplane,
    internalPoints 	&internal,
	boundaryList	&bndrs,
	uint32 			thisIndex
)
:
	boundaryBase(dict, bplane, internal, bndrs, thisIndex)
{}

bool pFlow::boundaryNone::beforeIteration
(
	uint32 step,
	uint32 iterNum, 
	real t,
	real dt
)
{
	return true;
}

bool pFlow::boundaryNone::iterate
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	return true;
}

bool pFlow::boundaryNone::afterIteration
(
	uint32 iterNum, 
	real t,
	real dt
)
{
	return true;
}