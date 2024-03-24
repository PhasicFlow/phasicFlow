
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
/** 
 * \file sphereGranFlow.cpp
 * \brief sphereGranFlow solver 
 *
 * This solver simulate granular flow of cohesion-less, spherical particles. 
 * Particle insertion can be activated in the solver to gradually insert 
 * particles into the simulation. Geometry can be defined generally with 
 * built-in features of the code or through ASCII stl files or a combination
 * of both. For more information refer to [tutorials/sphereGranFlow/]
 * (https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/sphereGranFlow) folder.
 */

#include "vocabs.hpp"
#include "phasicFlowKokkos.hpp"
#include "systemControl.hpp"
#include "commandLine.hpp"
#include "property.hpp"
#include "geometry.hpp"
#include "sphereParticles.hpp"
#include "interaction.hpp"
//#include "Insertions.hpp"



//#include "readControlDict.hpp"

using namespace pFlow;

/**
 * DEM solver for simulating granular flow of cohesion-less particles.
 *
 * In the root case directory just simply enter the following command to 
 * run the simulation. For command line options use flag -h. 
 */
int main( int argc, char* argv[])
{

commandLine cmds(
		"sphereGranFlow",
		"DEM solver for non-cohesive spherical particles with particle insertion "
		"mechanism and moving geometry");

bool isCoupling = false;

if(!cmds.parse(argc, argv)) return 0;

// this should be palced in each main 
processors::initProcessors(argc, argv);
initialize_pFlowProcessors();
#include "initialize_Control.hpp"
	 
	#include "setProperty.hpp"
	#include "setSurfaceGeometry.hpp"
	

	#include "createDEMComponents.hpp"
	
	REPORT(0)<<"\nStart of time loop . . .\n"<<END_REPORT;

	do 
	{
		
		/*if(! sphInsertion.insertParticles( 
			Control.time().currentTime(),
			Control.time().dt()	) )
		{
			fatalError<<
			"particle insertion failed in sphereDFlow solver.\n";
			return 1;
		}*/
		
		// set force to zero
		surfGeometry.beforeIteration();

		// set force to zero, predict, particle deletion and etc. 
		sphParticles.beforeIteration();

		sphInteraction.beforeIteration();
		
		sphInteraction.iterate();	
		
		surfGeometry.iterate();

		sphParticles.iterate();

		sphInteraction.afterIteration();

		surfGeometry.afterIteration();

		sphParticles.afterIteration();
				

	}while(Control++);

	REPORT(0)<<"\nEnd of time loop.\n"<<END_REPORT;

// this should be palced in each main 
#include "finalize.hpp"
processors::finalizeProcessors();


}	

