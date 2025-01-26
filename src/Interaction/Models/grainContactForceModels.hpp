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

#ifndef __grainContactForceModels_hpp__
#define __grainContactForceModels_hpp__

#include "cGAbsoluteLinearCF.hpp"
#include "cGRelativeLinearCF.hpp"
#include "cGNonLinearCF.hpp"
#include "cGNonLinearCF2.hpp"

#include "grainRolling.hpp"



namespace pFlow::cfModels 
{


using limitedCGAbsoluteLinearGrainRolling 	= grainRolling<cGAbsoluteLinear<true>>;
using nonLimitedCGAbsoluteLinearGrainRolling = grainRolling<cGAbsoluteLinear<false>>;

using limitedCGRelativeLinearGrainRolling   = grainRolling<cGRelativeLinear<true>>;
using nonLimitedCGRelativeLinearGrainRolling = grainRolling<cGRelativeLinear<false>>;

using limitedCGNonLinearGrainRolling   = grainRolling<cGNonLinear<true>>;
using nonLimitedCGNonLinearGrainRolling = grainRolling<cGNonLinear<false>>;

using limitedCGNonLinear2GrainRolling   = grainRolling<cGNonLinear2<true>>;
using nonLimitedCGNonLinear2GrainRolling = grainRolling<cGNonLinear2<false>>;

}



#endif //__grainContactForceModels_hpp__
