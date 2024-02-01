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
#include "systemControl.hpp"
#include "vocabs.hpp"

pFlow::simulationDomain::simulationDomain(systemControl& control)
:
	fileDictionary
	(
		objectFile
		(
			domainFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		&control.settings()
	),
    globalBox_(subDict("globalBox"))
{
    
}

pFlow::domain pFlow::simulationDomain::extendThisDomain
(
	const realx3 &lowerPointExtension, 
	const realx3 &upperPointExtension
) const
{
    realx3 minP = thisDomain().domainBox().minPoint() + lowerPointExtension;
	realx3 maxP = thisDomain().domainBox().maxPoint() + upperPointExtension;
	return domain({minP, maxP});
}

pFlow::uniquePtr<pFlow::simulationDomain>
pFlow::simulationDomain::create(systemControl &control)
{
	word sType = angleBracketsNames(
        "simulationDomain", 
        pFlowProcessors().localRunTypeName());


	if( systemControlvCtorSelector_.search(sType) )
	{
		return systemControlvCtorSelector_[sType] (control);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< sType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
	}
    return nullptr;
}
