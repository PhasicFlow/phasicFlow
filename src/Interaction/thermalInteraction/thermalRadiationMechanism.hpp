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

#include "phasicFlowKokkos.hpp"
#include "dictionary.hpp"

namespace pFlow
{

/**
 * @brief Owns radiation's dictionary parameters and per-particle
 * output (radSumTemp_/radNumPrt_). Constructed only when
 * enableRadiation is true -- otherwise no memory is allocated.
 *
 * Device-only: no host mirror here. thermalSphereDEMSystem (the
 * CFD-coupling-only class) owns host mirrors and pulls from this
 * class's device views directly, since host mirrors are only ever
 * needed for CFD exchange.
 */
class thermalRadiationMechanism
{
private:

    //- private members

        real                        radCut_ = 0.0;

        uint32                      radUpdateInterval_ = 1;

        /// Sum of neighbouring particle temperatures per particle,
        /// used by the linearised radiation model (CFD side) [K].
        deviceViewType1D<real>      radSumTemp_;

        /// Number of radiating neighbours found for each particle [-].
        deviceViewType1D<uint32>    radNumPrt_;

public:

    //- constructors

        /// @brief Reads radCut (mandatory) and radUpdateInterval
        /// (optional, default 1) from thermoDict.
        explicit thermalRadiationMechanism(const dictionary& thermoDict);

        ~thermalRadiationMechanism() = default;

    //- public methods

        /// @brief Search radius this mechanism needs.
        inline
        real requiredSearchCut() const
        {
            return radCut_;
        }

        inline
        uint32 updateInterval() const
        {
            return radUpdateInterval_;
        }

        /// @brief Resizes radSumTemp_/radNumPrt_ to the current
        /// particle count; a no-op once already sized.
        void ensureMemory(size_t numParticles);

        inline
        const deviceViewType1D<real>& radSumTemp() const
        {
            return radSumTemp_;
        }

        inline
        deviceViewType1D<real>& radSumTemp()
        {
            return radSumTemp_;
        }

        inline
        const deviceViewType1D<uint32>& radNumPrt() const
        {
            return radNumPrt_;
        }

        inline
        deviceViewType1D<uint32>& radNumPrt()
        {
            return radNumPrt_;
        }

}; // thermalRadiationMechanism

} // pFlow

#endif // pFlow_thermalRadiationMechanism_hpp
