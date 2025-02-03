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

#include "contactSearch.hpp"
#include "streams.hpp"
#include "particles.hpp"


pFlow::contactSearch::contactSearch(
	const dictionary& dict,
	const box& extDomain,
 	const particles& prtcl,
 	const geometry& geom,
 	Timers& timers)
:
	extendedDomainBox_(extDomain),
	updateInterval_(dict.getValMax<uint32>("updateInterval", 1u)),
	particles_(prtcl),
	geometry_(geom),
	bTimer_("Boundary particles contact search", &timers),
	ppTimer_("Internal particles contact search", &timers)
{

}

const pFlow::pointStructure &pFlow::contactSearch::pStruct() const
{
   return particles_.pStruct();
}

bool pFlow::contactSearch::broadSearch
(
	const timeInfo &ti, 
	csPairContainerType &ppPairs, 
	csPairContainerType &pwPairs, 
	bool force
)
{

	if(enterBroadSearch(ti, force))
	{
		ppTimer_.start();
		if( !BroadSearch(
			ti,
			ppPairs,
			pwPairs,
			force ) )
		{
			fatalErrorInFunction;
			performedSearch_ = false;
			return false;
		}
		ppTimer_.end();
		performedSearch_ = true;
		lastUpdated_ = ti.currentIter();
	}
	else
	{
		performedSearch_ = false;
	}

	return true;
}

bool pFlow::contactSearch::boundaryBroadSearch
(
	uint32 bndryIndex, 
	const timeInfo &ti, 
	csPairContainerType &ppPairs, 
	csPairContainerType &pwPairs, 
	bool force
)
{
	if(enterBroadSearchBoundary(ti, force))
	{
		bTimer_.start();
		for(uint32 i=0u; i<6u; i++)
		{
			//output<<" boundarySearch "<< i <<" for iter "<< ti.iter()<<endl;
			if(!BoundaryBroadSearch(
				i,
				ti,
				ppPairs,
				pwPairs,
				force))
			{
				performedSearchBoundary_ = false;
				return false;
			}
		}
		bTimer_.end();
		performedSearchBoundary_ = true;
	}
	else
	{

		performedSearchBoundary_ = false;
	}

    return true;
}

pFlow::uniquePtr<pFlow::contactSearch> pFlow::contactSearch::create(
    const dictionary &dict,
    const box &extDomain,
    const particles &prtcl,
    const geometry &geom,
    Timers &timers)
{
	word baseMethName	= dict.getVal<word>("method");	

	auto model = angleBracketsNames("ContactSearch", baseMethName);
	
	pOutput.space(2)<<"Selecting contact search model "<<Green_Text(model)<<endl;	
	if( dictionaryvCtorSelector_.search(model))
	{
		auto objPtr = dictionaryvCtorSelector_[model] (dict, extDomain, prtcl, geom, timers);
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< model << " does not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}