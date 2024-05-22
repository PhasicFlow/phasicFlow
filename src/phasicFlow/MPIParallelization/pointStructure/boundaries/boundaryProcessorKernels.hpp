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

#include "phasicFlowKokkos.hpp"
#include "infinitePlane.hpp"
#include "scatteredFieldAccess.hpp"

namespace pFlow::boundaryProcessorKernels
{

struct markNegative
{
    markNegative(const infinitePlane& pl,
	const deviceViewType1D<uint32>& f,
	const deviceScatteredFieldAccess<realx3>& p
	)
	:
		plane_(pl),
		flags_(f),
		points_(p)
	{}
	
	infinitePlane        plane_;
    deviceViewType1D<uint32> flags_;
    deviceScatteredFieldAccess<realx3> points_;

	INLINE_FUNCTION_HD
    void operator()(uint32 i, uint32& transferToUpdate)const
    {
        if(plane_.pointInNegativeSide(points_(i)))
		{
			flags_(i)=1;
			transferToUpdate++;
		}
    }

};

}