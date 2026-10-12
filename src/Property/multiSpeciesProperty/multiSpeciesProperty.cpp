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

#include "multiSpeciesProperty.hpp"
#include "dictionary.hpp"
#include <cmath>

namespace pFlow
{

//----------------------------- private methods -------------------------------

void multiSpeciesProperty::readReactiveDictionary()
{
    // Read-only lookup of thermoPhysicalInteraction next to the property
    // file (p_dir_), with caseSetup as the fallback, as in thermalProperty.
    dictionary thermoDict
    (
        "thermoPhysicalInteraction",
        (p_dir_ != nullptr ? *p_dir_ : fileSystem("caseSetup"))
            + "thermoPhysicalInteraction"
    );

    enableMultiReaction_ = thermoDict.getVal<Logical>("enableMultiReaction");

    if (!enableMultiReaction_)
    {
        REPORT(0) << Yellow_Text("  -> Multi-Reaction framework is disabled.")
                  << END_REPORT;
        return;
    }

    REPORT(0) << "\nInitializing Multi-Reaction CLC Physics . . ."
              << END_REPORT;

    if (!thermoDict.containsDictionay("multiReactiveProperties"))
    {
        fatalErrorInFunction
            << "Missing 'multiReactiveProperties' sub-dictionary in "
            << "'thermoPhysicalInteraction' file." << endl;
        fatalExit;
    }

    const dictionary& multiDict =
        thermoDict.subDict("multiReactiveProperties");

    solidSpeciesNames_ = multiDict.getVal<wordVector>("solidSpecies");
    gasSpeciesNames_   = multiDict.getVal<wordVector>("productGas");
    solidMw_           = multiDict.getVal<realVector>("solidMolarMasses");
    gasMw_             = multiDict.getVal<realVector>("gasMolarMasses");
    solidDensities_    = multiDict.getVal<realVector>("solidDensities");

    defaultGasConcentrations_ =
        multiDict.getVal<realVector>("defaultGasConcentrations");

    enableShrinking_ = multiDict.getVal<Logical>("enableShrinking");

    N_solid_ = static_cast<int32>(solidSpeciesNames_.size());
    N_gas_   = static_cast<int32>(gasSpeciesNames_.size());

    if (N_solid_ < 1 || N_gas_ < 1)
    {
        fatalErrorInFunction
            << "The reaction model needs at least one solid species "
            << "('solidSpecies') and one gas species ('productGas')." << endl;
        fatalExit;
    }

    if (   static_cast<int32>(solidMw_.size())        != N_solid_
        || static_cast<int32>(solidDensities_.size()) != N_solid_)
    {
        fatalErrorInFunction
            << "Dimension mismatch in solid properties." << endl;
        fatalExit;
    }

    if (   static_cast<int32>(gasMw_.size())                    != N_gas_
        || static_cast<int32>(defaultGasConcentrations_.size()) != N_gas_)
    {
        fatalErrorInFunction
            << "Dimension mismatch in gas properties: gasMolarMasses and "
            << "defaultGasConcentrations must have one entry per gas "
            << "species (" << N_gas_ << ")." << endl;
        fatalExit;
    }

    for (int32 s = 0; s < N_solid_; ++s)
    {
        if (solidMw_[s] <= real(0) || solidDensities_[s] <= real(0))
        {
            fatalErrorInFunction
                << "solidMolarMasses and solidDensities must be positive "
                << "(solid species " << solidSpeciesNames_[s] << ")." << endl;
            fatalExit;
        }

        for (int32 t = s + 1; t < N_solid_; ++t)
        {
            if (solidSpeciesNames_[s] == solidSpeciesNames_[t])
            {
                fatalErrorInFunction
                    << "Duplicate solid species name "
                    << solidSpeciesNames_[s] << "." << endl;
                fatalExit;
            }
        }
    }

    for (int32 g = 0; g < N_gas_; ++g)
    {
        if (gasMw_[g] <= real(0) || defaultGasConcentrations_[g] < real(0))
        {
            fatalErrorInFunction
                << "gasMolarMasses must be positive and "
                << "defaultGasConcentrations non-negative (gas species "
                << gasSpeciesNames_[g] << ")." << endl;
            fatalExit;
        }

        for (int32 h = g + 1; h < N_gas_; ++h)
        {
            if (gasSpeciesNames_[g] == gasSpeciesNames_[h])
            {
                fatalErrorInFunction
                    << "Duplicate gas species name "
                    << gasSpeciesNames_[g] << "." << endl;
                fatalExit;
            }
        }
    }

    if (!multiDict.containsDictionay("reactions"))
    {
        fatalErrorInFunction << "Missing 'reactions' sub-dictionary." << endl;
        fatalExit;
    }

    const dictionary& rxnDict = multiDict.subDict("reactions");
    wordList rxnNames = rxnDict.dictionaryKeywords();
    N_rxn_ = static_cast<int32>(rxnNames.size());

    if (N_rxn_ < 1)
    {
        fatalErrorInFunction
            << "The 'reactions' sub-dictionary has no reaction." << endl;
        fatalExit;
    }

    REPORT(0) << "  -> Found " << N_solid_ << " solid species, "
              << N_gas_ << " gas species, " << N_rxn_ << " reactions."
              << END_REPORT;

    stoicSolidFlattened_.resize(N_rxn_ * N_solid_, real(0));
    stoicGasFlattened_.resize(N_rxn_ * N_gas_, real(0));
    preExpFactor_.resize(N_rxn_, real(0));
    activationTemperature_.resize(N_rxn_, real(0));
    gasOrder_.resize(N_rxn_, real(0));
    solidOrder_.resize(N_rxn_, real(0));
    onsetTemperature_.resize(N_rxn_, real(0));
    gasSpeciesIdx_.resize(N_rxn_, 0);
    solidReactantIdx_.resize(N_rxn_, 0);
    dH_rxn_.resize(N_rxn_, real(0));

    // Relative tolerance of sum(nu * Mw) against sum(|nu| * Mw)
    constexpr real massBalanceTol = real(1e-4);

    for (int32 r = 0; r < N_rxn_; ++r)
    {
        const dictionary& rxn = rxnDict.subDict(rxnNames[r]);

        preExpFactor_[r]          = rxn.getVal<real>("preExpFactor");
        activationTemperature_[r] = rxn.getVal<real>("activationTemperature");
        onsetTemperature_[r]      = rxn.getVal<real>("onsetTemperature");
        dH_rxn_[r]                = rxn.getVal<real>("dH_rxn");
        gasOrder_[r]              = rxn.getVal<real>("gasOrder");
        solidOrder_[r]            = rxn.getVal<real>("solidOrder");

        if (   preExpFactor_[r]          < real(0)
            || activationTemperature_[r] < real(0)
            || onsetTemperature_[r]      < real(0)
            || gasOrder_[r]              <= real(0)
            || solidOrder_[r]            < real(0))
        {
            fatalErrorInFunction
                << "Reaction " << rxnNames[r] << ": preExpFactor, "
                << "activationTemperature, onsetTemperature and solidOrder "
                << "must not be negative, and gasOrder must be positive."
                << endl;
            fatalExit;
        }

        const int32 gIdx = rxn.getVal<int32>("gasSpeciesIdx");
        const int32 sIdx = rxn.getVal<int32>("solidReactantIdx");

        if (gIdx < 0 || gIdx >= N_gas_ || sIdx < 0 || sIdx >= N_solid_)
        {
            fatalErrorInFunction
                << "Species index out of bounds in reaction "
                << rxnNames[r] << endl;
            fatalExit;
        }

        gasSpeciesIdx_[r]    = gIdx;
        solidReactantIdx_[r] = sIdx;

        realVector sSolid = rxn.getVal<realVector>("stoicSolid");
        realVector sGas   = rxn.getVal<realVector>("stoicGas");

        if (   static_cast<int32>(sSolid.size()) != N_solid_
            || static_cast<int32>(sGas.size())   != N_gas_)
        {
            fatalErrorInFunction
                << "stoicSolid/stoicGas size mismatch in reaction "
                << rxnNames[r] << ": expected " << N_solid_
                << " solid and " << N_gas_ << " gas entries." << endl;
            fatalExit;
        }

        // The driving gas and the tracked solid are reactants, and a solid
        // product keeps the particle from vanishing.
        bool hasSolidProduct = false;
        real massBalance     = real(0);
        real massScale       = real(0);

        for (int32 s = 0; s < N_solid_; ++s)
        {
            stoicSolidFlattened_[r * N_solid_ + s] = sSolid[s];

            massBalance += sSolid[s] * solidMw_[s];
            massScale   += std::abs(sSolid[s] * solidMw_[s]);

            if (sSolid[s] > real(0)) hasSolidProduct = true;
        }

        for (int32 g = 0; g < N_gas_; ++g)
        {
            stoicGasFlattened_[r * N_gas_ + g] = sGas[g];

            massBalance += sGas[g] * gasMw_[g];
            massScale   += std::abs(sGas[g] * gasMw_[g]);
        }

        if (sSolid[sIdx] >= real(0) || sGas[gIdx] >= real(0))
        {
            fatalErrorInFunction
                << "Reaction " << rxnNames[r] << ": gasSpeciesIdx and "
                << "solidReactantIdx must refer to species with negative "
                << "stoichiometry (reactants)." << endl;
            fatalExit;
        }

        if (!hasSolidProduct)
        {
            fatalErrorInFunction
                << "Reaction " << rxnNames[r] << " has no solid product. "
                << "Particles that would vanish are not supported." << endl;
            fatalExit;
        }

        if (std::abs(massBalance) > massBalanceTol * massScale)
        {
            fatalErrorInFunction
                << "Reaction " << rxnNames[r] << " does not conserve mass: "
                << "sum(stoichiometry * molar mass) = " << massBalance
                << " kg/kmol. Check the molar masses and stoichiometry."
                << endl;
            fatalExit;
        }
    }
}

//------------------------------- constructors --------------------------------

multiSpeciesProperty::multiSpeciesProperty
(
    const word& fileName,
    repository* owner
)
:
    thermalProperty(fileName, owner)
{
    readReactiveDictionary();
}

multiSpeciesProperty::multiSpeciesProperty
(
    const word&       fileName,
    const fileSystem& dir
)
:
    thermalProperty(fileName, dir)
{
    readReactiveDictionary();
}

} // pFlow
