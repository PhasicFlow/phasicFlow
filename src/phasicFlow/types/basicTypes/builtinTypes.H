/*------------------------------- phasicFlow --------------------------------
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

#ifndef __builtinTypes_H__
#define __builtinTypes_H__

#include <string>


namespace pFlow
{

#define useDouble 1

// scalars
#if useDouble
	using real 	= double;
#else
	using real 	= float;
#endif

using int8 		= 	signed char;

using int16 	=	short int;

using int32 	=	int;

using int64 	= 	long long int;

using uint32 	= 	unsigned int;

using label    	=  	std::size_t;

using word 		= 	std:string;


} // end of pFlow


#endif
