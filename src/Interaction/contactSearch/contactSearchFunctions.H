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

#ifndef __broadSearchFunctions_H__
#define __broadSearchFunctions_H__

#include "types.H"
#include "iBox.H"

namespace pFlow
{

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

template<typename indexType, typename cellIndexType>
INLINE_FUNCTION_HD
void indexToCell(const indexType idx, const triple<cellIndexType>& extent, triple<cellIndexType>& cell)
{
	indexType nxny = extent.x()*extent.y();
	cell.z() = static_cast<cellIndexType>(idx / nxny);
	auto rem = idx % nxny;
	cell.y() = static_cast<cellIndexType>(rem / extent.x());
	cell.x() = static_cast<cellIndexType>(rem % extent.x());
}

template<typename cellIndexType>
INLINE_FUNCTION_HD
triple<cellIndexType> boxExtent( const iBox<cellIndexType>& box)
{
	return triple<cellIndexType>(
		box.maxPoint().x() - box.minPoint().x() + 1,
		box.maxPoint().y() - box.minPoint().y() + 1,
		box.maxPoint().z() - box.minPoint().z() + 1);
}

template<typename indexType, typename cellIndexType>
INLINE_FUNCTION_HD
void indexToCell(const indexType idx, const iBox<cellIndexType>& box, triple<cellIndexType>& cell)
{
	auto extent = boxExtent(box);
	indexToCell(idx, extent, cell);
	cell+= box.minPoint();
}






INLINE_FUNCTION_HD
bool sphereSphereCheck(const realx3& p1, const realx3 p2, real d1, real d2)
{
	return length(p2-p1) < 0.5*(d2+d1);
}

}

#endif //__broadSearchFunctions_H__
