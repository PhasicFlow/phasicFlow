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

#include "math.hpp"
#include "timeControl.hpp"
#include "dictionary.hpp"


bool pFlow::timeControl::screenReport()const
{
	return screenReportInterval_.isMember(ti_.currentIter());
}

pFlow::timeControl::timeControl
(
	const dictionary& dict
)
:
	ti_(dict),
	startTime_
	(
		dict.getVal<TimeValueType>("startTime")
	),
	endTime_
	(
		dict.getVal<TimeValueType>("endTime")
	),
	stopAt_(endTime_),
	saveInterval_
	(
		dict.getVal<TimeValueType>("saveInterval")
	),
	lastSaved_(startTime_),
	performSorting_
	(
		dict.getValOrSet("performSorting", Logical("No"))
	)
{
	timersReportInterval_ = timeStridedRange(
		startTime_, 
		dict.getValOrSet("timersReportInterval", 0.04));

	sortingInterval_ = timeStridedRange(
		startTime_, 
		dict.getValOrSet("sortingInterval", 1.0));
	checkForOutputToFile();
}

pFlow::timeControl::timeControl(
		dictionary& dict,
		TimeValueType startTime,
		TimeValueType endTime,
		TimeValueType saveInterval,
		word startTimeName)
:
	ti_(startTime, dict),
	startTime_(startTime),
	endTime_(endTime),
	stopAt_(endTime_),
	saveInterval_(saveInterval),
	lastSaved_(startTime_),	
	managedExternaly_(true),
	performSorting_
	(
		dict.getValOrSet("performSorting", Logical("No"))
	)	
{
	timeName_ = startTimeName;

	timersReportInterval_ = timeStridedRange(
		startTime_, 
		dict.getValOrSet("timersReportInterval", 0.04));

	sortingInterval_ = timeStridedRange(
		startTime_, 
		dict.getValOrSet("sortingInterval", 1.0));

	checkForOutputToFile();
}

pFlow::TimeValueType pFlow::timeControl::setTime(TimeValueType t)
{
	TimeValueType tmp = ti_.currentTime();
	ti_.currentTime_ = t;
	lastSaved_ = t;
	checkForOutputToFile();
	return tmp;
}

pFlow::word pFlow::timeControl::timeName()const
{
	if(managedExternaly_)
		return timeName_;
	else
		return currentTimeWord();
}

bool pFlow::timeControl::finalTime()const
{
	if( ti_.currentTime_ >= endTime_ ) return true;
	if( std::abs(ti_.currentTime_-endTime_) < 0.5*ti_.dt_ )return true;
	return false;	
}

bool pFlow::timeControl::reachedStopAt()const
{
	if( ti_.currentTime_ >= stopAt_ ) return true;
	if( std::abs(ti_.currentTime_-stopAt_) < 0.5*ti_.dt_ )return true;
	return false;
}

void pFlow::timeControl::checkForOutputToFile()
{
	
	bool save = false;
	if(managedExternaly_)
	{
		if( std::abs(ti_.currentTime_-writeTime_) < 0.5*ti_.dt_)
		{
			save = true;
			lastSaved_ = ti_.currentTime_;
		}
	}
	else
	{
		if ( std::abs(ti_.currentTime_ - lastSaved_ - saveInterval_) < 0.5 * ti_.dt_ )
		{	
			lastSaved_ = ti_.currentTime_;
			save = true;
		}
		else if( std::abs(ti_.currentTime_ - lastSaved_) < std::min( std::pow(10.0,-1.0*ti_.precision()), 0.5 *ti_.dt_) )
		{
			lastSaved_ = ti_.currentTime_;
			save = true;
		}
		
	}

	outputToFile_ = save;
}

bool pFlow::timeControl::timersReportTime()const
{
	if(ti_.currentIter_<=1)return false;
	return timersReportInterval_.isMember(ti_.currentTime_, 0.55*ti_.dt_);
}

bool pFlow::timeControl::sortTime()const
{
	return performSorting_()&&sortingInterval_.isMember(ti_.currentTime_,ti_.dt_);
}

void pFlow::timeControl::setSaveTimeFolder(
	bool saveToFile, 
	const word& timeName)
{
	if(managedExternaly_)
	{
		outputToFile_ = saveToFile;
		timeName_ = timeName;
	}
}

bool pFlow::timeControl::operator ++(int)
{

	if( reachedStopAt() ) return false;
	
	// increament iteration number 
	ti_.martchDT();

	if(screenReport() && !managedExternaly_)
	{
		REPORT(0)<<"Time (s): "<<Cyan_Text( currentTimeWord() )<<END_REPORT;
	}
	// switch outputToFile_ on/off
	checkForOutputToFile();

	return true;
}