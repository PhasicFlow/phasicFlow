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

#include "demComponent.hpp"
#include "systemControl.hpp"
#include "Time.hpp"

pFlow::demComponent::demComponent(const word& name, systemControl& control)
:
	control_(control),
	time_(control.time()),
	timers_(name, &control.timers()),
  componentName_(name)
{}

pFlow::real pFlow::demComponent::dt()const
{
	return time_.dt();
}

pFlow::real pFlow::demComponent::currentTime()const
{
   return time_.currentTime();
}

pFlow::uint32 pFlow::demComponent::currentIter() const
{
  return time_.currentIter();
}
