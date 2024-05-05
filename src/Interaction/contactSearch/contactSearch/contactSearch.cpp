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
	particles_(prtcl),
	geometry_(geom),
	ppTimer_("particle-particle contact search", &timers),
	pwTimer_("particle-wall contact search", &timers),
	dict_(dict)
{

}

const pFlow::pointStructure &pFlow::contactSearch::pStruct() const
{
   return particles_.pStruct();
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