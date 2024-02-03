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
#include "fileDictionary.hpp"
#include "Wall.hpp"
#include "Vectors.hpp"
#include "VectorSingles.hpp"
#include "multiTriSurface.hpp"
//#include "geometryMotion.hpp"
#include "commandLine.hpp"
//#include "readControlDict.hpp"


using namespace pFlow;

int main( int argc, char* argv[] )
{

	commandLine cmds(
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

	//#include "setProperty.hpp"

	REPORT(0)<<"\nReading "<<"geometryDict"<<" . . ."<<END_REPORT;
	auto geometryDict = fileDictionary(
		objectFile
		(
			"geometryDict",
			Control.settings().path(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		nullptr
	);
	
	auto& surfacesDict = geometryDict.subDict("surfaces");

	auto wallsDictName = surfacesDict.dictionaryKeywords();

	

	multiTriSurface surface
	(
		objectFile
		(
			triSurfaceFile__,
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_ALWAYS
		),
		&Control.geometry()
	);
	
	//wordVector materials;
	//wordVector motion;
	
	for(auto& name:wallsDictName)
	{
		REPORT(1)<<"Creating wall "<<Green_Text(name)<<" from dictionary "<<surfacesDict.globalName() <<END_REPORT;
		auto wallPtr = Wall::create( surfacesDict.subDict(name));
		auto& wall = wallPtr();
		REPORT(1)<<"wall type is "<<Green_Text(wall.typeName())<<'\n'<<END_REPORT;		

		realx3x3Vector trinalges(wall.name(), wall.triangles());
		
		surface.appendTriSurface(wall.name(), trinalges);
		/*materials.push_back(wall.materialName());
		motion.push_back(wall.motionName());*/
	}

	output<<surface<<endl;
	//REPORT(1)<<"Selected wall materials are "<<cyanText(materials)<<'\n'<<endREPORT;
		
	/*REPORT(0)<< "\nCreating geometry . . ."<<endREPORT;
	auto geomPtr = geometry::create(Control, proprties, geometryDict, surface, motion, materials);
	REPORT(1)<< "geometry type is "<< greenText(geomPtr().typeName())<<endREPORT;
	
	REPORT(1)<< "Writing geometry to folder "<< geomPtr().path()<<endREPORT;
	geomPtr().write();*/

	REPORT(0)<< Green_Text("\nFinished successfully.\n")<<END_REPORT;

// this should be palced in each main 
#include "finalize.hpp"

	return 0;
} 
