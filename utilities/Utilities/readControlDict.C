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
#include "readControlDict.H"
#include "iFstream.H"
#include "timeFolder.H"

pFlow::word pFlow::readControlDict::convertTimeToName(
	const real t, 
	const int32 precision)const
{
    std::ostringstream buf;
    if( formatType_ == "general")
    	buf.setf(ios_base::fmtflags(0), ios_base::floatfield);
    else if(formatType_ == "scientific")
		buf.setf(ios_base::fmtflags(ios_base::scientific), ios_base::floatfield);
    else if(formatType_ == "fixed")
    	buf.setf(ios_base::fmtflags(ios_base::fixed), ios_base::floatfield);
	else
	{
		fatalErrorInFunction<<"the timeFormat is not correct, it should be "
		" (genral, scientific, fixe) but you supplied "<< formatType_<<endl;
		fatalExit;
	}

    buf.precision(precision);
    buf << t;
    return buf.str();
}

bool pFlow::readControlDict::read()
{

	iFstream cdFile(cdPath_);

	word startFrom;

	if( !cdFile.findTokenAndNextSilent("startFrom", startFrom))
	{
		startFrom = "latestTime";
	}

	if(startFrom == "startTime")
	{
		if(!cdFile.findKeywordAndVal("startTime", startTime_ ))
		{
		  fatalErrorInFunction<<"Could not read startTime from file "<< cdPath_<<endl;
		  return false;
		}
	}
	else
	{
		timeFolder folders(rootPath_);
		if(startFrom == "firstTime")
		{
			startTime_ = folders.startTime();
		}
		else if( startFrom == "latestTime")
		{
			startTime_ = folders.endTime();
		}
		else
		{
			fatalErrorInFunction<<
			"expected firstTime, latestTime, or startTime in front of StartFrom, "<<endl<<
			"but found "<<startFrom<<endl;
			return false;
		}
	}
  

	if(!cdFile.findKeywordAndVal("endTime", endTime_ ))
	{
		fatalErrorInFunction<<
		"Could not read endTime from file "<< cdPath_<<endl;
		return false; 
	}

	if(!cdFile.findKeywordAndVal("writeInterval", saveInterval_))
	{
		fatalErrorInFunction<<
		"Could not read writeInterval from file "<< cdPath_<<endl;
		return false; 
	}

	formatType_ = cdFile.lookupDataOrSet<word>("timeFormat", "general");

	precision_ = cdFile.lookupDataOrSet("timePrecision", 6);

	return true;
}

pFlow::readControlDict::readControlDict(
	const fileSystem& rootPath, 
	const fileSystem& cdPath)
:
	rootPath_(rootPath),
	cdPath_(cdPath)
{

	if(!read())
	{
		fatalExit;
	}
}