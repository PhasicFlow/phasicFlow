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

#ifndef __builtinTypes_hpp__
#define __builtinTypes_hpp__

#include <string>

#include "phasicFlowConfig.H"

namespace pFlow
{

#ifdef pFlow_Build_Double
	#define UseDouble 1
	inline const char* floatingPointType__ = "double";
	inline const bool  usingDouble__       = true;
#else
	#define UseDouble 0
	inline const char* floatingPointType__ = "float";
	inline const bool  usingDouble__       = false;
#endif



// scalars
#if UseDouble
	using real = double;
#else
	using real = float;
#endif

using int8 = signed char;

using int32 = int;

using int64 = long long int;

using uint8 = unsigned char;

using uint32 = unsigned int;

using uint64 = unsigned long long int;

using word = std::string;

/// type for time values
using TimeValueType = double;

/// type for indexing arrays and vectors 
#ifdef pFlow_Build_Index64
	#define Index64 1
	using IndexType = std::size_t;
	inline const char* IndexType__ = "std::size_t";
#else
	#define Index64 0
	using IndexType = uint32;
	inline const char* IndexType__ = "uint32";
#endif

inline word
floatingPointDescription()
{
	return word("In this build, ") + word(floatingPointType__) +
	       word(" is used for floating point operations and ") +
		   IndexType__ + " for indexing.";
}

} // end of pFlow

#endif
