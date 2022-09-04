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

#include "contactSearch.H"



pFlow::contactSearch::contactSearch(
	const dictionary& dict,
	const box& domain,
 	const particles& prtcl,
 	const geometry& geom,
 	Timers& timers)
:
	interactionBase(prtcl, geom),
	domain_(domain),
	dict_(dict),
	sphereSphereTimer_("particle-particle contact search", &timers),
	sphereWallTimer_("particle-wall contact search", &timers)
{

}


pFlow::uniquePtr<pFlow::contactSearch> pFlow::contactSearch::create(
	const dictionary& dict,
	const box& domain,
 	const particles& prtcl,
 	const geometry& geom,
 	Timers& timers)
{

	word baseMethName	= dict.getVal<word>("method");
	word wallMethod 	= dict.getVal<word>("wallMapping");

	auto model = angleBracketsNames2("ContactSearch", baseMethName, wallMethod);


	Report(1)<<"Selecting contact search model . . ."<<endReport;
	

	if( dictionaryvCtorSelector_.search(model))
	{
		auto objPtr = dictionaryvCtorSelector_[model] (dict, domain, prtcl, geom, timers);
		Report(2)<<"Model "<< greenText(model)<<" is created."<<endReport;
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< model << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}