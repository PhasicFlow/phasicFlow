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

#ifndef __RandomReal_hpp__
#define __RandomReal_hpp__

//#include <random>

#include "randomReal.hpp"
#include "uniformRandomReal.hpp"


namespace pFlow
{

template<typename DistributionType>
class RandomReal
:
	public randomReal 
{
protected:

 	DistributionType distribution_;

public:

	// type info
	TypeInfoTemplate("randomReal", DistributionType);


	RandomReal(word distribution);

	add_vCtor
	(
		randomReal,
		RandomReal,
		word
	);

	virtual ~RandomReal()= default;

	virtual real  randomNumber(real a, real b)override;

	virtual realx3 randomNumber(realx3 a, realx3 b)override;
	

};


}

#include "RandomReal.cpp"



#endif
