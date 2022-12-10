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

#ifndef __randomReal_hpp__
#define __randomReal_hpp__

#include "types.hpp"
#include "virtualConstructor.hpp"

namespace pFlow
{


class randomReal
{

protected:

public:

	TypeInfo("randomReal");

	randomReal(word UNUSED(distribution)){}

	create_vCtor
	(
		randomReal,
		word,
		(word distribution),
		(distribution)
	);

	virtual ~randomReal()= default;

	virtual real  randomNumber(real a, real b) = 0;

	virtual realx3 randomNumber(realx3 a, realx3 b) = 0;

	
	
	
	static
	uniquePtr<randomReal> create(word distribution);
};



}

#endif
