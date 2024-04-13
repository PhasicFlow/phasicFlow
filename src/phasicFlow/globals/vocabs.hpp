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
const inline char* const settingsFolder__        = "settings";
const inline char* const settingsRepository__    = "settings";
const inline char* const caseSetupFolder__       = "caseSetup";
const inline char* const caseSetupRepository__   = "caseSetup";
const inline char* const geometryFolder__        = "geometry";
const inline char* const geometryRepository_     = "geometry";
const inline char* const integrationRepository__ = "integration";
const inline char* const integrationFolder__     = "integration";

// file names
const inline char* const settingsFile__       = "settingsDict";
const inline char* const domainFile__         = "domainDict";
const inline char* const insertionFile__      = "particleInsertion";
const inline char* const shapeFile__          = "shapes";
const inline char* const pointStructureFile__ = "pStructure";
const inline char* const triSurfaceFile__     = "triSurface";
const inline char* const createParticles__    = "createParticles";
const inline char* const motionModelFile__    = "motionModel";
const inline char* const contactSearchFile__  = "contactSearch";
const inline char* const propertyFile__       = "interaction";
const inline char* const interactionFile__    = "interaction";
const inline char* const postprocessFile__    = "postprocessDict";

const inline char* const uniform__    = "uniform";
const inline char* const nonUniform__ = "nonUniform";

}

#endif // __vocabs_hpp__
