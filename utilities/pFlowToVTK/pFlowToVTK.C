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


#include "systemControl.H"
#include "pointFieldToVTK.H"
#include "triSurfaceFieldToVTK.H"
#include "timeFolder.H"
#include "commandLine.H"
#include "ranges.H"

using pFlow::word;
using pFlow::wordVector;
using pFlow::geometryFolder__;
using pFlow::timeFolder;
using pFlow::fileSystem;
using pFlow::wordList;
using pFlow::IOfileHeader;
using pFlow::objectFile;
using pFlow::output;
using pFlow::endl;
using pFlow::multiTriSurface;
using pFlow::commandLine;
using pFlow::realCombinedRange;

int main(int argc, char** argv )
{
	word outFolder = (pFlow::CWD()/word("VTK")).wordPath();

	commandLine cmds(
		"pFlowToVTK",
		"Convrtes the saved pointField and geometry"
		" date in time folders into vtk file format.");

	wordVector times;
		
	bool noGoem = false;
	cmds.add_flag(
		"--no-geometry",
		noGoem,
		"Do not convert geometry to VTK file");
	
	bool noParticle = false;
	cmds.add_flag("--no-particles",
		noParticle,
		"Do not convert particle fields to VTK file");

	cmds.addOption("-o,--out-folder",
		outFolder,
		"path to output folder of VTK",
		"path");

	wordVector fields;
	bool 			 allFields = true;
	cmds.addOption("-f,--fields",
		fields.vectorField(),
		"a space-separated list of fields names to be converted to VTK",
		"word");

	cmds.addOption(
		"-t,--time",
		times.vectorField(),
		"a space separated lists of time folders, or a strided range begin:stride:end, or an interval begin:end",
		" ");

	if(!cmds.parse(argc, argv)) return 0;
	
	
// this should be palced in each main 
#include "initialize_Control.H"


	timeFolder folders(Control);
	fileSystem destFolder = fileSystem(outFolder)/geometryFolder__;
	fileSystem destFolderField = fileSystem(outFolder);
	wordList geomfiles{"triSurface"};


	if(cmds.count("--fields"))
	{
		allFields = false;
	}

	realCombinedRange validRange;
	if( cmds.count("--time") )
	{
		if(!validRange.addRanges(times))
		{
			fatalExit;
		}
	}
	else
	{
		validRange.addIntervalRange(folders.startTime(), folders.endTime());
	}

	do
	{

		if( !validRange.isMember( folders.time() ) )continue;
		
		output<< "time: " << cyanText( folders.time() )<<" s" <<endl;
		if(!noGoem)
		{	
			fileSystem geomFolder = folders.folder()/geometryFolder__;
			if(!pFlow::TSFtoVTK::convertTimeFolderTriSurfaceFields(geomFolder, folders.time(), destFolder, "surface"))
			{
				fatalExit;
				return 1;
			}	
		}

		if(!noParticle)
		{
			
			if(allFields)
			{
				if( !pFlow::PFtoVTK::convertTimeFolderPointFields(
					folders.folder(),
					folders.time(),
					destFolderField,
					"sphereFields" )
				)
				{
					fatalExit;
				}
			}else
			{
				if(!pFlow::PFtoVTK::convertTimeFolderPointFieldsSelected(
					folders.folder(),
					folders.time(),
					destFolderField,
					"sphereFields",
					fields,
					!pFlow::equal(folders.time(),static_cast<pFlow::real>(0.0)) )
				)
				{
					fatalExit;
				}
			}	
		}
		
		output<<endl;

	}
	while( folders++ );


	output<< "\nFinished successfully.\n";

	
// this should be palced in each main 
#include "finalize.H"
	return 0;
}
