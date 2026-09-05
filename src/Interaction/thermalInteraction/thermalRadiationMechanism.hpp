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
