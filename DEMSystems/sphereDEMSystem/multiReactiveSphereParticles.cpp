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

#include "multiReactiveSphereParticles.hpp"
#include "multiReactiveSphereParticlesKernels.hpp"

namespace pFlow
{

//----------------------------- private methods -------------------------------

void multiReactiveSphereParticles::checkReactiveMemory()
{
    thermalSphereFluidParticles::checkHostMemory();

    const uint32 numPrt   = static_cast<uint32>(this->temperature().size());
    const uint32 solidLen = numPrt * static_cast<uint32>(N_solid_);
    const uint32 gasLen   = numPrt * static_cast<uint32>(N_gas_);

    // Loaded from the fields at the start of every step
    if (solidMassFractions_.extent(0) != solidLen)
    {
        resizeNoInit(solidMassFractions_, solidLen);
    }

    if (gasMassSource_.extent(0) != gasLen)
    {
        resizeInit(gasMassSource_,     gasLen);
        resizeInit(gasMassSourceSp_,   gasLen);
        resizeInit(gasConcentrations_, gasLen);

        resizeNoInit(gasMassSourceHost_,     gasLen);
        resizeNoInit(gasMassSourceSpHost_,   gasLen);
        resizeNoInit(gasConcentrationsHost_, gasLen);

        resizeInit(gasMassSourceSum_,   gasLen);
        resizeInit(gasMassSourceSpSum_, gasLen);
    }

    if (massTransferCoeff_.extent(0) != numPrt)
    {
        resizeInit(massTransferCoeff_,     numPrt);
        resizeInit(massTransferCoeffHost_, numPrt);

        kineticsDev_.massTransferCoeff = massTransferCoeff_.data();
    }
}

void multiReactiveSphereParticles::setupKineticsDeviceStruct()
{
    const auto& props = multiSpheres_.multiProperties();

    N_solid_ = props.numSolid();
    N_gas_   = props.numGas();
    N_rxn_   = props.numRxn();

    // The kernel keeps per-particle rates and per-species tallies in
    // fixed-size arrays of clcMaxSpecies elements, so every dimension of the
    // reaction network must fit in them.
    if (   N_solid_ > clcMaxSpecies
        || N_gas_   > clcMaxSpecies
        || N_rxn_   > clcMaxSpecies)
    {
        fatalErrorInFunction
            << "Reaction network exceeds the fixed per-thread array size "
            << "used by the GPU kinetics kernel (limit = "
            << clcMaxSpecies << " for each of solid species, gas species, "
            << "and reactions).\n"
            << "N_solid=" << N_solid_ << ", N_gas=" << N_gas_
            << ", N_rxn=" << N_rxn_ << endl;
        fatalExit;
    }

    Kokkos::resize(d_stoicSolid_,            N_rxn_ * N_solid_);
    Kokkos::resize(d_stoicGas_,              N_rxn_ * N_gas_);
    Kokkos::resize(d_preExpFactor_,          N_rxn_);
    Kokkos::resize(d_activationTemperature_, N_rxn_);
    Kokkos::resize(d_gasOrder_,              N_rxn_);
    Kokkos::resize(d_solidOrder_,            N_rxn_);
    Kokkos::resize(d_onsetTemperature_,      N_rxn_);
    Kokkos::resize(d_gasSpeciesIdx_,         N_rxn_);
    Kokkos::resize(d_solidReactantIdx_,      N_rxn_);
    Kokkos::resize(d_dH_rxn_,                N_rxn_);
    Kokkos::resize(d_solidMw_,               N_solid_);
    Kokkos::resize(d_solidDensities_,        N_solid_);
    Kokkos::resize(d_gasMw_,                 N_gas_);

    auto hStoicSolid = Kokkos::create_mirror_view(d_stoicSolid_);
    auto hStoicGas   = Kokkos::create_mirror_view(d_stoicGas_);
    auto hPreExp     = Kokkos::create_mirror_view(d_preExpFactor_);
    auto hActTemp    = Kokkos::create_mirror_view(d_activationTemperature_);
    auto hGasOrd     = Kokkos::create_mirror_view(d_gasOrder_);
    auto hSolOrd     = Kokkos::create_mirror_view(d_solidOrder_);
    auto hOnsetTemp  = Kokkos::create_mirror_view(d_onsetTemperature_);
    auto hGasIdx     = Kokkos::create_mirror_view(d_gasSpeciesIdx_);
    auto hSolIdx     = Kokkos::create_mirror_view(d_solidReactantIdx_);
    auto hDH         = Kokkos::create_mirror_view(d_dH_rxn_);
    auto hSolidMw    = Kokkos::create_mirror_view(d_solidMw_);
    auto hSolidRho   = Kokkos::create_mirror_view(d_solidDensities_);
    auto hGasMw      = Kokkos::create_mirror_view(d_gasMw_);

    for (int32 i = 0; i < N_rxn_ * N_solid_; ++i)
        hStoicSolid(i) = props.stoicSolidFlattened()[i];

    for (int32 i = 0; i < N_rxn_ * N_gas_; ++i)
        hStoicGas(i) = props.stoicGasFlattened()[i];

    for (int32 r = 0; r < N_rxn_; ++r)
    {
        hPreExp(r)    = props.preExpFactor()[r];
        hActTemp(r)   = props.activationTemperature()[r];
        hGasOrd(r)    = props.gasOrder()[r];
        hSolOrd(r)    = props.solidOrder()[r];
        hOnsetTemp(r) = props.onsetTemperature()[r];
        hGasIdx(r)    = props.gasSpeciesIdx()[r];
        hSolIdx(r)    = props.solidReactantIdx()[r];
        hDH(r)        = props.dH_rxn()[r];
    }

    for (int32 s = 0; s < N_solid_; ++s)
    {
        hSolidMw(s)  = props.solidMw()[s];
        hSolidRho(s) = props.solidDensities()[s];
    }

    for (int32 g = 0; g < N_gas_; ++g)
        hGasMw(g) = props.gasMw()[g];

    Kokkos::deep_copy(d_stoicSolid_,            hStoicSolid);
    Kokkos::deep_copy(d_stoicGas_,              hStoicGas);
    Kokkos::deep_copy(d_preExpFactor_,          hPreExp);
    Kokkos::deep_copy(d_activationTemperature_, hActTemp);
    Kokkos::deep_copy(d_gasOrder_,              hGasOrd);
    Kokkos::deep_copy(d_solidOrder_,            hSolOrd);
    Kokkos::deep_copy(d_onsetTemperature_,      hOnsetTemp);
    Kokkos::deep_copy(d_gasSpeciesIdx_,         hGasIdx);
    Kokkos::deep_copy(d_solidReactantIdx_,      hSolIdx);
    Kokkos::deep_copy(d_dH_rxn_,                hDH);
    Kokkos::deep_copy(d_solidMw_,               hSolidMw);
    Kokkos::deep_copy(d_solidDensities_,        hSolidRho);
    Kokkos::deep_copy(d_gasMw_,                 hGasMw);

    // Particle-model parameters and correction tables
    const auto& pmParams = props.particleModelParams();
    const auto& pmTable  = props.correctionTableFlattened();

    Kokkos::resize(d_particleModel_,   N_rxn_);
    Kokkos::resize(d_correctionTable_, pmTable.size());

    auto hParticleModel = Kokkos::create_mirror_view(d_particleModel_);
    auto hTable         = Kokkos::create_mirror_view(d_correctionTable_);

    for (int32 r = 0; r < N_rxn_; ++r)
        hParticleModel(r) = pmParams[r];

    for (size_t i = 0; i < pmTable.size(); ++i)
        hTable(i) = pmTable[i];

    Kokkos::deep_copy(d_particleModel_,   hParticleModel);
    Kokkos::deep_copy(d_correctionTable_, hTable);

    kineticsDev_.N_solid               = N_solid_;
    kineticsDev_.N_gas                 = N_gas_;
    kineticsDev_.N_rxn                 = N_rxn_;
    kineticsDev_.stoicSolid            = d_stoicSolid_.data();
    kineticsDev_.stoicGas              = d_stoicGas_.data();
    kineticsDev_.preExpFactor          = d_preExpFactor_.data();
    kineticsDev_.activationTemperature = d_activationTemperature_.data();
    kineticsDev_.gasOrder              = d_gasOrder_.data();
    kineticsDev_.solidOrder            = d_solidOrder_.data();
    kineticsDev_.onsetTemperature      = d_onsetTemperature_.data();
    kineticsDev_.gasSpeciesIdx         = d_gasSpeciesIdx_.data();
    kineticsDev_.solidReactantIdx      = d_solidReactantIdx_.data();
    kineticsDev_.dH_rxn                = d_dH_rxn_.data();
    kineticsDev_.solidMw               = d_solidMw_.data();
    kineticsDev_.solidDensities        = d_solidDensities_.data();
    kineticsDev_.gasMw                 = d_gasMw_.data();
    kineticsDev_.particleModel         = d_particleModel_.data();
    kineticsDev_.correctionTable       = d_correctionTable_.data();
}

void multiReactiveSphereParticles::loadSolidFractions()
{
    auto mask = dynPointStruct().activePointsMaskDevice();

    for (int32 s = 0; s < N_solid_; ++s)
    {
        multiReactiveSphereParticlesKernels::loadSolidFraction
        (
            mask,
            static_cast<uint32>(N_solid_),
            static_cast<uint32>(s),
            solidFracFields_[s]->deviceViewAll(),
            solidMassFractions_
        );
    }
}

void multiReactiveSphereParticles::storeSolidState()
{
    auto mask = dynPointStruct().activePointsMaskDevice();

    for (int32 s = 0; s < N_solid_; ++s)
    {
        multiReactiveSphereParticlesKernels::storeSolidFraction
        (
            mask,
            static_cast<uint32>(N_solid_),
            static_cast<uint32>(s),
            solidMassFractions_,
            solidFracFields_[s]->deviceViewAll()
        );
    }

    multiReactiveSphereParticlesKernels::storeSolidMass
    (
        mask,
        mass().deviceViewAll(),
        solidMass_->deviceViewAll()
    );
}

void multiReactiveSphereParticles::validateSolidFractions()
{
    loadSolidFractions();

    auto mask = dynPointStruct().activePointsMaskDevice();

    // The fields are written with six significant digits
    constexpr real sumTol = real(1e-4);

    const real maxError =
        multiReactiveSphereParticlesKernels::maxSolidFractionError
        (
            mask,
            static_cast<uint32>(N_solid_),
            solidMassFractions_
        );

    if (maxError > sumTol)
    {
        fatalErrorInFunction
            << "The solidFrac_<species> fields of a particle deviate by "
            << maxError << " from a sum of 1 (limit " << sumTol << ")."
            << endl;
        fatalExit;
    }

    multiReactiveSphereParticlesKernels::normalizeSolidFractions
    (
        mask,
        static_cast<uint32>(N_solid_),
        solidMassFractions_
    );
}

void multiReactiveSphereParticles::checkSolidVolume()
{
    if (!multiSpheres_.multiProperties().enableShrinking()) return;

    constexpr real volumeTol = real(1e-3);

    const real mismatch =
        multiReactiveSphereParticlesKernels::maxVolumeMismatch
        (
            dynPointStruct().activePointsMaskDevice(),
            kineticsDev_,
            solidMassFractions_,
            mass().deviceViewAll(),
            diameter().deviceViewAll()
        );

    if (mismatch > volumeTol)
    {
        fatalErrorInFunction
            << "The volume of a particle sphere differs by a relative "
            << mismatch << " from the volume of its solid composition "
            << "(limit " << volumeTol << "). The particle density of the "
            << "'interaction' file must match solidDensities and the "
            << "solidFrac_<species> fields." << endl;
        fatalExit;
    }
}

void multiReactiveSphereParticles::setDefaultGasConcentrations
(
    uint32 first,
    uint32 last
)
{
    const auto&  defaultConc = multiSpheres_.defaultGasConcentrations();
    const uint32 ng          = static_cast<uint32>(N_gas_);

    Kokkos::deep_copy(gasConcentrationsHost_, gasConcentrations_);

    for (uint32 p = first; p < last; ++p)
    {
        for (uint32 g = 0; g < ng; ++g)
        {
            gasConcentrationsHost_[p * ng + g] = defaultConc[g];
        }
    }

    Kokkos::deep_copy(gasConcentrations_, gasConcentrationsHost_);
}

void multiReactiveSphereParticles::updateProbeFields()
{
    if (!reactionsEnabled() || !massTransferCoeffField_) return;

    using probePolicy = Kokkos::RangePolicy
    <
        DefaultExecutionSpace,
        Kokkos::IndexType<uint32>
    >;

    const uint32 ng   = static_cast<uint32>(N_gas_);
    const uint32 nPrt = static_cast<uint32>(massTransferCoeff_.extent(0));

    auto conc = gasConcentrations_;
    auto kc   = massTransferCoeff_;

    for (uint32 g = 0; g < ng; ++g)
    {
        auto field = gasConcFields_[g]->deviceViewAll();

        const uint32 n = min(nPrt, static_cast<uint32>(field.extent(0)));

        Kokkos::parallel_for
        (
            "multiReactiveSphereParticles::gasConcProbe",
            probePolicy(0, n),
            LAMBDA_HD(uint32 i)
            {
                field[i] = conc[i * ng + g];
            }
        );
    }

    auto kcField = massTransferCoeffField_->deviceViewAll();

    const uint32 n = min(nPrt, static_cast<uint32>(kcField.extent(0)));

    Kokkos::parallel_for
    (
        "multiReactiveSphereParticles::massTransferCoeffProbe",
        probePolicy(0, n),
        LAMBDA_HD(uint32 i)
        {
            kcField[i] = kc[i];
        }
    );

    Kokkos::fence();
}

void multiReactiveSphereParticles::initializeReactiveParticles()
{
    if (!reactionsEnabled()) return;

    setupKineticsDeviceStruct();

    const auto& solidNames =
        multiSpheres_.multiProperties().solidSpeciesNames();

    // One solidFrac_<species> field per solid species. Like temperature,
    // the fields are read from the case; a case that lacks them stops with
    // an error.
    solidFracFields_.reserve(static_cast<size_t>(N_solid_));

    for (int32 s = 0; s < N_solid_; ++s)
    {
        solidFracFields_.push_back
        (
            makeUnique<realPointField_D>
            (
                objectFile
                (
                    "solidFrac_" + word(solidNames[s]),
                    "",
                    objectFile::READ_ALWAYS,
                    objectFile::WRITE_ALWAYS
                ),
                dynPointStruct(),
                real(0)
            )
        );
    }

    reactionHeat_ = makeUnique<realPointField_D>
    (
        objectFile
        (
            "reactionHeat",
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER
        ),
        dynPointStruct(),
        real(0)
    );

    // The mass of a reacting particle cannot be rebuilt from the shape, so
    // it is read when the file exists (restart); a particle without a stored
    // mass keeps its nominal one.
    solidMass_ = makeUnique<realPointField_D>
    (
        objectFile
        (
            "solidMass",
            "",
            objectFile::READ_IF_PRESENT,
            objectFile::WRITE_ALWAYS
        ),
        dynPointStruct(),
        real(0)
    );

    // Probe fields for the replay check: kept in memory, not written
    const auto& gasNames =
        multiSpheres_.multiProperties().gasSpeciesNames();

    gasConcFields_.reserve(static_cast<size_t>(N_gas_));

    for (int32 g = 0; g < N_gas_; ++g)
    {
        gasConcFields_.push_back
        (
            makeUnique<realPointField_D>
            (
                objectFile
                (
                    "gasConc_" + word(gasNames[g]),
                    "",
                    objectFile::READ_NEVER,
                    objectFile::WRITE_NEVER
                ),
                dynPointStruct(),
                real(0)
            )
        );
    }

    massTransferCoeffField_ = makeUnique<realPointField_D>
    (
        objectFile
        (
            "massTransferCoeff",
            "",
            objectFile::READ_NEVER,
            objectFile::WRITE_NEVER
        ),
        dynPointStruct(),
        real(0)
    );

    checkReactiveMemory();
    validateSolidFractions();

    multiReactiveSphereParticlesKernels::restoreSolidState
    (
        dynPointStruct().activePointsMaskDevice(),
        multiSpheres_.multiProperties().enableShrinking(),
        kineticsDev_,
        solidMassFractions_,
        solidMass_->deviceViewAll(),
        mass().deviceViewAll(),
        diameter().deviceViewAll(),
        I().deviceViewAll()
    );

    checkSolidVolume();
    storeSolidState();

    // Gas concentrations start at the default values: the standalone
    // solver has no CFD mesh to supply them, and the coupled solvers
    // overwrite them at the first exchange.
    setDefaultGasConcentrations
    (
        0,
        static_cast<uint32>(this->temperature().size())
    );

    updateProbeFields();

    REPORT(1) << Green_Text("  -> Reactive arrays initialized.") << END_REPORT;
}

//------------------------------- constructors --------------------------------

multiReactiveSphereParticles::multiReactiveSphereParticles
(
    systemControl&                  control,
    const multiReactiveSphereShape& mShpShape
)
:
    thermalSphereFluidParticles(control, mShpShape),
    multiSpheres_(mShpShape)
{
    initializeReactiveParticles();
}

//----------------------------- public methods --------------------------------

bool multiReactiveSphereParticles::beforeIteration()
{
    if (!thermalSphereFluidParticles::beforeIteration()) return false;
    if (!reactionsEnabled()) return true;

    checkReactiveMemory();
    loadSolidFractions();

    Kokkos::deep_copy(gasMassSource_,                 real(0));
    Kokkos::deep_copy(gasMassSourceSp_,               real(0));
    Kokkos::deep_copy(reactionHeat_->deviceViewAll(), real(0));

    return true;
}

bool multiReactiveSphereParticles::iterate()
{
    if (!thermalSphereFluidParticles::iterate()) return false;
    if (!reactionsEnabled()) return true;

    multiReactiveSphereParticlesKernels::integrateReactionHeat
    (
        dynPointStruct().activePointsMaskDevice(),
        control().time().dt(),
        static_cast<uint32>(N_gas_),
        gasMassSource_,
        reactionHeat_->deviceViewAll(),
        mass().deviceViewAll(),
        Cp().deviceViewAll(),
        temperature().deviceViewAll()
    );

    multiReactiveSphereParticlesKernels::accumulateReactionData
    (
        dynPointStruct().activePointsMaskDevice(),
        control().time().dt(),
        static_cast<uint32>(N_gas_),
        gasMassSource_,
        gasMassSourceSp_,
        gasMassSourceSum_,
        gasMassSourceSpSum_
    );

    return true;
}

bool multiReactiveSphereParticles::afterIteration()
{
    if (reactionsEnabled())
    {
        storeSolidState();
    }

    return thermalSphereFluidParticles::afterIteration();
}

bool multiReactiveSphereParticles::insertParticles
(
    const realx3Vector& pos,
    const wordVector&   names,
    const anyList&      vars
)
{
    const uint32 oldSize = static_cast<uint32>(this->temperature().size());

    if (!thermalSphereFluidParticles::insertParticles(pos, names, vars))
    {
        return false;
    }

    if (!reactionsEnabled()) return true;

    checkReactiveMemory();

    // Particles appended after the old ones start from the default gas
    // concentrations; particles placed in a free slot keep its previous
    // values until the next CFD exchange.
    setDefaultGasConcentrations
    (
        oldSize,
        static_cast<uint32>(this->temperature().size())
    );

    updateProbeFields();

    // The solidFrac_<species> fields of the inserted particles come from
    // vars or take their default value, and must sum to one.
    validateSolidFractions();
    checkSolidVolume();
    storeSolidState();

    return true;
}

void multiReactiveSphereParticles::gasConcentrationsHostUpdatedSync()
{
    checkReactiveMemory();

    Kokkos::deep_copy(gasConcentrations_, gasConcentrationsHost_);

    updateProbeFields();
}

void multiReactiveSphereParticles::massTransferCoeffHostUpdatedSync()
{
    checkReactiveMemory();

    Kokkos::deep_copy(massTransferCoeff_, massTransferCoeffHost_);

    updateProbeFields();
}

void multiReactiveSphereParticles::reactionDataHostUpdatedSync()
{
    checkReactiveMemory();

    Kokkos::deep_copy(gasMassSourceHost_,   gasMassSource_);
    Kokkos::deep_copy(gasMassSourceSpHost_, gasMassSourceSp_);
}

void multiReactiveSphereParticles::resetReactionDataSum()
{
    checkReactiveMemory();

    Kokkos::deep_copy(gasMassSourceSum_,   real(0));
    Kokkos::deep_copy(gasMassSourceSpSum_, real(0));
}

void multiReactiveSphereParticles::reactionDataAverageHostSync(real dt)
{
    checkReactiveMemory();

    Kokkos::deep_copy(gasMassSourceHost_,   gasMassSourceSum_);
    Kokkos::deep_copy(gasMassSourceSpHost_, gasMassSourceSpSum_);

    // A zero time span leaves the averages at zero
    const real   invDt  = dt > real(0) ? real(1) / dt : real(0);
    const uint32 gasLen = static_cast<uint32>(gasMassSourceHost_.extent(0));

    for (uint32 i = 0; i < gasLen; ++i)
    {
        gasMassSourceHost_[i]   *= invDt;
        gasMassSourceSpHost_[i] *= invDt;
    }
}

} // pFlow
