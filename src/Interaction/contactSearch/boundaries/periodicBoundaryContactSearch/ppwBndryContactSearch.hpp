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

#ifndef __ppwBndryContactSearch_hpp__
#define __ppwBndryContactSearch_hpp__

#include "contactSearchGlobals.hpp"
#include "scatteredFieldAccess.hpp"
#include "cells.hpp"

namespace pFlow
{

class ppwBndryContactSearch
{
public:
	using HeadType = deviceViewType3D<uint32>;

	using NextType = deviceViewType1D<uint32>;

private:

	cells    searchCells_;

	HeadType head_{ "periodic::head", 1, 1, 1 };

	NextType next_{ "periodic::next", 1 };

	real     sizeRatio_ = 1.0;

	uint32   nextCapacity_ = 0;

	void checkAllocateNext(uint32 n);

	void nullifyHead();

	void nullifyNext(uint32 n);

	void buildList(
		const deviceScatteredFieldAccess<realx3> &points);

public:
	ppwBndryContactSearch(
		const box &domain,
		real cellSize,
		real sizeRatio = 1.0);

	bool broadSearchPP(
		csPairContainerType &ppPairs,
		const deviceScatteredFieldAccess<realx3> &points,
		const deviceScatteredFieldAccess<real> &diams,
		const deviceScatteredFieldAccess<realx3> &mirrorPoints,
		const deviceScatteredFieldAccess<real> &mirrorDiams,
		const realx3 &transferVec);

	const auto& searchCells()const
	{
		return searchCells_;
	}

	real sizeRatio()const
	{
		return sizeRatio_;
	}
};

}

#endif //__ppwBndryContactSearch_hpp__