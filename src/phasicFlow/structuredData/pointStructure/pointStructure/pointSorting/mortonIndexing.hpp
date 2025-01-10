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
#ifndef __mortonIndexing_hpp__
#define __mortonIndexing_hpp__

#include "types.hpp"
#include "box.hpp"
#include "indexContainer.hpp"
#include "pointFlag.hpp"

namespace pFlow
{

uint32IndexContainer getSortedIndices(
	box boundingBox,
	real dx,  
	const ViewType1D<realx3>& pos, 
	const pFlagTypeDevice& flag);


INLINE_FUNCTION_HD
uint64_t splitBy3(const uint64_t val){
	uint64_t x = val;
	x = (x | x << 32) & 0x1f00000000ffff;
	x = (x | x << 16) & 0x1f0000ff0000ff;
	x = (x | x << 8) & 0x100f00f00f00f00f;
	x = (x | x << 4) & 0x10c30c30c30c30c3;
	x = (x | x << 2) & 0x1249249249249249;
	return x;
}

INLINE_FUNCTION_HD
uint64_t xyzToMortonCode64(uint64_t x, uint64_t y, uint64_t z)
{
	return splitBy3(x) | (splitBy3(y) << 1) | (splitBy3(z) << 2);
}


INLINE_FUNCTION_HD
uint64_t getThirdBits(uint64_t x)
{
	x = x & 0x9249249249249249;
    x = (x | (x >> 2))  & 0x30c30c30c30c30c3;
    x = (x | (x >> 4))  & 0xf00f00f00f00f00f;
    x = (x | (x >> 8))  & 0x00ff0000ff0000ff;
    x = (x | (x >> 16)) & 0xffff00000000ffff;
    x = (x | (x >> 32)) & 0x00000000ffffffff;
    return x;
	
}

INLINE_FUNCTION_HD
void mortonCode64Toxyz(uint64_t morton, uint64_t& x, uint64_t& y, uint64_t& z)
{
	x = getThirdBits(morton);
	y = getThirdBits(morton >> 1);
	z = getThirdBits(morton >> 2);
}

struct indexMorton
{
	size_t morton;
	size_t index;
};


}

#endif //__mortonIndexing_hpp__
