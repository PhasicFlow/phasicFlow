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

#ifndef pFlow_multiSpeciesProperty_hpp
#define pFlow_multiSpeciesProperty_hpp

#include "thermalProperty.hpp"

namespace pFlow
{

/**
 * @brief Species, stoichiometry and Arrhenius kinetics of the multi-species
 * (CLC) reaction model, read from the multiReactiveProperties dictionary of
 * thermoPhysicalInteraction.
 *
 * Molar masses are in kg/kmol, gas concentrations in kmol/m3 and reaction
 * enthalpies in J/kmol, matching OpenFOAM's molar convention (the rate law
 * and its units are described in multiReactionTypes.hpp). The parameters
 * are stored once here and shared by multiShrinkingInteraction and
 * multiReactiveSphereParticles.
 */
class multiSpeciesProperty
:
    public thermalProperty
{
private:

    /// Master switch of the multi-reaction model.
    Logical enableMultiReaction_;

    /// Shrinking Particle Model switch (off: constant particle diameter).
    Logical enableShrinking_;

    /// Number of solid species.
    int32 N_solid_ = 0;

    /// Number of gas species.
    int32 N_gas_ = 0;

    /// Number of reactions.
    int32 N_rxn_ = 0;

    /// Names of the solid species.
    wordVector solidSpeciesNames_;

    /// Names of the gas species (dictionary key: productGas).
    wordVector gasSpeciesNames_;

    /// Molar masses of the solid species [kg/kmol].
    realVector solidMw_;

    /// Molar masses of the gas species [kg/kmol].
    realVector gasMw_;

    /// Densities of the solid species [kg/m3].
    realVector solidDensities_;

    /**
     * Gas concentrations [kmol/m3], one per gas species. Used where no CFD
     * field exists: in the standalone solver and before the first CFD
     * exchange.
     */
    realVector defaultGasConcentrations_;

    /// Solid stoichiometry, row-major [N_rxn x N_solid].
    realVector stoicSolidFlattened_;

    /// Gas stoichiometry, row-major [N_rxn x N_gas].
    realVector stoicGasFlattened_;

    /// Pre-exponential factor of each reaction.
    realVector preExpFactor_;

    /// Activation temperature E_a/R [K] of each reaction.
    realVector activationTemperature_;

    /// Reaction order in the gas concentration, per reaction.
    realVector gasOrder_;

    /// Reaction order in the solid reactant mass fraction, per reaction.
    realVector solidOrder_;

    /// Temperature [K] below which each reaction is inactive.
    realVector onsetTemperature_;

    /// Index of the gas species driving each reaction.
    int32Vector gasSpeciesIdx_;

    /// Index of the tracked solid reactant of each reaction.
    int32Vector solidReactantIdx_;

    /// Enthalpy of reaction [J/kmol] of each reaction (negative: exothermic).
    realVector dH_rxn_;

    /// Reads and checks multiReactiveProperties of thermoPhysicalInteraction.
    void readReactiveDictionary();

public:

    TypeInfo("multiSpeciesProperty");

    explicit multiSpeciesProperty
    (
        const word& fileName,
        repository* owner = nullptr
    );

    multiSpeciesProperty
    (
        const word&       fileName,
        const fileSystem& dir
    );

    ~multiSpeciesProperty() override = default;

    inline bool enableMultiReaction() const
    {
        return static_cast<bool>(enableMultiReaction_);
    }

    inline bool enableShrinking() const
    {
        return static_cast<bool>(enableShrinking_);
    }

    inline int32 numSolid() const { return N_solid_; }

    inline int32 numGas() const { return N_gas_; }

    inline int32 numRxn() const { return N_rxn_; }

    inline const auto& solidSpeciesNames() const
    {
        return solidSpeciesNames_;
    }

    inline const auto& gasSpeciesNames() const { return gasSpeciesNames_; }

    inline const auto& solidMw() const { return solidMw_; }

    inline const auto& gasMw() const { return gasMw_; }

    inline const auto& solidDensities() const { return solidDensities_; }

    inline const auto& defaultGasConcentrations() const
    {
        return defaultGasConcentrations_;
    }

    inline const auto& stoicSolidFlattened() const
    {
        return stoicSolidFlattened_;
    }

    inline const auto& stoicGasFlattened() const
    {
        return stoicGasFlattened_;
    }

    inline const auto& preExpFactor() const { return preExpFactor_; }

    inline const auto& activationTemperature() const
    {
        return activationTemperature_;
    }

    inline const auto& gasOrder() const { return gasOrder_; }

    inline const auto& solidOrder() const { return solidOrder_; }

    inline const auto& onsetTemperature() const { return onsetTemperature_; }

    inline const auto& gasSpeciesIdx() const { return gasSpeciesIdx_; }

    inline const auto& solidReactantIdx() const { return solidReactantIdx_; }

    inline const auto& dH_rxn() const { return dH_rxn_; }
};

} // pFlow

#endif // pFlow_multiSpeciesProperty_hpp
