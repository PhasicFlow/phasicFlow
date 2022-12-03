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


#include "types.H"
#include "iOstream.H"
#include "error.H"
#include "systemControl.H"
#include "vocabs.H"


pFlow::word pFlow::systemControl::getRunName
(
	const fileSystem& path
)
{

	// gets the canonical form of path
	word wPath = path.canonical().wordPath()+"/";

	auto first = wPath.find_first_of('/');
    auto last  = wPath.find_last_of('/');

    if( first == word::npos)
    {
    	fatalErrorInFunction <<
    	"path is empty \n";
    	fatalExit; 
    }
    
    if( last == wPath.size()-1)
    {
    	wPath = wPath.substr(0,last);
    	last = wPath.find_last_of('/');
    }
	
    word rName = wPath.substr(last+1);
    
	return rName;

}

pFlow::word pFlow::systemControl::getTopFolder
(
	const fileSystem& path
)
{

	// gets the canonical form of path
	word wPath = path.canonical().wordPath();

	auto first = wPath.find_first_of('/');
    auto last  = wPath.find_last_of('/');

    if( first == word::npos)
    {
    	fatalErrorInFunction <<
    	"path is empty \n";
    	fatalExit; 
    }

    if( last == wPath.size()-1)
    {
    	wPath = wPath.substr(0,last);
    	last = wPath.find_last_of('/');
    }

    word tFolder = wPath.substr(0,last);
    
	return tFolder;

}


pFlow::systemControl::systemControl
(
	const fileSystem path
)
:
	repository
	(
		"systemControl",
		path, 		// local path
		nullptr    // no owner
	),
	runName_
	(
		getRunName(path)
	),
	topLevelFolder_
	(	
		getTopFolder(path)
	),
	settings_
	(
		settingsRepository__,
		settingsFolder__,
		this
	),
	caseSetup_
	(
		caseSetupRepository__,
		caseSetupFolder__,
		this
	),
	settingsDict_
	(
		settings().emplaceObject<dictionary>
		(
			objectFile
			(
				settingsFile__,
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_NEVER
			),
			settingsFile__,
			true
		)
	),
	Time_
	(
		this,
		settingsDict_
	),
	g_(
		settingsDict_.getVal<realx3>("g")
	),
	domain_(
		settingsDict_.subDict("domain")
	),
	timers_(runName_),
	timersReport_
	(
		settingsDict_.getValOrSet("timersReport", Logical("Yes"))
	),
	writeToFileTimer_("Write to file", &timers_)	
{}

pFlow::systemControl::systemControl(
		const real startTime, 
		const real endTime,
		const real saveInterval,
		const word startTimeName,
		const fileSystem path)
:
	repository
	(
		"systemControl",
		path, 		// local path
		nullptr    // no owner
	),
	runName_
	(
		getRunName(path)
	),
	topLevelFolder_
	(	
		getTopFolder(path)
	),
	settings_
	(
		settingsRepository__,
		settingsFolder__,
		this
	),
	caseSetup_
	(
		caseSetupRepository__,
		caseSetupFolder__,
		this
	),
	settingsDict_
	(
		settings().emplaceObject<dictionary>
		(
			objectFile
			(
				settingsFile__,
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_NEVER
			),
			settingsFile__,
			true
		)
	),
	Time_
	(
		this,
		settingsDict_
	),
	externalTimeControl_(true),
	g_(
		settingsDict_.getVal<realx3>("g")
	),
	domain_(
		settingsDict_.subDict("domain")
	),
	timers_(runName_),
	timersReport_
	(
		settingsDict_.getValOrSet("timersReport", Logical("Yes"))
	),
	writeToFileTimer_("Write to file", &timers_)	
{}


bool pFlow::systemControl::operator ++(int)
{

	// skip writing to file for the first iteration 
	writeToFileTimer_.start();
	if(time().currentIter() != 0)
	{
		//- save the results to file  
		if( !time().write() )
		{
			fatalErrorInFunction;
			return false;
		}
	}
	else if( time().finished() )
	{
		if( !time().write() )
		{
			fatalErrorInFunction;
			return false;
		}
	}
	writeToFileTimer_.end();

	if( time().timersReportTime() &&
		timersReport() )
	{
		timers_.write(output, true);
	}
		
	return time()++;

}


