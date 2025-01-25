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

#include "NBS.hpp"

pFlow::NBS::NBS
(
    const dictionary& dict,
	const box& domainBox,
	real  	minBSSize,
	real 	maxBSSize,
	const deviceViewType1D<realx3> &position, 
	const pFlagTypeDevice &flags, 
	const deviceViewType1D<real> &diam, 
	uint32 					nWallPoints,
	uint32 					nWallElements,
	const ViewType1D<realx3,memory_space>& 		wallPoints, 
	const ViewType1D<uint32x3,memory_space>& 	wallVertices,
  const ViewType1D<realx3,memory_space>& 		wallNormals 
)
: 
    particleWallContactSearchs<NBS>(
        dict,
        domainBox,
        minBSSize,
        maxBSSize,
        position,
        flags,
        diam),
    sizeRatio_(max(dict.getVal<real>("sizeRatio"), one)),
    cellExtent_(max(dict.getVal<real>("cellExtent"), half)),
    adjustableBox_(dict.getVal<Logical>("adjustableBox")),
    NBSLevel0_
	(
        this->domainBox_,
        maxBSSize,
        sizeRatio_,
        position,
        flags,
        adjustableBox_()
	),
	cellsWallLevel0_
	(
		cellExtent_,
		nWallPoints,
		nWallElements,
		wallPoints,
		wallVertices,
    wallNormals
	)
{
}
