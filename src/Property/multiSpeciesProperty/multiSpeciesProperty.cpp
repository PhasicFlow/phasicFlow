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
    particleModelParams_.resize(N_rxn_);

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

        // The driving gas is the only gas reactant
        for (int32 g = 0; g < N_gas_; ++g)
        {
            if (g != gIdx && sGas[g] < real(0))
            {
                fatalErrorInFunction
                    << "Reaction " << rxnNames[r] << ": gas species " << g
                    << " has negative stoichiometry but is not "
                    << "gasSpeciesIdx. A reaction has one gas reactant."
                    << endl;
                fatalExit;
            }
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

        readParticleModel
        (
            rxn,
            rxnNames[r],
            r,
            (p_dir_ != nullptr ? *p_dir_ : fileSystem("caseSetup"))
        );
    }
}

void multiSpeciesProperty::readParticleModel
(
    const dictionary& rxn,
    const word&       rxnName,
    int32             r,
    const fileSystem& caseSetupDir
)
{
    auto& pm = particleModelParams_[r];

    const word model = rxn.getValOrSet<word>("particleModel", "none");
    const word basis = rxn.getValOrSet<word>("rateBasis", "outer");

    if      (model == "none")  pm.model = clcModelNone;
    else if (model == "USCM")  pm.model = clcModelUSCM;
    else if (model == "grain") pm.model = clcModelGrain;
    else
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": unknown particleModel " << model
            << ". Valid options are none, USCM and grain." << endl;
        fatalExit;
    }

    if (basis != "outer" && basis != "grain")
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": unknown rateBasis " << basis
            << ". Valid options are outer and grain." << endl;
        fatalExit;
    }

    pm.grainBasis = (basis == "grain") ? 1 : 0;

    if (pm.grainBasis && pm.model != clcModelGrain)
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": rateBasis grain needs "
            << "particleModel grain." << endl;
        fatalExit;
    }

    if (pm.model == clcModelNone) return;

    pm.solidProductIdx = rxn.getVal<int32>("solidProductIdx");
    pm.D_e             = rxn.getVal<real>("D_e");
    pm.D_eKnudsen      = rxn.getValOrSet<real>("D_eKnudsen", real(0));
    pm.D_eTemperature  = rxn.getValOrSet<real>("D_eTemperature", real(0));
    pm.k_c             = rxn.getValOrSet<real>("k_c", real(0));
    pm.Z               = rxn.getValOrSet<real>("Z", real(1));

    if (pm.model == clcModelGrain)
    {
        pm.grainRadius = rxn.getVal<real>("grainRadius");
        pm.D_P         = rxn.getValOrSet<real>("D_P", real(0));
    }

    if (   pm.solidProductIdx < 0 || pm.solidProductIdx >= N_solid_
        || stoicSolidFlattened_[r * N_solid_ + pm.solidProductIdx]
           <= real(0))
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": solidProductIdx must refer to "
            << "a solid species with positive stoichiometry." << endl;
        fatalExit;
    }

    if (   pm.D_e <= real(0) || pm.D_eKnudsen < real(0)
        || pm.D_eTemperature < real(0) || pm.D_P < real(0)
        || pm.k_c < real(0) || pm.Z <= real(0))
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": D_e and Z must be positive, "
            << "and D_eKnudsen, D_eTemperature, D_P and k_c must not be "
            << "negative." << endl;
        fatalExit;
    }

    // The rate of a particle model is the rate of its gas reactant: the
    // kinetics of the grain model are per mole of gas (Abad et al. 2007),
    // and the moduli and the film term take one mole of gas per reaction.
    const real nuGas = stoicGasFlattened_[r * N_gas_ + gasSpeciesIdx_[r]];

    if (std::abs(nuGas + real(1)) > real(1e-12))
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": a particle model needs the gas "
            << "reactant with the stoichiometry -1, not " << nuGas
            << ". Write the reaction per mole of the gas reactant." << endl;
        fatalExit;
    }

    if (pm.model == clcModelGrain && pm.grainRadius <= real(0))
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": grainRadius must be positive."
            << endl;
        fatalExit;
    }

    if (pm.model == clcModelGrain)
    {
        const Logical useTable =
            rxn.getValOrSet<Logical>("correctionTable", Logical("No"));

        if (useTable) readCorrectionTable(rxnName, r, caseSetupDir);
    }

    REPORT(1) << "Reaction " << rxnName << ": particleModel " << model
              << ", rateBasis " << basis
              << (pm.tableOffset >= 0 ? ", with" : ", without")
              << " correction table." << END_REPORT;
}

