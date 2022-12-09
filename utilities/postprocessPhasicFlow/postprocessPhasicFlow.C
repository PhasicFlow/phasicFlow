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

#include "KokkosUtilities.H"
#include "systemControl.H"
#include "timeFolder.H"
#include "commandLine.H"
#include "ranges.H"
#include "readControlDict.H"

#include "postprocess.H"

using pFlow::word;
using pFlow::wordVector;
using pFlow::wordList;
using pFlow::commandLine;
using pFlow::timeFolder;
using pFlow::output;
using pFlow::endl;




int main(int argc, char** argv )
{ 

	word outFolder = (pFlow::CWD()/word("VTK/postprocess")).wordPath();

	commandLine cmds(
		"postprocessPhasicFlow",
		"post-process fields in time folders based on the input file "
		"settings/postprocessDict and convetes the results into vtk file format.");

	wordVector times;
		
	cmds.addOption("-o,--out-folder",
		outFolder,
		"path to output folder of VTK/postprocess",
		"path");

	cmds.addOption(
		"-t,--time",
		times.vectorField(),
		"a space separated lits of time folders, or a strided range begin:stride:end, or an interval begin:end",
		" ");

	bool withZeroFolder = false;
	cmds.addOption(
		"-z, --zeroFolder",
		withZeroFolder,
		"Do NOT exclude zero folder from processing time folders");

	bool isCoupling = false;

	if(!cmds.parse(argc, argv)) return 0;

	#include "initialize_Control.H"

	
	pFlow::postprocess post(Control);
		
	// time folders in case 
	timeFolder folders(Control);

	// time in command line 
	pFlow::realCombinedRange validRange;
	if( cmds.count("--time") )
	{
		if(!validRange.addRanges(times)){
			fatalExit; }
	}
	else
	{
		validRange.addIntervalRange(folders.startTime(), folders.endTime());
	}

	pFlow::fileSystem destFolder = pFlow::fileSystem(outFolder);

	do 
	{


		if( !validRange.isMember( folders.time() ) )continue;

		if( !withZeroFolder && pFlow::equal(folders.time() , 0.0))continue;

		post.processTimeFolder(folders);

		if(!post.writeToVTK(destFolder, "processed"))
		{
			fatalExit;
		}	
		

	}while (folders++);

	#include "finalize.H"

	return true;
}