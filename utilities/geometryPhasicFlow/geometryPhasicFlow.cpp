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
#include "Wall.hpp"
#include "geometry.hpp"
#include "commandLine.hpp"
//#include "readControlDict.hpp"


int main( int argc, char* argv[] )
{

	pFlow::commandLine cmds(
    	"geometryPhasicFlow",
    	"Converts the supplied informaiton for sufraces in"
    	" geometryDict into PhasicFlow geometry data structure");


  bool isCoupling = false;
  cmds.add_flag(
    "-c,--coupling",
    isCoupling,
    "Is this a fluid-particle coupling simulation?");

  if(!cmds.parse(argc, argv)) return 0;

// this should be palced in each main 
#include "initialize_Control.hpp"

	#include "setProperty.hpp"

	REPORT(0)<<"\nReading "<<"geometryDict"<<" . . ."<<END_REPORT;
	auto geometryDict = pFlow::fileDictionary(
		pFlow::objectFile
		(
			"geometryDict",
			Control.settings().path(),
			pFlow::objectFile::READ_ALWAYS,
			pFlow::objectFile::WRITE_NEVER
		),
		nullptr
	);
	
	auto& surfacesDict = geometryDict.subDict("surfaces");

	auto wallsDictName = surfacesDict.dictionaryKeywords();

	
    auto mSurfaceName = pFlow::word("geometryPhasicFlow_")+pFlow::word(pFlow::triSurfaceFile__);
	pFlow::multiTriSurface surface
	(
		pFlow::objectFile
		(
			mSurfaceName,
			"",
			pFlow::objectFile::READ_NEVER,
			pFlow::objectFile::WRITE_ALWAYS
		),
		nullptr
	);
	
	pFlow::wordVector materials;
	pFlow::wordList materialsList;

	pFlow::wordVector motion;
	pFlow::wordList motionList;

	for(auto& name:wallsDictName)
	{
		REPORT(1)<<"Creating wall "<<Green_Text(name)<<" from dictionary "<<surfacesDict.globalName() <<END_REPORT;
		auto wallPtr = pFlow::Wall::create( surfacesDict.subDict(name));
		auto& wall = wallPtr();
		REPORT(1)<<"wall type is "<<Green_Text(wall.typeName())<<'\n'<<END_REPORT;		

		pFlow::realx3x3Vector trinalges(wall.name(), wall.triangles());
		
		surface.appendSurface(wall.name(), trinalges);
		materials.push_back(wall.materialName());
		materialsList.push_back(wall.materialName());

		motion.push_back(wall.motionName());
		motionList.push_back(wall.motionName());
	}

	
	REPORT(1)<<"Selected wall materials are "<<Cyan_Text(materialsList)<<'\n'<<END_REPORT;
	REPORT(1)<<"Selected wall motion components are "<<Cyan_Text(motionList)<<'\n'<<END_REPORT;
		
	REPORT(0)<< "\nCreating geometry . . ."<<END_REPORT;
	auto geomPtr = pFlow::geometry::create(
		Control, 
		proprties, 
		surface, 
		motion, 
		materials, 
		geometryDict);
	
	REPORT(1)<< "geometry type is "<< Green_Text(geomPtr().typeName())<<END_REPORT;

	Control.time().write(true);

	REPORT(0)<< Green_Text("\nFinished successfully.\n")<<END_REPORT;

// this should be palced in each main 
#include "finalize.hpp"

	return 0;
} 
