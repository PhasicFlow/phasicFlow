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

#include "DEMSystem.hpp"


pFlow::coupling::DEMSystem::DEMSystem(
	word  demSystemName,
	int32 numDomains, 
	const std::vector<box>& domains, 
	int argc, 
	char* argv[])
:
	ControlDict_(),
	domains_(domains)
{
  
	REPORT(0)<<"\nCreating Control repository . . ."<<endREPORT;
	Control_ = makeUnique<systemControl>(
		ControlDict_.startTime(),
		ControlDict_.endTime(),
		ControlDict_.saveInterval(),
		ControlDict_.startTimeName());

}

pFlow::coupling::DEMSystem::~DEMSystem()
{}


pFlow::uniquePtr<pFlow::coupling::DEMSystem>
	pFlow::coupling::DEMSystem::create(
		word  demSystemName,
		int32 numDomains, 
		const std::vector<box>& domains, 
		int argc, 
		char* argv[]
		)
{
	if( wordvCtorSelector_.search(demSystemName) )
	{
		return wordvCtorSelector_[demSystemName] (demSystemName, numDomains, domains, argc, argv);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< demSystemName << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		return nullptr;
	}

	return nullptr;
}

