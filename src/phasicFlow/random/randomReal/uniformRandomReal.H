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

#ifndef __uniformRandomReal_H__
#define __uniformRandomReal_H__

#include <random>

#include "types.H"
#include "typeInfo.H"

namespace pFlow
{

class uniformRandomReal
{
protected:

 	std::mt19937_64 engineGen_;

 	std::uniform_real_distribution<double> distrbution_;

public:

	// type info
	TypeNameNV("uniform");

	explicit uniformRandomReal()
	:
		engineGen_(std::random_device()()),
		distrbution_(0.0, 1.0)
	{}

	~uniformRandomReal()= default;

	inline real  randomNumber(real a, real b) 
	{
		return  a + (b-a)*distrbution_(engineGen_);
	}
	
	inline realx3 randomNumber(const realx3& a, const realx3& b)
	{
		realx3 r3
		(
			randomNumber(0.0,1.0),
			randomNumber(0.0,1.0),
			randomNumber(0.0,1.0)
		);

		return a + (b-a)*r3;
	}	

	inline realx3 operator()(const realx3& a, const realx3& b)
	{
		return randomNumber(a,b);
	}

	inline real operator()(real a, real b)
	{
		return randomNumber(a,b);
	}

};

}

#endif
