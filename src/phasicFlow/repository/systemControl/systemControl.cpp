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

#include "systemControl.hpp"
#include "Lists.hpp"
#include "error.hpp"
#include "iOstream.hpp"
#include "types.hpp"
#include "vocabs.hpp"

bool pFlow::systemControl::readIncludeExclue(const dictionary& dict)
{
	if (dict.containsDataEntry("includeObjects"))
	{
		wordList incld = dict.getVal<wordList>("includeObjects");
		for (auto& nm : incld)
		{
			includeList_.insert(nm);
		}
	}

	if (dict.containsDataEntry("excludeObjects"))
	{
		wordList excld = dict.getVal<wordList>("excludeObjects");
		for (auto& nm : excld)
		{
			excludeList_.insert(nm);
		}
	}
	return true;
}

pFlow::word pFlow::systemControl::getRunName(const fileSystem& path)
{
	// gets the canonical form of path
	word wPath = path.canonical().wordPath() + "/";

	auto first = wPath.find_first_of('/');
	auto last  = wPath.find_last_of('/');

	if (first == word::npos)
	{
		fatalErrorInFunction << "path is empty \n";
		fatalExit;
	}

	if (last == wPath.size() - 1)
	{
		wPath = wPath.substr(0, last);
		last  = wPath.find_last_of('/');
	}

	word rName = wPath.substr(last + 1);

	return rName;
}

pFlow::word pFlow::systemControl::getTopFolder(const fileSystem& path)
{
	// gets the canonical form of path
	word wPath = path.canonical().wordPath();

	auto first = wPath.find_first_of('/');
	auto last  = wPath.find_last_of('/');

	if (first == word::npos)
	{
		fatalErrorInFunction << "path is empty \n";
		fatalExit;
	}

	if (last == wPath.size() - 1)
	{
		wPath = wPath.substr(0, last);
		last  = wPath.find_last_of('/');
	}

	word tFolder = wPath.substr(0, last);

	return tFolder;
}

pFlow::systemControl::systemControl(const fileSystem path)
  : repository(
        "systemControl",
        path,   // local path
        nullptr // no owner
    ),
    runName_(getRunName(path)),
    topLevelFolder_(getTopFolder(path)),
    settingsDict_(makeUnique<fileDictionary>(
        objectFile(
            settingsFile__,
            settingsFolder__,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_NEVER
        ),
        this
    )),
    Time_(this, settingsDict_()),
    settings_(
        makeUnique<repository>(settingsRepository__, settingsFolder__, this)
    ),
    caseSetup_(
        makeUnique<repository>(caseSetupRepository__, caseSetupFolder__, this)
    ),

    libs_(settingsDict_()),
    outFilePrecision_(
        settingsDict_().getValOrSet("outFilePrecision", static_cast<uint64>(6))
    ),
    timers_(runName_),
    timersReport_(settingsDict_().getValOrSet("timersReport", Logical("Yes"))),
    writeToFileTimer_("Write to file", &timers_)
{
	readIncludeExclue(settingsDict_());
}

pFlow::systemControl::systemControl(
    const real       startTime,
    const real       endTime,
    const real       saveInterval,
    const word       startTimeName,
    const fileSystem path
)
  : repository(
        "systemControl",
        path,   // local path
        nullptr // no owner
    ),
    runName_(getRunName(path)),
    topLevelFolder_(getTopFolder(path)),
    settingsDict_(makeUnique<fileDictionary>(
        objectFile(
            settingsFile__,
            settingsFolder__,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_NEVER
        ),
        this
    )),
    Time_(
        this,
        settingsDict_(),
        startTime,
        endTime,
        saveInterval,
        startTimeName
    ),
    settings_(
        makeUnique<repository>(settingsRepository__, settingsFolder__, this)
    ),
    caseSetup_(
        makeUnique<repository>(caseSetupRepository__, caseSetupFolder__, this)
    ),
    libs_(settingsDict_()),
    externalTimeControl_(true),
    timers_(runName_),
    timersReport_(settingsDict_->getValOrSet("timersReport", Logical("Yes"))),
    writeToFileTimer_("Write to file", &timers_)
{
	readIncludeExclue(settingsDict_());
}

bool pFlow::systemControl::operator++(int)
{
	auto toContinue = time()++;

	if (toContinue)
	{
		writeToFileTimer_.start();
		// if(time().currentIter() != 0 )
		{
			//- save the results to file
			if (!time().write())
			{
				fatalErrorInFunction;
				return false;
			}
		}
		writeToFileTimer_.end();

		if (time().timersReportTime() && timersReport())
		{
			timers_.write(mOutput, true);
		}
	}
	else if (time().finalTime())
	{
		writeToFileTimer_.start();
		if (!time().write())
		{
			fatalErrorInFunction;
			return false;
		}
		writeToFileTimer_.end();

		timers_.write(mOutput, true);
	}

	return toContinue;
}
