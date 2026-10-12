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

#ifndef pFlow_multiReactiveSphereParticles_hpp
#define pFlow_multiReactiveSphereParticles_hpp

#include <vector>

#include "thermalSphereFluidParticles.hpp"
#include "multiReactiveSphereShape.hpp"
#include "multiReactionTypes.hpp"

namespace pFlow
{

/**
 * @brief GPU particle container for multi-species CLC reactions.
 *
 * Extends thermalSphereFluidParticles with the solid composition of each
 * particle, the kinetics data uploaded to the GPU, the gas concentration
 * and mass-source buffers exchanged with the CFD side, and the per-particle
 * reaction heat (all of it deposited in the particle).
 * fluidForce_/fluidTorque_ inherited from thermalSphereFluidParticles stay
 * unused in the standalone solver.
 */
class multiReactiveSphereParticles
:
    public thermalSphereFluidParticles
{
private:

    /// Shape holding the multi-species property database.
    const multiReactiveSphereShape& multiSpheres_;

    /// Number of solid species.
    int32  N_solid_ = 0;

    /// Number of gas species.
    int32  N_gas_ = 0;

    /// Number of reactions.
    int32  N_rxn_ = 0;

    /// Device copy of the solid stoichiometry.
    deviceViewType1D<real>   d_stoicSolid_;

    /// Device copy of the gas stoichiometry.
    deviceViewType1D<real>   d_stoicGas_;

    /// Device copy of the pre-exponential factors.
    deviceViewType1D<real>   d_preExpFactor_;

    /// Device copy of the activation temperatures [K].
    deviceViewType1D<real>   d_activationTemperature_;

    /// Device copy of the reaction orders in the gas concentration.
    deviceViewType1D<real>   d_gasOrder_;

    /// Device copy of the reaction orders in the solid mass fraction.
    deviceViewType1D<real>   d_solidOrder_;

    /// Device copy of the per-reaction onset temperatures [K].
    deviceViewType1D<real>   d_onsetTemperature_;

    /// Device copy of the gas species index of each reaction.
    deviceViewType1D<int32>  d_gasSpeciesIdx_;

    /// Device copy of the solid reactant index of each reaction.
    deviceViewType1D<int32>  d_solidReactantIdx_;

    /// Device copy of the reaction enthalpies [J/kmol].
    deviceViewType1D<real>   d_dH_rxn_;

    /// Device copy of the solid molar masses [kg/kmol].
    deviceViewType1D<real>   d_solidMw_;

    /// Device copy of the solid densities [kg/m3].
    deviceViewType1D<real>   d_solidDensities_;

    /// Device copy of the gas molar masses [kg/kmol].
    deviceViewType1D<real>   d_gasMw_;

    /// Kinetics data passed by value to the reaction kernel.
    clcKineticsDevice        kineticsDev_;

    /// Reaction heat of each particle [W].
    uniquePtr<realPointField_D> reactionHeat_ = nullptr;

    /// Total solid mass of each particle [kg], read at restart.
    uniquePtr<realPointField_D> solidMass_ = nullptr;

    /// One solidFrac_<species> field per solid species.
    std::vector<uniquePtr<realPointField_D>> solidFracFields_;

    /// Solid mass fractions, flat [N_p x N_solid]: the working copy of the
    /// solidFrac_<species> fields. The kernel needs contiguous access to
    /// all species of a particle. Loaded in beforeIteration(), stored back
    /// in afterIteration().
    deviceViewType1D<real>   solidMassFractions_;

    /// Gas mass source [kg/s], flat [N_p x N_gas].
    deviceViewType1D<real>   gasMassSource_;

    /// Implicit gas sink coefficient, flat [N_p x N_gas].
    deviceViewType1D<real>   gasMassSourceSp_;

    /// Gas concentrations [kmol/m3], flat [N_p x N_gas].
    deviceViewType1D<real>   gasConcentrations_;

    /// Host mirror of gasMassSource_.
    hostViewType1D<real>     gasMassSourceHost_;

    /// Host mirror of gasMassSourceSp_.
    hostViewType1D<real>     gasMassSourceSpHost_;

    /// Host mirror of gasConcentrations_.
    hostViewType1D<real>     gasConcentrationsHost_;

    /// True when the multi-reaction model is enabled.
    inline bool reactionsEnabled() const
    {
        return multiSpheres_.multiProperties().enableMultiReaction();
    }

    /// Sizes the reactive device and host arrays to the particle count.
    void checkReactiveMemory();

    /// Uploads the kinetics data of multiSpeciesProperty to the GPU.
    void setupKineticsDeviceStruct();

    /// Copies the solidFrac_<species> fields into solidMassFractions_.
    void loadSolidFractions();

    /// Copies solidMassFractions_ into the solidFrac_<species> fields and
    /// the particle mass into solidMass_.
    void storeSolidState();

    /**
     * Loads the solid mass fractions, checks that they sum to one for every
     * particle and normalises them.
     */
    void validateSolidFractions();

    /// Checks that diameter and mass match the solid composition
    /// (shrinking model only).
    void checkSolidVolume();

    /// Sets the gas concentrations of the particles [first, last) to the
    /// default values.
    void setDefaultGasConcentrations(uint32 first, uint32 last);

    /// Creates the solid fields, restores the evolved particle state and
    /// sets the gas buffers.
    void initializeReactiveParticles();

public:

    TypeInfo("multiReactiveSphereParticles");

    multiReactiveSphereParticles
    (
        systemControl&                  control,
        const multiReactiveSphereShape& mShpShape
    );

    ~multiReactiveSphereParticles() override = default;

    bool beforeIteration() override;
    bool iterate()         override;
    bool afterIteration()  override;

    bool insertParticles
    (
        const realx3Vector& pos,
        const wordVector&   names,
        const anyList&      vars
    ) override;

    inline auto getActiveMask()
    {
        return dynPointStruct().activePointsMaskDevice();
    }

    inline const clcKineticsDevice& kineticsDevice() const
    {
        return kineticsDev_;
    }

    inline
    deviceViewType1D<real>& solidMassFractions()
    {
        return solidMassFractions_;
    }

    inline
    deviceViewType1D<real>& gasMassSource()
    {
        return gasMassSource_;
    }

    inline
    deviceViewType1D<real>& gasMassSourceSp()
    {
        return gasMassSourceSp_;
    }

    inline
    deviceViewType1D<real>& gasConcentrations()
    {
        return gasConcentrations_;
    }

    inline
    realPointField_D& reactionHeat()
    {
        return *reactionHeat_;
    }

    inline
    hostViewType1D<real>& gasMassSourceHost()
    {
        return gasMassSourceHost_;
    }

    inline
    hostViewType1D<real>& gasMassSourceSpHost()
    {
        return gasMassSourceSpHost_;
    }

    inline
    hostViewType1D<real>& gasConcentrationsHost()
    {
        return gasConcentrationsHost_;
    }

    /// Copies the gas concentrations from host to GPU (after the CFD gather).
    void gasConcentrationsHostUpdatedSync();

    /// Copies the gas mass sources from GPU to host (for the CFD side).
    void reactionDataHostUpdatedSync();
};

} // pFlow

#endif // pFlow_multiReactiveSphereParticles_hpp
