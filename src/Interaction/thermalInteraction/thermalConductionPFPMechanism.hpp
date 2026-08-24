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

#ifndef pFlow_thermalConductionPFPMechanism_hpp
#define pFlow_thermalConductionPFPMechanism_hpp

#include "types.hpp"
#include "dictionary.hpp"

namespace pFlow
{

/**
 * @brief Owns the parameters shared by static-contact conduction
 * (Q_pp) and particle-fluid-particle transfer (Q_pfp) -- combined
 * because both need the Hertzian contact radius from
 * thermalConductionPFPKernels::contactConduction(), even when Q_pp
 * itself is disabled.
 *
 * Constructed only when enableConduction or enablePFP is true.
 * Owns no per-particle memory: Q_pp/Q_pfp outputs live on
 * thermalSphereParticles, the class that consumes them.
 */
class thermalConductionPFPMechanism
{
private:

    //- private members

        bool    enableConduction_ = false;

        bool    enablePFP_        = false;

        real    simYoungsModulus_ = 1e7;

public:

    //- constructors

        /// @brief Reads simYoungsModulus from thermoDict -- mandatory
        /// for the Hertzian contact-radius correction.
        thermalConductionPFPMechanism(
            const dictionary&   thermoDict,
            bool                enableConduction,
            bool                enablePFP);

        ~thermalConductionPFPMechanism() = default;

    //- public methods

        inline
        bool conductionEnabled() const
        {
            return enableConduction_;
        }

        inline
        bool pfpEnabled() const
        {
            return enablePFP_;
        }

        inline
        real simYoungsModulus() const
        {
            return simYoungsModulus_;
        }

        /// @brief Search radius needed: 2x the largest contact
        /// distance for conduction, 3x for PFP's wider reach.
        real requiredSearchCut(real maxBoundingSphere) const;

}; // thermalConductionPFPMechanism

} // pFlow

#endif // pFlow_thermalConductionPFPMechanism_hpp
