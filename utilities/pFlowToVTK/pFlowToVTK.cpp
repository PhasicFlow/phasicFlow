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

#include "vocabs.hpp"
#include "systemControl.hpp"
#include "timeFolder.hpp"
#include "commandLine.hpp"
#include "ranges.hpp"
#include "Vectors.hpp"
#include "phasicFlowKokkos.hpp"
#include "pointFieldToVTK.hpp"
#include "triSurfaceFieldToVTK.hpp"
#include "fileSeries.hpp"
//#include "readControlDict.hpp"

bool bindaryOutput__;

int main(int argc, char** argv )
{
	pFlow::word outFolder = (pFlow::CWD()/pFlow::word("VTK")).wordPath();

	pFlow::commandLine cmds(
		"pFlowToVTK",
		"Convrtes the saved pointField and geometry"
		" date in time folders into vtk file format.");

	pFlow::wordVector times;
		 
	bool noGoem = false;
	cmds.add_flag(
		"--no-geometry",
		noGoem,
		"Do not convert geometry to VTK file");
	
	bool noParticle = false;
	cmds.add_flag("--no-particles",
		noParticle,
		"Do not convert particle fields to VTK file");

	bindaryOutput__ = false;
	cmds.add_flag("-b, --binary",
		bindaryOutput__,
		"Wrtie vtk file (for particles only) in binary format. Default is ASCII");

	cmds.addOption("-o,--out-folder",
		outFolder,
		"path to output folder of VTK",
		"path"); 
	
	bool separateSurfaces = false;
	cmds.add_flag(
		"-s,--separate-surfaces",
		separateSurfaces,
		"use this when you want to have sub-surfaces in separate files");

	pFlow::wordVector fields;
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

	//bool isCoupling = false;

	if(!cmds.parse(argc, argv)) return 0;
	
	
// this should be palced in each main 
#include "initialize_Control.hpp"


	if(!bindaryOutput__)
	{
		INFORMATION<<"Writing vtk file in binray format will accelerate the conversion (5~10x)"<<
		" and visualization in paraview."<<
		" Consider addig flag -b or --binary in the command line."<<END_INFO;
	}

	pFlow::word formatName = bindaryOutput__?"binary":"ascii";

	pFlow::timeFolder folders(Control);
	auto destFolderGeometry = pFlow::fileSystem(outFolder)/pFlow::word(pFlow::geometryFolder__);
	auto destFolderField = pFlow::fileSystem(outFolder)/pFlow::word("particles");
	pFlow::wordList geomfiles{"triSurface"};


	if(cmds.count("--fields"))
	{
		allFields = false;
	}

	pFlow::realCombinedRange validRange;
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

	{

	pFlow::PFtoVTK::fileSeries timeSeries{pFlow::fileSystem(outFolder)};
	pFlow::word fileName;
	pFlow::wordList geomFileNames;
	pFlow::wordList surfNames;
	do
	{
		Control.time().setTime(folders.time());
		if( !validRange.isMember( folders.time() ) )continue;
		
		pFlow::output<< "time: " << Cyan_Text( folders.time() )<<" s" <<pFlow::endl;
		if(!noGoem)
		{	
			
			if(!pFlow::TSFtoVTK::convertTimeFolderTriSurfaceFields(
				Control, destFolderGeometry, "surface", separateSurfaces, surfNames, geomFileNames))
			{
				fatalExit;
				return 1;
			}

			timeSeries.addTimeFile(surfNames, folders.time(), geomFileNames);
		}

		if(!noParticle)
		{
			REPORT(1)<< "Converting pointFields to vtk file in "<< formatName<< " format ..."<<END_REPORT;
			if(allFields)
			{
				if( !pFlow::PFtoVTK::convertTimeFolderPointFields(
					Control,
					destFolderField,
					"particles",
					fileName 
					)
				)
				{
					fatalExit;
				}
			}else
			{
				if(!pFlow::PFtoVTK::convertTimeFolderPointFieldsSelected(
					Control,
					destFolderField,
					"particles",
					fields,
					!pFlow::equal(folders.time(),static_cast<pFlow::real>(0.0)),
					fileName 
					)
				)
				{
					fatalExit;
				}
			}

			timeSeries.addTimeFile("particles", folders.time(), fileName);
				
		}
		
		pFlow::output<<pFlow::endl;

	}
	while( folders++ );

	}


	pFlow::output<< "\nFinished successfully.\n";

	
// this should be palced in each main 
#include "finalize.hpp"
	return 0;
}
