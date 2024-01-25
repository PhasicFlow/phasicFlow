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

#ifndef __vocabs_hpp__
#define __vocabs_hpp__


namespace pFlow
{

	// folders / repositories
const inline char* settingsFolder__ 		= "settings";
const inline char* settingsRepository__		= "settings";
const inline char* caseSetupFolder__ 		= "caseSetup";
const inline char* caseSetupRepository__	= "caseSetup";
const inline char* geometryFolder__ 		= "geometry";
const inline char* geometryRepository_      = "geometry";
const inline char* integrationRepository__  = "integration";
const inline char* integrationFolder__  	= "integration";

// file names
const inline char* settingsFile__			= "settingsDict";
const inline char* domainFile__     		= "domainDict";
const inline char* insertionFile__ 			= "particleInsertion";
const inline char* shapeFile__          	= "shapes";
const inline char* pointStructureFile__ 	= "pStructure";
const inline char* triSurfaceFile__ 		= "triSurface";
const inline char* createParticles__		= "createParticles";
const inline char* motionModelFile__        = "motionModel";
const inline char* contactSearchFile__		= "contactSearch";
const inline char* propertyFile__ 			= "interaction";
const inline char* interactionFile__		= "interaction";
const inline char* postprocessFile__    = "postprocessDict";


const inline char* uniform__ 		= "uniform";
const inline char* nonUniform__		= "nonUniform";


}


#endif // __vocabs_hpp__
