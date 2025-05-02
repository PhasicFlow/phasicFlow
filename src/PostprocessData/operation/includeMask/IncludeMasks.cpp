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

#include "IncludeMask.hpp"

namespace pFlow::postprocessData
{

// real
template class IncludeMask<real, compareOne<real, lessThanOp> >;
template class IncludeMask<real, compareOne<real, lessThanEqOp> >;

template class IncludeMask<real, compareOne<real, greaterThanOp> >;
template class IncludeMask<real, compareOne<real, greaterThanEqOp> >;

template class IncludeMask<real, compareOne<real, equalOp> >;

template class IncludeMask<real, compareTwo<real, betweenOp> >;
template class IncludeMask<real, compareTwo<real, betweenEqOp> >;

template class IncludeMask<real, allOp<real>>;

// realx3
template class IncludeMask<realx3, compareOne<realx3, lessThanOp> >;
template class IncludeMask<realx3, compareOne<realx3, lessThanEqOp> >;

template class IncludeMask<realx3, compareOne<realx3, greaterThanOp> >;
template class IncludeMask<realx3, compareOne<realx3, greaterThanEqOp> >;

template class IncludeMask<realx3, compareOne<realx3, equalOp> >;

template class IncludeMask<realx3, compareTwo<realx3, betweenOp> >;
template class IncludeMask<realx3, compareTwo<realx3, betweenEqOp> >;


} // postprocessData
