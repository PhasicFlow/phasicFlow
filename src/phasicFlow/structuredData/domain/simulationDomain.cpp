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

#include "simulationDomain.hpp"
#include "pFlowProcessors.hpp"

pFlow::simulationDomain::simulationDomain(const dictionary& dict)
:
    globalBox_(dict.subDict("globalBox")),
    globalDomainDict_(dict)
{
    
}

pFlow::uniquePtr<pFlow::simulationDomain> 
    pFlow::simulationDomain::create(const dictionary &dict)
{
	word sType = angleBracketsNames(
        "simulationDomain", 
        pFlowProcessors().localRunTypeName());


	if( dictionaryvCtorSelector_.search(sType) )
	{
		return dictionaryvCtorSelector_[sType] (dict);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< sType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
    return nullptr;
}
