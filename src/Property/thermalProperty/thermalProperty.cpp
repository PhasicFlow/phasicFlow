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
#include "fileDictionary.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

// ========================================================================= //
// Section 1: Dictionary I/O
// ========================================================================= //

bool thermalProperty::readDictionary()
{
    uniquePtr<fileDictionary> thermoDictPtr = nullptr;

    // Dynamic path resolution
    if (p_dir_ != nullptr)
    {
        thermoDictPtr = makeUnique<fileDictionary>(
            "thermoPhysicalInteraction", 
            *p_dir_);
    }
    else
    {
        // Safe fallback for legacy code calling the default constructor
        thermoDictPtr = makeUnique<fileDictionary>(
            "thermoPhysicalInteraction", 
            fileSystem("caseSetup"));
    }

    auto& thermoDict = thermoDictPtr();

    // Read thermal properties
    heatCapacities_ = 
        thermoDict.getVal<realVector>("heatCapacities");
        
    heatConductivities_ = 
        thermoDict.getVal<realVector>("heatConductivities");
        
    emissivities_ = 
        thermoDict.getVal<realVector>("emissivities");
    
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

// ========================================================================= //
// Section 2: Constructors
// ========================================================================= //

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




