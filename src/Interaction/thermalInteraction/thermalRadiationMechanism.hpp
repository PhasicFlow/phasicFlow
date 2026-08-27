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
 * @brief Owns radiation's own update-interval decision (independent
 * of thermalInteraction's neighborListUpdateInterval -- see that
 * class's doc comment for why these are two separate values) and its
 * complete neighbourhood sweep. Constructed only when enableRadiation
 * is true.
 *
 * Owns no per-particle storage: radSumTemp_/radNumPrt_ live as
 * pointFields on thermalSphereParticles (auto-resizing, and
 * constructible only where non-const point-structure access exists),
 * the same ownership split already used for heatSourceCondPP_/
 * heatSourcePFP_ and thermalConductionPFPMechanism. This class only
 * writes into them via the views passed into iterate().
 */
class thermalRadiationMechanism
{
private:

    //- private members

        real    radCut_ = 0.0;

        uint32  radUpdateInterval_ = 1;

        /// Own step counter for the update-interval decision.
        uint32  stepCounter_ = 0;

public:

    //- constructors

        /// @brief Reads radCut and radUpdateInterval (both mandatory)
        /// from thermoDict.
        explicit thermalRadiationMechanism(const dictionary& thermoDict);

        ~thermalRadiationMechanism() = default;

    //- public methods

        /// @brief Search radius this mechanism needs.
        inline
        real requiredSearchCut() const
        {
            return radCut_;
        }

        /**
         * @brief Sweeps the 27-cell neighbourhood of every active
         * particle and accumulates neighbour temperatures into
         * radSumTemp/radNumPrt, but only on update steps (every
         * radUpdateInterval_-th call, decided internally). Between
         * update steps, this is a no-op: radSumTemp/radNumPrt simply
         * keep whatever value they already hold -- that retained
         * value is the "history" this interval is meant to preserve.
         */
        void iterate(
            const pFlagTypeDevice&          m,
            const deviceViewType1D<realx3>& pos,
            const deviceViewType1D<real>&   temperature,
            const mapperNBS::CellIterator&  cellIter,
            const realx3&                   domainMin,
            const real&                     cellSize,
            const int32x3&                  numCells,
            deviceViewType1D<real>          radSumTemp,
            deviceViewType1D<uint32>        radNumPrt);

}; // thermalRadiationMechanism

} // pFlow

#endif // pFlow_thermalRadiationMechanism_hpp
