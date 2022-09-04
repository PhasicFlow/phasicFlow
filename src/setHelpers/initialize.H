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

#ifndef __initialize_H__
#define __initialize_H__

// initilized and finalize should be placed in onc scope 
Report(0)<<"Initializing host/device execution spaces . . . \n";
Report(1)<<"Host execution space is "<< greenText(pFlow::DefaultHostExecutionSpace::name())<<endReport;
Report(1)<<"Device execution space is "<<greenText(pFlow::DefaultExecutionSpace::name())<<endReport;

Kokkos::initialize( argc, argv );
{


#endif
