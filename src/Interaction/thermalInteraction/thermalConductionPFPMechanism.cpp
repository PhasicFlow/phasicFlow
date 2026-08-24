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

#include "thermalConductionPFPMechanism.hpp"

namespace pFlow
{

thermalConductionPFPMechanism::thermalConductionPFPMechanism(
    const dictionary&   thermoDict,
    bool                enableConduction,
    bool                enablePFP)
:
    enableConduction_(enableConduction),
    enablePFP_(enablePFP)
{
    if (thermoDict.containsDataEntry("simYoungsModulus"))
    {
        simYoungsModulus_ = thermoDict.getVal<real>("simYoungsModulus");
    }
    else
    {
        fatalErrorInFunction
            << "Parameter 'simYoungsModulus' is mandatory when "
            << "enableConduction or enablePFP is true.\n"
            << "Please add it to the thermoPhysicalInteraction dictionary."
            << endl;
        fatalExit;
    }
}

real thermalConductionPFPMechanism::requiredSearchCut(
    real maxBoundingSphere) const
{
    real cut = 0.0;

    if (enableConduction_)
    {
        // Largest possible contact distance R_i+R_j between any two
        // particles in the case.
        cut = max(cut, 2.0 * maxBoundingSphere);
    }

    if (enablePFP_)
    {
        cut = max(cut, 3.0 * maxBoundingSphere);
    }

    return cut;
}

} // pFlow
