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
#include "Wall.H"
#include "multiTriSurface.H"
#include "geometryMotion.H"

using pFlow::output;
using pFlow::endl;
using pFlow::IOobject;
using pFlow::dictionary;
using pFlow::objectFile;
using pFlow::wordVector;
using pFlow::Wall;
using pFlow::geometry;
using pFlow::multiTriSurface;

int main( int argc, char* argv[] )
{

// this should be palced in each main 
#include "initialize_Control.H"

	#include "setProperty.H"

	Report(0)<<"\nReading "<<"createGeometryDict"<<" . . ."<<endReport;
	auto objDict = IOobject::make<dictionary>
	(
		objectFile
		(
			"geometryDict",
			Control.settings().path(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		"geometryDict",
		true
	);

	auto& geometryDict = objDict().getObject<dictionary>();

	auto& surfacesDict = geometryDict.subDict("surfaces");

	auto wallsDictName = surfacesDict.dictionaryKeywords();

	

	multiTriSurface surface;
	wordVector materials;
	wordVector motion;
	
	for(auto& name:wallsDictName)
	{
		Report(1)<<"Creating wall "<<greenText(name)<<" from file dictionary . . . "<<endReport;
		auto wallPtr = Wall::create( surfacesDict.subDict(name));
		auto& wall = wallPtr();
		Report(1)<<"wall type is "<<greenText(wall.typeName())<<'\n'<<endReport;		

		surface.addTriSurface(wall.name(), wall.triangles());
		materials.push_back(wall.materialName());
		motion.push_back(wall.motionName());
	}

	Report(1)<<"Selected wall materials are "<<cyanText(materials)<<'\n'<<endReport;
		
	Report(0)<< "\nCreating geometry . . ."<<endReport;
	auto geomPtr = geometry::create(Control, proprties, geometryDict, surface, motion, materials);
	Report(1)<< "geometry type is "<< greenText(geomPtr().typeName())<<endReport;
	
	Report(1)<< "Writing geometry to folder "<< geomPtr().path()<<endl;
	geomPtr().write();

	Report(0)<< greenText("\nFinished successfully.\n");

// this should be palced in each main 
#include "finalize.H"

	return 0;
} 
