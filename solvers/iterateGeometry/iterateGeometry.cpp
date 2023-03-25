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
 * \file iterateGeometry.cpp
 * \brief iterateGeometry solver 
 *
 * This solver only requires the information for geometry. It is used to test 
 * the settings for wall definition and motion model to check if the walls move 
 * as expected or not.
 * For more information refer to [tutorials/iterateGeometry/]
 * (https://github.com/PhasicFlow/phasicFlow/tree/main/tutorials/iterateGeometry)
 * folder.
 */
 
#include "systemControl.hpp"
#include "geometryMotion.hpp"
#include "commandLine.hpp"
#include "readControlDict.hpp"

using pFlow::commandLine;

int main( int argc, char* argv[] )
{

commandLine cmds(
    "iterateGeometry",
    "Performs simulation without particles, only geometry is solved");


  bool isCoupling = false;
  cmds.add_flag(
    "-c,--coupling",
    isCoupling,
    "Is this a fluid-particle coupling simulation?");

  if(!cmds.parse(argc, argv)) return 0;
   

// this should be palced in each main 
#include "initialize_Control.hpp"
	
	#include "setProperty.hpp"
	#include "setSurfaceGeometry.hpp"

	
	do
	{
		surfGeometry.iterate();

	}while( Control++);

// this should be palced in each main 
#include "finalize.hpp"

}

