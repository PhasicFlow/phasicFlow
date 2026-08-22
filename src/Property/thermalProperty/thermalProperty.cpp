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

#include "thermalProperty.hpp"
#include "dictionary.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

bool thermalProperty::readDictionary()
{
    // thermoPhysicalInteraction is read-only here (never written back,
    // never registered) -- a plain dictionary is enough, matching the
    // same read-only secondary-file pattern already used in
    // thermalInteraction.cpp for this exact file. fileDictionary's
    // extra IOobject/objectFile machinery isn't needed for this.
    dictionary thermoDict(
        "thermoPhysicalInteraction",
        // Fallback when constructed without a directory: the
        // (fileName, owner) constructor has none to pass here.
        p_dir_ != nullptr ? *p_dir_ : fileSystem("caseSetup"));

    // Read thermal properties
    heatCapacities_ = 
        thermoDict.getVal<realVector>("heatCapacities");
        
    heatConductivities_ = 
        thermoDict.getVal<realVector>("heatConductivities");
        
    emissivities_ = 
        thermoDict.getVal<realVector>("emissivities");

    // ---------------------------------------------------------------------- //
    // Ambient fluid properties for the PFP model, used only when no CFD
    // mesh exists to sample fluidKappa_/fluidAlpha_ from (standalone
    // DEM-only solvers such as heatSphereGranFlow/multiSpeciesGranFlow).
    // Coupled CFD-DEM solvers always overwrite the per-particle
    // fluidKappa_/fluidAlpha_ fields from the real mesh on the very
    // first data exchange, so reading this block is harmless there too.
    //
    // The sub-dictionary itself is optional: if it is absent, both
    // values default to 0, matching the pre-existing standalone
    // behaviour where PFP silently contributes nothing. If the case
    // does add the block, both keys inside it are required -- a
    // partially-specified ambient environment (only kappa or only
    // alpha) is far more likely to be a mistake than an intentional
    // setting, so it is caught immediately rather than silently
    // defaulting the missing half.
    // ---------------------------------------------------------------------- //
    if (thermoDict.containsDictionay("fluidProperties"))
    {
        const dictionary& fluidPropDict = 
            thermoDict.subDict("fluidProperties");

        ambientFluidKappa_ = fluidPropDict.getVal<real>("kappa");
        ambientFluidAlpha_ = fluidPropDict.getVal<real>("alpha");
    }
    else
    {
        ambientFluidKappa_ = real(0);
        ambientFluidAlpha_ = real(0);
    }
    
    // Read mechanical properties from the base interaction dictionary
    realYoungsModuli_ = 
        this->getVal<realVector>("realYoungsModuli");
        
    poissonRatios_ = 
        this->getVal<realVector>("poissonRatios");
    
    // Validate sizes against the materials list
    bool isValid = 
        (materials().size() == heatCapacities_.size()) && 
        (materials().size() == heatConductivities_.size()) &&
        (materials().size() == emissivities_.size()) &&
        (materials().size() == realYoungsModuli_.size()) &&
        (materials().size() == poissonRatios_.size());

    if (!isValid)
    {
        fatalErrorInFunction 
            << "  Mismatch in the number of material properties between "
            << "'interaction' and 'thermoPhysicalInteraction' dictionaries." 
            << endl;
        fatalExit;
    }

    return isValid;
}

bool thermalProperty::writeDictionary()
{
    bool isWritten = 
        add("heatCapacities",     heatCapacities_) && 
        add("heatConductivities", heatConductivities_) &&
        add("emissivities",       emissivities_) &&
        add("realYoungsModuli",   realYoungsModuli_) &&
        add("poissonRatios",      poissonRatios_);

    if (!isWritten)
    {
        fatalErrorInFunction 
            << "  Error in writing thermal properties to dictionary " 
            << globalName() << endl;
    }

    return isWritten;
}

//----------------------------- constructors ----------------------------------

thermalProperty::thermalProperty(
    const word&         fileName, 
    repository*         owner) 
: 
    property(fileName, owner)
{ 
    if (!readDictionary()) 
    { 
        fatalExit; 
    }
}

thermalProperty::thermalProperty(
    const word&         fileName, 
    const fileSystem&   dir) 
: 
    property(fileName, dir), 
    p_dir_(&dir)
{ 
    if (!readDictionary()) 
    { 
        fatalExit; 
    }
}

thermalProperty::thermalProperty(
    const word&         fileName, 
    const wordVector&   materials, 
    const realVector&   densities,
    const realVector&   heatCapacities, 
    const realVector&   heatConductivities, 
    const realVector&   emissivities, 
    const realVector&   realYoungsModuli,
    const realVector&   poissonRatios,
    repository*         owner)
: 
    property(fileName, materials, densities, owner), 
    heatCapacities_(heatCapacities), 
    heatConductivities_(heatConductivities), 
    emissivities_(emissivities),
    realYoungsModuli_(realYoungsModuli),
    poissonRatios_(poissonRatios)
{ 
    if (!writeDictionary()) 
    { 
        fatalExit; 
    }
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow


