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

#ifndef __initialize_hpp__
#define __initialize_hpp__

// initilized and finalize should be placed in onc scope 
REPORT(0)<<"Initializing host/device execution spaces . . . \n";
REPORT(1)<<"Host execution space is "<< Green_Text(pFlow::DefaultHostExecutionSpace::name())<<END_REPORT;
REPORT(1)<<"Device execution space is "<<Green_Text(pFlow::DefaultExecutionSpace::name())<<END_REPORT;

Kokkos::initialize( argc, argv );
{


#endif
