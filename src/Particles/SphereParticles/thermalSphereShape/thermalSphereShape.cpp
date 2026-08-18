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

#include "thermalSphereShape.hpp"
#include "thermalProperty.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

bool thermalSphereShape::readThermalProperties(const thermalProperty& prop)
{
    // Retrieve the array that links each shape index to a specific material ID
    auto pids = shapePropertyIds();

    // Allocate memory for shape-specific thermal property vectors
    cp_         = realVector("Cp",               numShapes());
    k_          = realVector("k",                numShapes());
    emissivity_ = realVector("emissivity",       numShapes());
    E0_         = realVector("realYoungsModuli", numShapes());
    nu_         = realVector("poissonRatios",    numShapes());

    const realVector& allCp  = prop.heatCapacities();
    const realVector& allK   = prop.heatConductivities();
    const realVector& allEps = prop.emissivities();
    const realVector& allE0  = prop.realYoungsModuli();
    const realVector& allNu  = prop.poissonRatios();

    // Map the global material properties to the specific local shapes
    for (uint32 i = 0; i < numShapes(); ++i)
    {
        cp_[i]         = allCp [pids[i]];
        k_[i]          = allK  [pids[i]];
        emissivity_[i] = allEps[pids[i]];
        E0_[i]         = allE0 [pids[i]];
        nu_[i]         = allNu [pids[i]];
    }

    // ---------------------------------------------------------------------- //
    // Ambient fluid properties for the standalone-mode PFP fallback.
    // Optional at the dictionary level (see thermalProperty); both
    // default to 0 there when the case does not configure them.
    // ---------------------------------------------------------------------- //
    ambientFluidKappa_ = prop.ambientFluidKappa();
    ambientFluidAlpha_ = prop.ambientFluidAlpha();

    return true;
}

//----------------------------- constructors ----------------------------------

thermalSphereShape::thermalSphereShape(
    const word&             fileName,
    repository*             owner,
    const thermalProperty&  prop)
:
    sphereShape(fileName, owner, prop)
{
    readThermalProperties(prop);
}

thermalSphereShape::thermalSphereShape(
    const word&             shapeType,
    const word&             fileName,
    repository*             owner,
    const thermalProperty&  prop)
:
    thermalSphereShape(fileName, owner, prop)
{
    // Body intentionally empty — delegates to primary constructor
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow



