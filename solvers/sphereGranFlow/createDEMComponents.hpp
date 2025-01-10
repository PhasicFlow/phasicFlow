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

//
REPORT(0)<<"\nReading sphere particles . . ."<<END_REPORT;
pFlow::sphereParticles sphParticles(Control, proprties);

//
REPORT(0)<<"\nCreating particle insertion object . . ."<<END_REPORT;
/*auto& sphInsertion = 
	Control.caseSetup().emplaceObject<sphereInsertion>(
		objectFile(
			insertionFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
			),
		sphParticles,
		sphParticles.shapes()
	);*/

auto sphInsertion =  pFlow::sphereInsertion(
	sphParticles, 
	sphParticles.spheres());

REPORT(0)<<"\nCreating interaction model for sphere-sphere contact and sphere-wall contact . . ."<<END_REPORT;
auto interactionPtr = pFlow::interaction::create(
	Control,
	sphParticles,
	surfGeometry
	);

auto& sphInteraction = interactionPtr();
