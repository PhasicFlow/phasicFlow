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

#ifndef __twoPartContactSearch_hpp__
#define __twoPartContactSearch_hpp__

#include "contactSearchGlobals.hpp"
#include "scatteredFieldAccess.hpp"
#include "cells.hpp"
#include "VectorSingles.hpp"

namespace pFlow
{

class twoPartContactSearch
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
	twoPartContactSearch(
		const box &domain,
		real cellSize,
		real sizeRatio = 1.0);

	/// @brief Perform a broad-search for spheres in two adjacent regions.
	/// Region 1 is considered as the master (primary) region and region 2 as slave 
	/// @param ppPairs pairs container which holds i and j
	/// @param points1 point positions in region 1
	/// @param diams1  diameter of spheres in region 1
	/// @param points2 point positions in region 2
	/// @param diams2 diameter of spheres in region 2
	/// @param transferVec a vector to transfer points from region 2 to region 1
	/// @return true if it is successful 
	bool broadSearchPP(
		csPairContainerType &ppPairs,
		const deviceScatteredFieldAccess<realx3> &points1,
		const deviceScatteredFieldAccess<real> &diams1,
		const deviceScatteredFieldAccess<realx3> &points2,
		const deviceScatteredFieldAccess<real> &diams2,
		const realx3 &transferVec);
	
	bool broadSearchPP(
		csPairContainerType &ppPairs,
		const deviceScatteredFieldAccess<realx3> &points1,
		const deviceScatteredFieldAccess<real> &diams1,
		const realx3Vector_D& points2,
		const realVector_D& diams2,
		const word& name);

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

#endif //__twoPartContactSearch_hpp__