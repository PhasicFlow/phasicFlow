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
#define useDouble 1
inline const char* floatingPointType__ = "double";
inline const bool  usingDouble__       = true;
#else
#define useDouble 0
inline const char* floatingPointType__ = "float";
inline const bool  usingDouble__       = false;
#endif

// scalars
#if useDouble
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

using id_t = uint32;

using size_t = std::size_t;

using word = std::string;

using timeValue = double;

inline const int numBytesForReal__ = sizeof(real);

inline word
floatingPointDescription()
{
	return word("In this build, ") + word(floatingPointType__) +
	       word(" is used for floating point operations.");
}

} // end of pFlow

#endif
