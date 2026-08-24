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

#include "thermalRadiationMechanism.hpp"

namespace pFlow
{

thermalRadiationMechanism::thermalRadiationMechanism(
    const dictionary& thermoDict)
{
    if (!thermoDict.containsDataEntry("radCut"))
    {
        fatalErrorInFunction
            << "Parameter 'radCut' is mandatory when enableRadiation "
            << "is true.\nPlease add it to the thermoPhysicalInteraction "
            << "dictionary." << endl;
        fatalExit;
    }
    radCut_ = thermoDict.getVal<real>("radCut");

    radUpdateInterval_ = thermoDict.getValOrSet<uint32>(
        "radUpdateInterval",
        1);

    if (radUpdateInterval_ == 0)
    {
        fatalErrorInFunction
            << "'radUpdateInterval' must be a positive integer, got 0."
            << endl;
        fatalExit;
    }
}

void thermalRadiationMechanism::ensureMemory(size_t numParticles)
{
    if (radSumTemp_.extent(0) != numParticles)
    {
        Kokkos::resize(radSumTemp_, numParticles);
        Kokkos::resize(radNumPrt_,  numParticles);
    }
}

} // pFlow
