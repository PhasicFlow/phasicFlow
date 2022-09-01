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


#ifndef __bitTransfer_H__
#define __bitTransfer_H__


#include "types.H"
#include "Vectors.H"

namespace pFlow
{

// a simple functor that transfers 3 integers into a long variable and 
// transfer the long to 3 integers

class bitTransfer
{
protected:

	static const int numBits_ = 21;
	static const int numBits2_ = 2 * numBits_;
	static const unsigned long mask1_ = 0x000000000001FFFFF;
	static const unsigned long mask2_ = 0x0000003FFFFE00000;
	static const unsigned long mask3_ = 0x07FFFFC0000000000;

public:

	bitTransfer(){}

	inline unsigned long operator()(const unit3& int3 )
	{
		return 
			static_cast<long>(int3.x()) |
			static_cast<long>(int3.y()) << numBits_ |
			static_cast<long>(int3.z()) << numBits2_;
	}

	inline unit3 operator() (const unsigned long& ul )
	{
		return unit3
			(
				 ul & mask1_,
				(ul & mask2_)>> numBits_,
				(ul & mask3_)>> numBits2_
			);
	}
};

}


#endif
