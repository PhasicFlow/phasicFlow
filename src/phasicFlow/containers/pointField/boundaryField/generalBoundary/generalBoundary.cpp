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

#include "generalBoundary.hpp"
#include "pointStructure.hpp"

pFlow::generalBoundary::generalBoundary
(
	const boundaryBase& boundary,
	const pointStructure& pStruct,
	const word&  dataType,
	const word&  option
)
:
	observer(&boundary, defaultMessage_),
	boundary_(boundary),
	pStruct_(pStruct)
{}


pFlow::Time const& pFlow::generalBoundary::time() const
{
    return pStruct_.time();
}