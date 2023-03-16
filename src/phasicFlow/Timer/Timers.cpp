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


#include "Timers.hpp"

pFlow::real pFlow::Timers::accTimersTotal()const
{
	// first this timer
	real total = 0;
	if(this->timerActive()) total += this->totalTime();

	for(const auto tmr:timers_)
	{
		if(tmr -> master())
		{
			total += dynamic_cast<const Timers*>(tmr)->accTimersTotal();
		}
		else if(tmr->timerActive())
		{
			total += tmr->totalTime();
		}
	}

	return total;
}


bool pFlow::Timers::write(iOstream& os, bool subTree)const
{
	if(level() == 0 )os<<"\n";
	Timer::write(os, subTree);
	if(subTree)
	{

		for(const auto& timer:timers_)
		{
			timer->write(os, subTree);
		}

	}
	if(level() == 0 )os<<"\n";
	return true;
}