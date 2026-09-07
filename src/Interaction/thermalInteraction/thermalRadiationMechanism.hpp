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
#ifndef pFlow_thermalRadiationMechanism_hpp
#define pFlow_thermalRadiationMechanism_hpp
#include "dictionary.hpp"
#include "mapperNBS.hpp"
namespace pFlow
{
/**
 * @brief Radiation's neighbourhood sweep and update-interval.
 * Constructed only when enableRadiation is true. Owns no per-particle
 * storage: radSumTemp_/radNumPrt_ are pointFields on
 * thermalSphereParticles, written via iterate()'s views.
 *
 * @details
 * This class computes only a neighbourhood temperature sum and count
 * within radCut -- it does not itself evaluate the Stefan-Boltzmann
 * radiative flux. The actual radiative heat rate is computed on the
 * CFD side, using this sum as a local "environment temperature":
 * \f[
 *   Q_{i,rad} = \sigma \varepsilon_i A_i
 *   \left( T_{env,i}^4 - T_i^4 \right)
 * \f]
 * \f[
 *   T_{env,i} = \varepsilon_f T_{f,i} +
 *   (1-\varepsilon_f) \frac{1}{N_i} \sum_{j} T_j
 * \f]
 * where the sum over j runs over neighbours within radCut, matching
 * the sub-domain concept of Musser (2011) and the local-environment-
 * temperature formulation of Zhou et al. (2003, 2009). The same
 * formulation is presented in Norouzi et al. (2016), Section 6.3.2.4,
 * Eqs. 6.178-6.179.
 *
 * A direct consequence: in a purely standalone (non-CFD-coupled) run,
 * this sum is computed if radiation is enabled, but has no effect on
 * particle temperature at all, since there is no CFD side to consume
 * it.
 *
 * @cite Zhou, H.S., Flamant, G., Gauthier, D., Flitris, Y., 2003.
 * Simulation of coal combustion in a bubbling fluidized bed by
 * distinct element method. Chem. Eng. Res. Des. 81(9), 1144-1149.
 * @cite Zhou, Z.Y., Yu, A.B., Zulli, P., 2009. Particle scale study of
 * heat transfer in packed and bubbling fluidized beds. AIChE J. 55(4),
 * 868-884.
 * @cite Musser, J.M., 2011. Modeling of Heat Transfer and Reactive
 * Chemistry for Particles in Gas-Solid Flow Utilizing Continuum-
 * Discrete Methodology (CDM). PhD Dissertation, West Virginia
 * University.
 * @cite Norouzi, H.R., Zarghami, R., Sotudeh-Gharebagh, R., Mostoufi,
 * N., 2016. Coupled CFD-DEM Modeling: Formulation, Implementation and
 * Application to Multiphase Flows. Wiley, Section 6.3.2.4.
 */
class thermalRadiationMechanism
{
private:
    //- private members
        real    radCut_ = 0.0;
        uint32  radUpdateInterval_ = 1;
        uint32  stepCounter_ = 0;
public:
    //- constructors
        explicit thermalRadiationMechanism(const dictionary& thermoDict);
        ~thermalRadiationMechanism() = default;
    //- public methods
        inline
        real requiredSearchCut() const
        {
            return radCut_;
        }
        /// @brief Sweeps the neighbourhood on update steps only;
        /// otherwise a no-op (keeps the previous value).
        void iterate(
            const pFlagTypeDevice&          flags,
            const deviceViewType1D<realx3>& pos,
            const deviceViewType1D<real>&   temperature,
            const mapperNBS&                mapper,
            deviceViewType1D<real>          radSumTemp,
            deviceViewType1D<uint32>        radNumPrt);
}; // thermalRadiationMechanism
} // pFlow
#endif // pFlow_thermalRadiationMechanism_hpp
