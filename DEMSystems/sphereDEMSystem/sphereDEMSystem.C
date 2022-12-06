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

#include "sphereDEMSystem.H"


pFlow::sphereDEMSystem::sphereDEMSystem(int argc, char* argv[])
:
	ControlDict_()
{
  
	Report(0)<<"Initializing host/device execution spaces . . . \n";
	Report(1)<<"Host execution space is "<< greenText(DefaultHostExecutionSpace::name())<<endReport;
	Report(1)<<"Device execution space is "<<greenText(DefaultExecutionSpace::name())<<endReport;

 	// initialize Kokkos
	Kokkos::initialize( argc, argv );  
  
	Report(0)<<"\nCreating Control repository . . ."<<endReport;
	Control_ = makeUnique<systemControl>(
		ControlDict_.startTime(),
		ControlDict_.endTime(),
		ControlDict_.saveInterval(),
		ControlDict_.startTimeName());

	Report(0)<<"\nReading proprties . . . "<<endReport;
	property_ = makeUnique<property>(
    	Control().caseSetup().path()+propertyFile__);

	Report(0)<< "\nCreating surface geometry for sphereDEMSystem . . . "<<endReport;
	geometry_ = geometry::create(Control(), Property());

	Report(0)<<"\nReading sphere particles . . ."<<endReport;
	particles_ = makeUnique<sphereParticles>(Control(), Property());


	//Report(0)<<"\nCreating particle insertion for spheres. . ."<<endReport;
	/*insertion_ = 
	Control().caseSetup().emplaceObject<sphereInsertion>(
		objectFile(
			insertionFile__,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_ALWAYS
			),
		sphParticles,
		sphParticles.shapes()
	);*/

	Report(0)<<"\nCreating interaction model for sphere-sphere contact and sphere-wall contact . . ."<<endReport;
	interaction_ = interaction::create(
		Control(),
		Particles(),
		Geometry());

}

pFlow::sphereDEMSystem::~sphereDEMSystem()
{
	interaction_.reset();
	insertion_.reset();
	particles_.reset();
	geometry_.reset();
	property_.reset();
	Control_.reset();

	output<< "\nFinalizing host/device execution space ...."<<endl;
	Kokkos::finalize();
}

