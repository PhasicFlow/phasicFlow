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

#include "timeFlowControl.hpp"
#include "dictionary.hpp"

size_t pFlow::timeFlowControl::numberToBeInserted(real currentTime)
{
	if(currentTime<startTime_)return 0;
	if(currentTime>endTime_) return 0;

	return static_cast<size_t>
	( 
		(currentTime - startTime_ + interval_)*rate_ - numInserted_ 
	);
}

bool pFlow::timeFlowControl::readTimeFlowControl
(
	const dictionary& dict
)
{
	rate_ = dict.getVal<real>("rate");
	startTime_ 	= dict.getVal<real>("startTime");
	endTime_	= dict.getVal<real>("endTime");
	interval_	= dict.getVal<real>("interval");
	numInserted_=0;
	return true;
}

bool pFlow::timeFlowControl::writeTimeFlowControl
(
	dictionary& dict
) const
{
	if(!dict.add("rate", rate_)) return false;
	if(!dict.add("startTime", startTime_)) return false;
	if(!dict.add("endTime", endTime_)) return false;
	if(!dict.add("interval", interval_)) return false;

	return true;
}


pFlow::timeFlowControl::timeFlowControl
(
	const dictionary& dict
)
{

	if(!readTimeFlowControl(dict))
	{
		fatalExit;
	}

}

bool pFlow::timeFlowControl::insertionTime( real currentTime, real dt)
{
	if(currentTime < startTime_) return false;

	if(currentTime > endTime_) return false;
	if( mod(abs(currentTime-startTime_),interval_)/dt < 1 ) return true;

	return false;
}