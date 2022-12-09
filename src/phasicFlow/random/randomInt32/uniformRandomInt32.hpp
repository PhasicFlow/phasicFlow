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

#ifndef __uniformRandomInt32_hpp__
#define __uniformRandomInt32_hpp__

#include <random>

#include "types.hpp"
#include "typeInfo.hpp"

namespace pFlow
{

class uniformRandomInt32
{
protected:

 	std::mt19937_64 engineGen_;

 	std::uniform_int_distribution<int32> distrbution_;

public:

	// type info
	TypeInfoNV("uniform");

	explicit uniformRandomInt32(int32 min, int32 max)
	:
		engineGen_(std::random_device()()),
		distrbution_(min, max)
	{}

	~uniformRandomInt32()= default;

	inline real  randomNumber() 
	{
		return  distrbution_(engineGen_);
	}
	
	inline int32x3 randomNumber3()
	{
		return int32x3
		(
			randomNumber(),
			randomNumber(),
			randomNumber()
		);
	}	

	inline realx3 operator()()
	{
		return randomNumber();
	}

	

};

}

#endif
