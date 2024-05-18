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

#ifndef __boundaryNone_hpp__
#define __boundaryNone_hpp__


#include "boundaryBase.hpp"

namespace pFlow
{

class boundaryNone
:
 	public boundaryBase
{

public:

	TypeInfo("boundary<none>");

	boundaryNone(
		const dictionary &dict,
		const plane 	&bplane,
		internalPoints 	&internal,
		boundaryList	&bndrs,
		uint32 			thisIndex);

	~boundaryNone() final= default;
	
	add_vCtor
	(
		boundaryBase,
		boundaryNone,
		dictionary
	);

	bool beforeIteration(uint32 iterNum, real t, real dt) final;

	bool iterate(uint32 iterNum, real t, real dt) final;

	bool afterIteration(uint32 iterNum, real t, real dt) final;


};

}

#endif