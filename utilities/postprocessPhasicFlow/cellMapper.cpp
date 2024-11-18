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

#include "cellMapper.hpp"
//#include "cellMapperKernels.hpp"
#include "streams.hpp"


void pFlow::cellMapper::allocateArrays(rangeU32 nextRng)
{
    checkAllocateNext(nextRng);
    nullifyNext(nextRng);
    reallocFill(head_, domainCells_.nx(), domainCells_.ny(), domainCells_.nz(), NoPos);
}

void pFlow::cellMapper::checkAllocateNext(rangeU32 nextRng)
{

    auto newCap = nextRng.end();
    
    if( nextCapacity_ < newCap)
    {
        nextCapacity_ = newCap;	
        reallocNoInit(next_, nextCapacity_);
    }
}

void pFlow::cellMapper::nullifyHead()
{
    fill(head_, NoPos);	
}

void pFlow::cellMapper::nullifyNext(rangeU32 nextRng)
{
	fill(next_, nextRng, NoPos);
}

pFlow::cellMapper::cellMapper(
    const rectangleMesh& rectMesh,
	const hostViewType1D<realx3>& pointPos,
	const pFlagTypeHost& flags
)
: 
   	domainCells_(rectMesh)
{

    allocateArrays(flags.activeRange());
}

bool pFlow::cellMapper::build
(
    const hostViewType1D<realx3>& pointPos, 
    const pFlagTypeHost & flags
)
{
    auto aRange = flags.activeRange();
    
    checkAllocateNext(aRange);
    nullifyHead();
    nullifyNext(aRange);
	int32x3 ind;
	for(uint32 i=aRange.start(); i<aRange.end(); i++)
	{
		if( domainCells_.isInsideIndex(pointPos[i], ind) )
		{
			next_[i] = head_(ind.x(), ind.y(), ind.z());
			head_(ind.x(), ind.y(), ind.z()) = i;
		}
	}     
    
    return true;

}

