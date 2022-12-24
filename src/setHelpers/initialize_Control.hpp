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

#ifndef __initializedControl_hpp__
#define __initializedControl_hpp__

// initilized and finalize should be placed in onc scope 
#include "initialize.hpp"

REPORT(0)<<"\nCreating Control repository . . ."<<endREPORT;
pFlow::uniquePtr<pFlow::systemControl> ControlPtr = nullptr;

if(isCoupling)
{
	pFlow::readControlDict controlDict;
	
	ControlPtr = pFlow::makeUnique<pFlow::systemControl>
	(
		controlDict.startTime(),
		controlDict.endTime(),
		controlDict.saveInterval(),
		controlDict.startTimeName()
	);	
}
else
{
  	ControlPtr = pFlow::makeUnique<pFlow::systemControl>();
}

auto& Control = ControlPtr();

#endif
