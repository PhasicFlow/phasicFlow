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

bool thermalSphereShape::readThermalProperties()
{
    // Retrieve the array that links each shape index to a specific material ID
    auto pids = shapePropertyIds();

    // Allocate memory for shape-specific thermal property vectors
    cp_         = realVector("Cp",               numShapes());
    k_          = realVector("k",                numShapes());
    emissivity_ = realVector("emissivity",       numShapes());
    E0_         = realVector("realYoungsModuli", numShapes());
    nu_         = realVector("poissonRatios",    numShapes());

    // ---------------------------------------------------------------------- //
    // Direct access to the already-constructed base property object, which
    // holds all dictionary values parsed from the case file. No separate
    // fileDictionary instantiation or hardcoded path is needed here.
    // ---------------------------------------------------------------------- //
    const thermalProperty* tProps = 
        dynamic_cast<const thermalProperty*>(&properties());
        
    if (!tProps)
    {
        fatalErrorInFunction 
            << "Provided property object is not a thermalProperty!" 
            << endl;
        fatalExit;
    }

    const realVector& allCp  = tProps->heatCapacities();
    const realVector& allK   = tProps->heatConductivities();
    const realVector& allEps = tProps->emissivities();
    const realVector& allE0  = tProps->realYoungsModuli();
    const realVector& allNu  = tProps->poissonRatios();

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
    // Initial temperature assigned to newly inserted particles.
    //
    // A silently-defaulted value here (e.g. a stale 300 K) would mean any
    // particle inserted through dynamic insertion (rather than read from
    // an initial positions file) enters the domain at an unintended
    // temperature with no warning at all, so this must be supplied
    // explicitly rather than falling back to a built-in default.
    // ---------------------------------------------------------------------- //
    if (!properties().containsDataEntry("insertionTemperature"))
    {
        fatalErrorInFunction
            << "Missing MANDATORY entry 'insertionTemperature' in the "
            << "interaction dictionary." << endl;
        fatalExit;
    }
    
    insertionTemperature_ = properties().getVal<real>("insertionTemperature");

    return true;
}

//---------------------------- protected methods ------------------------------

bool thermalSphereShape::writeToDict(dictionary& dict) const
{
    bool isWritten = sphereShape::writeToDict(dict)
        && dict.add("heatCapacities",     cp_)
        && dict.add("heatConductivities", k_)
        && dict.add("emissivities",       emissivity_)
        && dict.add("realYoungsModuli",   E0_)
        && dict.add("poissonRatios",      nu_);

    return isWritten;
}

//----------------------------- constructors ----------------------------------

// ========================================================================= //
// Constructors
// ========================================================================= //

thermalSphereShape::thermalSphereShape(
    const word&     fileName,
    repository*     owner,
    const property& prop)
:
    sphereShape(fileName, owner, prop)
{
    readThermalProperties();
}

thermalSphereShape::thermalSphereShape(
    const word&     shapeType,
    const word&     fileName,
    repository*     owner,
    const property& prop)
:
    thermalSphereShape(fileName, owner, prop)
{
    // Body intentionally empty — delegates to primary constructor
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow



