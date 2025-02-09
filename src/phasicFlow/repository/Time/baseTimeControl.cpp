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
#include "timeInfo.hpp"

pFlow::baseTimeControl::baseTimeControl
(
    const dictionary &dict,
    const word& intervalPrefix,
	timeValue defStartTime
)
:
	intervalPrefix_(intervalPrefix)
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

	if(!isTimeStep_)
	{
		auto startTime = (dict.getValOrSet<timeValue>("startTime", defStartTime));
    	auto endTime = (dict.getValOrSet<timeValue>("endTime", largeValue));
		auto interval = dict.getVal<timeValue>(intervalWord);
		rRange_ = stridedRange<timeValue>(startTime, endTime, interval);

	}
	else
	{
		auto startTime = (dict.getValOrSet<int32>("startTime", 0));
    	auto endTime = (dict.getValOrSet<int32>("endTime", largestPosInt32));
		auto interval = dict.getVal<int32>(intervalWord);
		iRange_ = int32StridedRagne(startTime, endTime, interval);
	}
    
}

pFlow::baseTimeControl::baseTimeControl
(
	const dictionary& dict,
	const timeValue   defInterval,
	const word&       intervalPrefix,
	const timeValue   defStartTime
)
:
	intervalPrefix_(intervalPrefix)
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

	if(!isTimeStep_)
	{
		auto startTime = (dict.getValOrSet<timeValue>("startTime", defStartTime));
    	auto endTime = (dict.getValOrSet<timeValue>("endTime", largeValue));
		auto interval = dict.getValOrSet<timeValue>(intervalWord, defInterval);
		rRange_ = stridedRange<timeValue>(startTime, endTime, interval);

	}
	else
	{
		auto startTime = (dict.getValOrSet<int32>("startTime", 0));
    	auto endTime = (dict.getValOrSet<int32>("endTime", largestPosInt32));
		auto interval = dict.getValOrSet<int32>(intervalWord, 1);
		iRange_ = int32StridedRagne(startTime, endTime, interval);
	}
}

pFlow::baseTimeControl::baseTimeControl(int32 start, int32 end, int32 stride, const word &intervalPrefix)
:
	isTimeStep_(true),
	iRange_(start, end, std::max(stride,1)),
	intervalPrefix_(
		intervalPrefix.size()==0uL? word("interval"): intervalPrefix+"Interval"
	)
{
}

bool pFlow::baseTimeControl::eventTime(uint32 iter, timeValue t, timeValue dt) const
{
	if(isTimeStep_)
	{	
		return iRange_.isMember(iter);
	}
	else
	{
		return rRange_.isMember(t, 0.55*dt);
	}
    return false;
}

bool pFlow::baseTimeControl::eventTime(const timeInfo &ti) const
{
	return eventTime(ti.iter(), ti.t(), ti.dt());
}

bool
pFlow::baseTimeControl::isInRange(uint32 iter, timeValue t, timeValue dt) const
{
	if(isTimeStep_)
	{
		return iRange_.isInRange(iter);
	}
	else
	{
		return rRange_.isInRange(t);
	}
}

pFlow::timeValue
pFlow::baseTimeControl::startTime() const
{
	if(!isTimeStep_)
	{
		return rRange_.begin();
	}

	fatalErrorInFunction<<"timeControl is not simulationTime or runTime"<<endl;
	fatalExit;
	return 0;
}

pFlow::timeValue
pFlow::baseTimeControl::endTime() const
{
	if(!isTimeStep_)
	{
		return rRange_.end();
	}

	fatalErrorInFunction<<"timeControl is not simulationTime or runTime"<<endl;
	fatalExit;
	return 0;
}

pFlow::timeValue
pFlow::baseTimeControl::rInterval() const
{
	if(!isTimeStep_)
	{
		return rRange_.stride();
	}
	fatalErrorInFunction<<"timeControl is not simulationTime or runTime"<<endl;
	fatalExit;
	return 0;
}

pFlow::int32
pFlow::baseTimeControl::startIter() const
{
	if(isTimeStep_)
	{
		return iRange_.begin();
	}
	fatalErrorInFunction<<"timeControl is not timeStep"<<endl;
	fatalExit;
	return 0;
}

pFlow::int32
pFlow::baseTimeControl::endIter() const
{
	if(isTimeStep_)
	{
		return iRange_.end();
	}
	fatalErrorInFunction<<"timeControl is not timeStep"<<endl;
	fatalExit;
	return 0;
}

pFlow::int32
pFlow::baseTimeControl::iInterval() const
{
	if(isTimeStep_)
	{
		return iRange_.stride();
	}
	fatalErrorInFunction<<"timeControl is not timeStep"<<endl;
	fatalExit;
	return 0;
}

bool
pFlow::baseTimeControl::write(dictionary& dict) const
{
	if(isTimeStep_)
	{
		dict.add("timeControl", "timeStep");
	}
	else
	{
		dict.add("timeControl", "runTime");
	}

	word intervalWord = intervalPrefix_.size()==0? word("interval"): intervalPrefix_+"Interval";
	
	if(!isTimeStep_)
	{
		dict.add(intervalWord,rRange_.stride());
		dict.add("startTime",rRange_.begin());
		dict.add("endTime", rRange_.end());
	}
	else
	{
		dict.add(intervalWord,iRange_.stride());
		dict.add("startTime",iRange_.begin());
		dict.add("endTime", iRange_.end());
	}
	return true;
}