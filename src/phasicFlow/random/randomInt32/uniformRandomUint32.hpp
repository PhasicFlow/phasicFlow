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

#ifndef __uniformRandomUint32_hpp__
#define __uniformRandomUint32_hpp__

#include <random>

#include "types.hpp"
#include "typeInfo.hpp"

namespace pFlow
{

class uniformRandomUint32
{
protected:

 	std::mt19937_64 engineGen_;

 	std::uniform_int_distribution<uint32> distrbution_;

public:

	// type info
	TypeInfoNV("uniform");

	explicit uniformRandomUint32(uint32 min, uint32 max)
	:
		engineGen_(std::random_device()()),
		distrbution_(min, max)
	{}

	~uniformRandomUint32()= default;

	inline uint32  randomNumber() 
	{
		return  distrbution_(engineGen_);
	}
	
	inline triple<uint32> randomNumber3()
	{
		return triple<uint32>
		(
			randomNumber(),
			randomNumber(),
			randomNumber()
		);
	}	

	inline triple<uint32> operator()()
	{
		return randomNumber3();
	}

	

};

}

#endif
