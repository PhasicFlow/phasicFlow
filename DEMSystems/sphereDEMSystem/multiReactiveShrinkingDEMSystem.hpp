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

#ifndef pFlow_multiReactiveShrinkingDEMSystem_hpp
#define pFlow_multiReactiveShrinkingDEMSystem_hpp

#include "thermalSphereDEMSystem.hpp"
#include "multiReactiveSphereParticles.hpp"
#include "multiShrinkingInteraction.hpp"
#include "multiSpeciesProperty.hpp"

namespace pFlow
{

/**
 * @brief DEM system for multi-species Chemical Looping Combustion.
 *
 * Extends thermalSphereDEMSystem with the multi-species Arrhenius reactions
 * of multiShrinkingInteraction. The reactive particles replace the thermal
 * particles of the base class (thermalParticles_), so the thermal coupling
 * interfaces are inherited unchanged. The gas concentrations and the gas
 * mass sources are the reaction data exchanged with the CFD side.
 */
class multiReactiveShrinkingDEMSystem
:
    public thermalSphereDEMSystem
{
private:

    /// Multi-species property database (kinetics, stoichiometry).
    uniquePtr<multiSpeciesProperty>      multiProp_          = nullptr;

    /// Reactive particles; the same object as thermalParticles_.
    multiReactiveSphereParticles*        multiReactivePrtcl_ = nullptr;

    /// Multi-species Arrhenius reactions; null when the model is disabled.
    uniquePtr<multiShrinkingInteraction> multiShrinkingInt_  = nullptr;

    /// Runs the DEM sub-steps up to the stop time.
    bool loop();

public:

    TypeInfo("multiReactiveSphereDEMSystem");

    multiReactiveShrinkingDEMSystem
    (
        word                    demSystemName,
        const std::vector<box>& domains,
        int                     argc,
        char*                   argv[],
        bool                    requireRVel = false
    );

    ~multiReactiveShrinkingDEMSystem() override = default;

    add_vCtor
    (
        DEMSystem,
        multiReactiveShrinkingDEMSystem,
        word
    );

    bool beforeIteration() override;

    bool iterate
    (
        real upToTime,
        real timeToWrite,
        word timeName
    ) override;

    bool iterate(real upToTime) override;

    //--- reaction coupling interfaces ----------------------------------------

    span<real> gasMassSource() override;

    span<real> gasMassSourceSp() override;

    span<real> gasConcentrations() override;

    bool sendGasConcentrationsToDEM() override;

    /// Gas species names in the order of the DEM kinetics.
    std::vector<std::string> gasSpeciesNames() const override;

    /// Gas molar masses [kg/kmol] in the order of gasSpeciesNames().
    std::vector<real> gasMolarMasses() const override;
};

} // pFlow

#endif // pFlow_multiReactiveShrinkingDEMSystem_hpp
