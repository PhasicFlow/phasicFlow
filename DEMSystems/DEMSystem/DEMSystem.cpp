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

// from phasicFlow
#include "KokkosTypes.hpp"

#include "DEMSystem.hpp"


pFlow::DEMSystem::DEMSystem(
	word  demSystemName,
	const std::vector<box>& domains, 
	int argc, 
	char* argv[])
:
	ControlDict_()
{
  
	REPORT(0)<<"Initializing host/device execution spaces . . . \n";
	REPORT(1)<<"Host execution space is "<< Green_Text(DefaultHostExecutionSpace::name())<<END_REPORT;
	REPORT(1)<<"Device execution space is "<<Green_Text(DefaultExecutionSpace::name())<<END_REPORT;

 	// initialize Kokkos
	Kokkos::initialize( argc, argv ); 

	REPORT(0)<<"\nCreating Control repository . . ."<<END_REPORT;
	Control_ = makeUnique<systemControl>(
		ControlDict_.startTime(),
		ControlDict_.endTime(),
		ControlDict_.saveInterval(),
		ControlDict_.startTimeName());

	timers_ = makeUnique<Timers>(demSystemName, &Control_().timers());

}

pFlow::DEMSystem::~DEMSystem()
{
	
	Control_.reset();

	output<< "\nFinalizing host/device execution space ...."<<endl;
	Kokkos::finalize();
}


pFlow::uniquePtr<pFlow::DEMSystem>
	pFlow::DEMSystem::create(
		word  demSystemName,
		const std::vector<box>& domains, 
		int argc, 
		char* argv[]
		)
{
	if( wordvCtorSelector_.search(demSystemName) )
	{
		return wordvCtorSelector_[demSystemName] (demSystemName, domains, argc, argv);
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
