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

#include "baseTimeControl.hpp"



pFlow::baseTimeControl::baseTimeControl
(
    const dictionary &dict,
    const word& intervalPrefix,
	real defStartTime
)
{
	auto tControl = dict.getVal<word>("timeControl");
	if(tControl == "timeStep")
		isTimeStep_ = true;
	else if( tControl == "runTime" || 
			 tControl == "simulationTime")
		isTimeStep_ = false;
	else
	{
		fatalErrorInFunction<<
		"bad input for intervalControl in dictionary "<< dict.globalName()<<endl<<
		"valid inputs are "<< 
		wordList({"timeStep", "runTime", "simulationTime"})<<endl;
		fatalExit;
	}

	word intervalWord = intervalPrefix.size()==0? word("interval"): intervalPrefix+"Interval";

	if(isTimeStep_)
	{
		auto startTime = (dict.getValOrSet<real>("startTime", defStartTime));
    	auto endTime = (dict.getValOrSet<real>("endTime", largeValue));
		auto interval = dict.getVal<real>(intervalWord);
		rRange_ = realStridedRange(startTime, endTime, interval);

	}
	else
	{
		auto startTime = (dict.getValOrSet<int32>("startTime", 0));
    	auto endTime = (dict.getValOrSet<int32>("endTime", largestPosInt32));
		auto interval = dict.getVal<int32>(intervalWord);
		iRange_ = int32StridedRagne(startTime, endTime, interval);
	}
    
}

bool pFlow::baseTimeControl::timeEvent(uint32 iter, real t, real dt) const
{
	if(isTimeStep_)
	{	
		return iRange_.isMember(iter);
	}
	else
	{
		return rRange_.isMember(t, 0.51*dt);
	}
    return false;
}
