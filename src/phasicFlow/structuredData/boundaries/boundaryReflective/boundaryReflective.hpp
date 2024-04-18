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

#ifndef __boundaryReflective_hpp__
#define __boundaryReflective_hpp__


#include "boundaryBase.hpp"

namespace pFlow
{

class boundaryReflective
:
 	public boundaryBase
{
private:

	real        restitution_ = 0.95;

    word        velocityName_{"velocity"};
	
    word        diameterName_{"diameter"};

public:

	TypeInfo("boundary<reflective>");

	boundaryReflective(
		const dictionary &dict,
		const plane 	&bplane,
		internalPoints 	&internal,
		boundaryList	&bndrs,
		uint32 			thisIndex);

	 
	~boundaryReflective()override = default;
	
	add_vCtor
	(
		boundaryBase,
		boundaryReflective,
		dictionary
	);

	bool beforeIteration(uint32 iterNum, real t, real dt) override;

	bool iterate(uint32 iterNum, real t, real dt) override;

	bool afterIteration(uint32 iterNum, real t, real dt) override;


};

}

#endif