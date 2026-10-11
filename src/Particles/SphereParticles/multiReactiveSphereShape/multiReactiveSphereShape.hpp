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

#ifndef pFlow_multiReactiveSphereShape_hpp
#define pFlow_multiReactiveSphereShape_hpp

#include "thermalSphereShape.hpp"
#include "multiSpeciesProperty.hpp"

namespace pFlow
{

/**
 * @brief Thermal sphere shape that also references the multi-species
 * property database (species data and kinetics of the reactive particles).
 */
class multiReactiveSphereShape
:
    public thermalSphereShape
{
private:

    /// Multi-species property database.
    const multiSpeciesProperty* multiProp_ = nullptr;

    /// Default gas concentrations [kmol/m3], one per gas species.
    realVector defaultGasConcentrations_;

public:

    TypeInfo("multiReactiveSphereShape");

    multiReactiveSphereShape
    (
        const word&                  name,
        repository*                  owner,
        const multiSpeciesProperty&  prop
    );

    multiReactiveSphereShape
    (
        const word&                  name,
        const word&                  material,
        repository*                  owner,
        const multiSpeciesProperty&  prop
    );

    ~multiReactiveSphereShape() override = default;

    /// Multi-species property database.
    inline const multiSpeciesProperty& multiProperties() const
    {
        return *multiProp_;
    }

    /// Default gas concentrations [kmol/m3], one per gas species.
    inline const realVector& defaultGasConcentrations() const
    {
        return defaultGasConcentrations_;
    }
};

} // pFlow

#endif // pFlow_multiReactiveSphereShape_hpp
