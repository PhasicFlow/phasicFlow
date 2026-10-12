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

#include "multiReactiveSphereShape.hpp"

namespace pFlow
{

multiReactiveSphereShape::multiReactiveSphereShape
(
    const word&                  fileName,
    repository*                  owner,
    const multiSpeciesProperty&  prop
)
:
    thermalSphereShape(fileName, owner, prop),
    multiProp_(&prop),
    defaultGasConcentrations_(prop.defaultGasConcentrations())
{
}

multiReactiveSphereShape::multiReactiveSphereShape
(
    const word&                  shapeType,
    const word&                  fileName,
    repository*                  owner,
    const multiSpeciesProperty&  prop
)
:
    thermalSphereShape(shapeType, fileName, owner, prop),
    multiProp_(&prop),
    defaultGasConcentrations_(prop.defaultGasConcentrations())
{
}

} // pFlow
