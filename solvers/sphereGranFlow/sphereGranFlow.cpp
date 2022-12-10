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

#include "property.hpp"
#include "geometry.hpp"
#include "sphereParticles.hpp"
#include "Insertions.hpp"
#include "systemControl.hpp"
#include "contactSearch.hpp"
#include "sphereInteraction.hpp"
#include "commandLine.hpp"
#include "readControlDict.hpp"

using pFlow::output;
using pFlow::endl;
using pFlow::property;
using pFlow::sphereParticles;
using pFlow::objectFile;
using pFlow::sphereInsertion;
using pFlow::insertionFile__;
using pFlow::interactionFile__;
using pFlow::contactSearch;
using pFlow::interaction;
using pFlow::commandLine;

int main( int argc, char* argv[])
{

commandLine cmds(
		"sphereGranFlow",
		"DEM solver for non-cohesive spherical particles with particle insertion "
		"mechanism and moving geometry");

bool isCoupling = false;

if(!cmds.parse(argc, argv)) return 0;

// this should be palced in each main 
#include "initialize_Control.hpp"
	
	#include "setProperty.hpp"
	#include "setSurfaceGeometry.hpp"
	

	#include "createDEMComponents.hpp"
	
	Report(0)<<"\nStart of time loop . . .\n"<<endReport;

	do 
	{
		Control.timers().start();
		
		if(! sphInsertion.insertParticles( 
			Control.time().currentTime(),
			Control.time().dt()	) )
		{
			fatalError<<
			"particle insertion failed in sphereDFlow solver.\n";
			return 1;
		}

		surfGeometry.beforeIteration();
		
		sphInteraction.beforeIteration();

		sphParticles.beforeIteration();
		
		
		sphInteraction.iterate();

		sphParticles.iterate();

		surfGeometry.iterate();

		sphParticles.afterIteration();
		
		surfGeometry.afterIteration();
		

		Control.timers().end();

	}while(Control++);

	Report(0)<<"\nEnd of time loop.\n"<<endReport;

// this should be palced in each main 
#include "finalize.hpp"


}	

