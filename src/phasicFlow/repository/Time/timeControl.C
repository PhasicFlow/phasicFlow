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


#include "timeControl.H"
#include "dictionary.H"


bool pFlow::timeControl::screenReport()const
{
	return screenReportInterval_.isMember(currentIter_);
}

pFlow::timeControl::timeControl
(
	const dictionary& dict
)
:
	dt_
	(
		dict.getVal<real>("dt")
	),
	startTime_
	(
		dict.getVal<real>("startTime")
	),
	endTime_
	(
		dict.getVal<real>("endTime")
	),
	currentTime_(startTime_),
	saveInterval_
	(
		dict.getVal<real>("saveInterval")
	),
	lastSaved_(startTime_),
	currentIter_(0),
	timePrecision_
	(
		dict.getValOrSet("timePrecision", 4)
	),
	timersReportInterval_
	(
		startTime_,
		dict.getValOrSet("timersReportInterval", 0.04)
	)

{
	checkForOutputToFile();
}

pFlow::timeControl::timeControl(
		dictionary& dict,
		real startTime,
		real endTime,
		real saveInterval,
		word startTimeName)
:
	dt_
	(
		dict.getVal<real>("dt")
	),
	startTime_(startTime),
	endTime_(endTime),
	currentTime_(startTime_),
	saveInterval_(saveInterval),
	lastSaved_(startTime_),
	currentIter_(0),
	timePrecision_
	(
		dict.getValOrSet("timePrecision", 4)
	),
	managedExternaly_(true),
	timeName_(startTimeName),
	timersReportInterval_
	(
		startTime_,
		dict.getValOrSet("timersReportInterval", 0.04)
	)	
{
	checkForOutputToFile();
}


void pFlow::timeControl::checkForOutputToFile()
{
	
	if(managedExternaly_) return;

	bool save = false;
	
	if ( abs(currentTime_ - lastSaved_ - saveInterval_) < 0.5 * dt_ )
	{	
		lastSaved_ = currentTime_;
		save = true;
	}
	else if( abs(currentTime_ - lastSaved_) < min( pow(10.0,-1.0*timePrecision_), 0.5 *dt_) )
	{
		lastSaved_ = currentTime_;
		save = true;
	}

	outputToFile_ = save;
}

bool pFlow::timeControl::timersReportTime()const
{
	if(currentIter_<=1)return false;
	return timersReportInterval_.isMember(currentTime_, dt_);
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

