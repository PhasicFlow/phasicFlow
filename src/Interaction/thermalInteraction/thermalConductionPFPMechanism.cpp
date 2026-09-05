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
    bool                enablePP,
    bool                enablePFP)
:
    enablePP_(enablePP),
    enablePFP_(enablePFP)
{
    simYoungsModulus_ = thermoDict.getVal<real>("simYoungsModulus");
}

real thermalConductionPFPMechanism::requiredSearchCut(
    real maxBoundingSphere) const
{
    real cut = 0.0;

    if (enablePP_)
    {
        cut = max(cut, 2.0 * maxBoundingSphere);
    }

    if (enablePFP_)
    {
        cut = max(cut, 3.0 * maxBoundingSphere);
    }

    return cut;
}

void thermalConductionPFPMechanism::iterate(
    const pFlagTypeDevice&          flags,
    const deviceViewType1D<realx3>& pos,
    const deviceViewType1D<real>&   diameter,
    const deviceViewType1D<real>&   temperature,
    const deviceViewType1D<real>&   K,
    const deviceViewType1D<real>&   E0,
    const deviceViewType1D<real>&   nu,
    const deviceViewType1D<real>&   fluidKappa,
    const deviceViewType1D<real>&   fluidAlpha,
    const mapperNBS&                mapper,
    deviceViewType1D<real>          Q_pp,
    deviceViewType1D<real>          Q_pfp) const
{
    using namespace thermalConductionPFPMechanismKernels;

    if (enablePP_)
    {
        if (enablePFP_)
        {
            sweep<true, true>(
                flags, pos, diameter, temperature, K, E0, nu,
                fluidKappa, fluidAlpha, mapper,
                simYoungsModulus_, Q_pp, Q_pfp);
        }
        else
        {
            sweep<true, false>(
                flags, pos, diameter, temperature, K, E0, nu,
                fluidKappa, fluidAlpha, mapper,
                simYoungsModulus_, Q_pp, Q_pfp);
        }
    }
    else
    {
        if (enablePFP_)
        {
            sweep<false, true>(
                flags, pos, diameter, temperature, K, E0, nu,
                fluidKappa, fluidAlpha, mapper,
                simYoungsModulus_, Q_pp, Q_pfp);
        }
        else
        {
            sweep<false, false>(
                flags, pos, diameter, temperature, K, E0, nu,
                fluidKappa, fluidAlpha, mapper,
                simYoungsModulus_, Q_pp, Q_pfp);
        }
    }
}

} // pFlow
