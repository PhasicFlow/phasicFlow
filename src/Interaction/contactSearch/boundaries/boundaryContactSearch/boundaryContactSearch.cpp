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

#include "boundaryContactSearch.hpp"
#include "contactSearch.hpp"

pFlow::boundaryContactSearch::boundaryContactSearch(
    const dictionary &dict,
    const boundaryBase &boundary,
    const contactSearch &cSearch)
    : generalBoundary(
          boundary,
          cSearch.pStruct(),
          "",
          ""),
      contactSearch_(cSearch),
      updateInterval_(dict.getVal<uint32>("updateInterval"))
{
}

pFlow::uniquePtr<pFlow::boundaryContactSearch>
pFlow::boundaryContactSearch::create(
	const dictionary &dict,
	const boundaryBase &boundary,
	const contactSearch &cSearch)
{
	
	word bType = angleBracketsNames2( 
		"boundaryContactSearch",
		pFlowProcessors().localRunTypeName(), 
		boundary.type());
	word altBType{"boundaryContactSearch<none>"};

	if( boundaryBasevCtorSelector_.search(bType) )
	{
		REPORT(2)<<"Creating contact search boundary "<< Green_Text(bType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[bType](dict, boundary, cSearch);
	}
	else if(boundaryBasevCtorSelector_.search(altBType))
	{
		REPORT(2)<<"Creating contact search boundary "<< Green_Text(altBType)<<
		" for "<<boundary.name()<<endl;
		return boundaryBasevCtorSelector_[altBType](dict, boundary, cSearch);
	}
  else
  {
    printKeys
		( 
			fatalError << "Ctor Selector "<< bType<<
      " and "<< altBType << " do not exist. \n"
			<<"Avaiable ones are: \n"
			,
			boundaryBasevCtorSelector_
		);
		fatalExit;
  }
	
	return nullptr;
}