void multiSpeciesProperty::readCorrectionTable
(
    const word&       rxnName,
    int32             r,
    const fileSystem& caseSetupDir
)
{
    auto& pm = particleModelParams_[r];

    const fileSystem tableFile = caseSetupDir + "correctionTables";

    if (!tableFile.exist())
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << " has correctionTable Yes, but "
            << "the file " << tableFile << " does not exist. Run "
            << "correctionTablePhasicFlow in the case first." << endl;
        fatalExit;
    }

    dictionary tablesDict("correctionTables", tableFile);

    if (!tablesDict.containsDictionay(rxnName))
    {
        fatalErrorInFunction
            << "File " << tableFile << " has no table for reaction "
            << rxnName << ". Run correctionTablePhasicFlow again." << endl;
        fatalExit;
    }

    const dictionary& tableDict = tablesDict.subDict(rxnName);

    const real order  = tableDict.getVal<real>("reactionOrder");
    realVector sigma2 = tableDict.getVal<realVector>("sigma2");
    realVector g      = tableDict.getVal<realVector>("g");
    realVector X      = tableDict.getVal<realVector>("X");
    realVector lnc    = tableDict.getVal<realVector>("lnc");

    if (   sigma2.size() < 1 || g.size() < 1 || X.size() < 1
        || lnc.size() != sigma2.size() * g.size() * X.size())
    {
        fatalErrorInFunction
            << "Correction table of reaction " << rxnName << ": lnc must "
            << "hold size(sigma2) * size(g) * size(X) values." << endl;
        fatalExit;
    }

    if (std::abs(order - gasOrder_[r]) > real(1e-6))
    {
        fatalErrorInFunction
            << "Reaction " << rxnName << ": the correction table is for "
            << "reactionOrder " << order << ", but gasOrder is "
            << gasOrder_[r] << ". Run correctionTablePhasicFlow again."
            << endl;
        fatalExit;
    }

    auto ascending = [](const realVector& v)
    {
        for (size_t i = 1; i < v.size(); ++i)
            if (v[i] <= v[i - 1]) return false;
        return true;
    };

    if (!ascending(sigma2) || !ascending(g) || !ascending(X)
        || sigma2[0] <= real(0))
    {
        fatalErrorInFunction
            << "Correction table of reaction " << rxnName << ": sigma2, g "
            << "and X must ascend, and sigma2 must be positive." << endl;
        fatalExit;
    }

    pm.tableOffset = static_cast<int32>(correctionTableFlattened_.size());
    pm.nSigma2     = static_cast<int32>(sigma2.size());
    pm.nG          = static_cast<int32>(g.size());
    pm.nX          = static_cast<int32>(X.size());

    for (size_t i = 0; i < sigma2.size(); ++i)
        correctionTableFlattened_.push_back(std::log(sigma2[i]));

    for (size_t i = 0; i < g.size(); ++i)
        correctionTableFlattened_.push_back(g[i]);

    for (size_t i = 0; i < X.size(); ++i)
        correctionTableFlattened_.push_back(X[i]);

    for (size_t i = 0; i < lnc.size(); ++i)
        correctionTableFlattened_.push_back(lnc[i]);
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
