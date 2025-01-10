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

#include "KokkosTypes.hpp"
#include "systemControl.hpp"
#include "localProcessors.hpp"
#include "commandLine.hpp"


using pFlow::output;
using pFlow::endl;
using pFlow::commandLine;

int main( int argc, char* argv[] )
{

	commandLine cmds(
		"checkPhasicFlow",
		"A utility to check software version, host and device environments and whether it is linked");

if(!cmds.parse(argc, argv)) return 0;

// this should be palced in each main 
#include "initialize.hpp"

	output<<endl;
	REPORT(1)<< "You are using "<<Yellow_Text(cmds.productNameCopyright())<<END_REPORT;
	REPORT(1)<< Yellow_Text(pFlow::floatingPointDescription())<<END_REPORT;
    REPORT(1)<< (pFlow::localProcessors::builtForMPI()?
        "This is a built for MPI execution":
        "This is not a build for MPI execution")<<END_REPORT;

// this should be palced in each main 
#include "finalize.hpp"

	return 0;
} 
