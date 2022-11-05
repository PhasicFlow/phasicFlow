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

#ifndef __contactForceModels_H__
#define __contactForceModels_H__


#include "linearCF.H"
#include "nonLinearCF.H"
#include "normalRolling.H"
#include "nonLinearMod.H"


namespace pFlow::cfModels 
{


using limitedLinearNormalRolling 	= normalRolling<linear<true>>;
using nonLimitedLinearNormalRolling = normalRolling<linear<false>>;

using limitedNonLinearNormalRolling 	= normalRolling<nonLinear<true>>;
using nonLimitedNonLinearNormalRolling 	= normalRolling<nonLinear<false>>;

using limitedNonLinearModNormalRolling   = normalRolling<nonLinearMod<true>>;
using nonLimitedNonLinearModNormalRolling  = normalRolling<nonLinearMod<false>>;

}



#endif //__contactForceModels_H__