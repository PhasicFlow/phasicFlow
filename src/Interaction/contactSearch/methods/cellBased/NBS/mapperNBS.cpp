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

#include "mapperNBS.hpp"
#include "mapperNBSKernels.hpp"
#include "streams.hpp"

pFlow::uint32 pFlow::mapperNBS::checkInterval_ = 1000;
pFlow::real pFlow::mapperNBS::enlargementFactor_ = 1;

bool pFlow::mapperNBS::setSearchBox
(
    const deviceViewType1D<realx3> &pointPos, 
    const pFlagTypeDevice &flags,
    real cellSize
)
{
    box domainBox = domainCells_.domainBox();

	
    if(adjustableBox_)
    {
        lastCheckForBox_ = buildCount_;
        
        realx3 minP;
        realx3 maxP;
        pFlow::mapperNBSKernels::findPointExtends
        (
            pointPos, 
            flags, 
            minP, maxP
        );

        minP = max( minP - enlargementFactor_*cellSize, domainBox.minPoint());
        maxP = min( maxP + enlargementFactor_*cellSize, domainBox.maxPoint());       
        
        box searchBox = {minP, maxP};
        searchCells_ = cells(searchBox, cellSize);    
        INFORMATION<<"Search box for contact search has changed: "<< 
        "search box is ["<<searchCells_.domainBox().minPoint()<<
        " "<<searchCells_.domainBox().maxPoint()<<"]"<<END_INFO;
        
        return true;
        
    }
    else
    {
        searchCells_ = cells(domainBox, cellSize);
        INFORMATION<<"Search box for contact search is: ["
        << domainBox.minPoint()<<" "<<domainBox.maxPoint()<<"]"<<END_INFO;

        return false;
    }
    
}

void pFlow::mapperNBS::allocateArrays(rangeU32 nextRng)
{
    checkAllocateNext(nextRng);
    nullifyNext(nextRng);
    reallocFill(head_, searchCells_.nx(), searchCells_.ny(), searchCells_.nz(), NoPos);
}

void pFlow::mapperNBS::checkAllocateNext(rangeU32 nextRng)
{

    auto newCap = nextRng.end();
    
    if( nextCapacity_ < newCap)
    {
        nextCapacity_ = newCap;	
        if(!nextOwner_)return;
        reallocNoInit(next_, nextCapacity_);
    }
}

void pFlow::mapperNBS::nullifyHead()
{
    fill(head_, NoPos);	
}

void pFlow::mapperNBS::nullifyNext(rangeU32 nextRng)
{
    if(!nextOwner_)return;
		fill(next_, nextRng, NoPos);
}

pFlow::mapperNBS::mapperNBS(
    const box &domain,
    real cellSize,
    const deviceViewType1D<realx3> &pointPos,
    const pFlagTypeDevice &flags,
    bool adjustableBox,
    bool nextOwner
)
: 
   	domainCells_(domain, cellSize),
    searchCells_(domain, cellSize),
   	adjustableBox_(adjustableBox),
	nextOwner_(nextOwner)
{
    setSearchBox(pointPos, flags, cellSize);

    allocateArrays(flags.activeRange());
}

bool pFlow::mapperNBS::build
(
    const deviceViewType1D<realx3>& pointPos, 
    const pFlagTypeDevice & flags,
    bool&  searchBoxChanged
)
{
    auto aRange = flags.activeRange();
    buildCount_++;
    if(adjustableBox_ && buildCount_%checkInterval_ == 0)
    {   

        if(searchBoxChanged = 
            setSearchBox(pointPos, flags, searchCells_.cellSize());searchBoxChanged)
        {
            allocateArrays(aRange);
        }  
        
    }
    else
    {
        checkAllocateNext(aRange);
        nullifyHead();
        nullifyNext(aRange);
    }    
    
    if( adjustableBox_ )
    {
        if(!pFlow::mapperNBSKernels::buildListsReduce(
            searchCells_,
            head_,
            next_,
            pointPos,
            flags) )
        {

            buildCount_++;           
            setSearchBox(pointPos, flags, searchCells_.cellSize());
            
            searchBoxChanged = true;
            
            allocateArrays(flags.activeRange());

            if(!pFlow::mapperNBSKernels::buildListsReduce(
                searchCells_,
                head_,
                next_,
                pointPos,
                flags))
            {
                fatalErrorInFunction<<"failed to build list in anjustable search box mode!"<<endl;
                return false;   
            }
        }
    }
    else
    {
        pFlow::mapperNBSKernels::buildLists(
            searchCells_,
            head_,
            next_,
            pointPos,
            flags
        );
        searchBoxChanged = false;
    }
    
    return true;

}

