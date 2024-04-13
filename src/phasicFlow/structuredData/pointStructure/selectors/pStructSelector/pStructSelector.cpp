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


#include "pStructSelector.hpp"
#include "pointStructure.hpp"
#include "dictionary.hpp"

pFlow::pStructSelector::pStructSelector
(
	const pointStructure& pStruct,
	const dictionary& UNUSED(dict)
)
:
	pStruct_(pStruct)
{}

const pFlow::pointStructure& pFlow::pStructSelector::pStruct()const
{
	return pStruct_;
}

pFlow::realx3Vector
pFlow::pStructSelector::selectedPointPositions() const
{

	const auto& slctd = selectedPoints();

	if(slctd.empty()) return realx3Vector("selectedPointPositions");
	
	realx3Vector slctdPoints("selectedPointPositions", slctd.size());
	
	auto pPos = pStruct().pointPositionHost();

	for(uint32 i=0; i<slctd.size(); i++)
	{
		slctdPoints[i] = pPos[slctd[i]];
	}

	return slctdPoints;
}

pFlow::uniquePtr<pFlow::pStructSelector>
pFlow::pStructSelector::create(
  const pointStructure& pStruct,
  const dictionary&     dict
)
{

	word selectorMethod = angleBracketsNames("selector", dict.getVal<word>("selector"));

	if( dictionaryvCtorSelector_.search(selectorMethod) )
	{
		return dictionaryvCtorSelector_[selectorMethod] (pStruct, dict);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< selectorMethod << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
