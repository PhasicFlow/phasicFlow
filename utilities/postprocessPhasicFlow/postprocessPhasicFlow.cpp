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
#include "Vectors.hpp"
#include "commandLine.hpp"
#include "readControlDict.hpp"

#include "postprocessData.hpp"
#include "postprocessGlobals.hpp"


int main(int argc, char** argv )
{ 

	pFlow::word outFolder = (pFlow::CWD()/pFlow::postprocessData::defaultRelDir__).wordPath();

	pFlow::commandLine cmds(
		"postprocessPhasicFlow",
		"post-process fields in time folders based on the input file "
		"settings/postprocessDataDict.");

	pFlow::wordVector times;
	
	pFlow::word description = "path to output folder of postprocess data: ./"
		+ pFlow::postprocessData::defaultRelDir__;

	cmds.addOption("-o,--out-folder",
		outFolder,
		description,
		"path");

	cmds.addOption(
		"-t,--time",
		times.vectorField(),
		"a SPACE separated list of time folders, "
		"or a strided range <begin>:<stride>:<end>, or an interval <begin>:<end>",
		" ");

	bool withZeroFolder = false;
	cmds.add_flag(
		"-z, --zeroFolder",
		withZeroFolder,
		"include zero folder into processing time folders");

	bool isCoupling = false;

	if(!cmds.parse(argc, argv)) return 0;

	#include "initialize_Control.hpp"
		
	// time in command line 
	pFlow::combinedRange<pFlow::TimeValueType> validRange;

	if( cmds.count("--time") )
	{
		if(!validRange.addRanges(times))
		{
			fatalExit; 
		}
		if(withZeroFolder)
			validRange.addIndividual(0.0);
	}
	else
	{
		if(withZeroFolder)
			validRange.addIntervalRange(0.0, 1.0e+15);
		else
			validRange.addIntervalRange(1.0e-7, 1.0e+15);
	}

	pFlow::TimeValueType nextTime = validRange.minVal();

	if(nextTime <0.0)
	{
		fatalError
			<<"Invalid time range in the command line. "
			<<"your input range is: "
			<<times
			<<" which resulted to start time "
			<< nextTime<<pFlow::endl;
		fatalExit;
	}

	pFlow::postprocessData::postprocessData postprocess(Control, nextTime);
	postprocess.setOutputDirectory(pFlow::fileSystem(outFolder+"/").absolute());
	
	bool folderSkipped = false;
	pFlow::output<<"\n"<<pFlow::endl;
	while(nextTime>=0.0)
	{

		if( !folderSkipped )
		{
			if(!postprocess.execute())
			{
				fatalError
					<<"Error occured in executing postprocessing...."<<pFlow::endl;
				fatalExit;
			}

			if(!postprocess.write())
			{
				fatalError
					<<"Error occured in writing postprocess results..."<<pFlow::endl;
				fatalExit;
			}

			pFlow::output<<"\n"<<pFlow::endl;
		}

		nextTime = postprocess.database().getNextTimeFolder();

		if(nextTime <0.0) break;

		if(validRange.isMember(nextTime))
		{
			postprocess.database().setToNextTimeFolder();
			folderSkipped = false;
		}
		else
		{
			postprocess.database().skipNextTimeFolder();
			folderSkipped = true;
		}
	
	}

	#include "finalize.hpp"

	return true;
}