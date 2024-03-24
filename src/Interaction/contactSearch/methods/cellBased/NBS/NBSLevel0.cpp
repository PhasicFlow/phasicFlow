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

#include "NBSLevel0.hpp"
#include "streams.hpp"

bool pFlow::NBSLevel0::findPairs
(
	csPairContainerType &pairs, 
	const deviceViewType1D<realx3> &pointPos, 
	const pFlagTypeDevice &flags, 
	const deviceViewType1D<real> &diameter
)
{
	uint32 getFull = 1;
	
	// loop until the container size fits the numebr of contact pairs
	while (getFull > 0)
	{

		getFull = pFlow::NBSLevel0Kernels::findPairsCount
		(
			pairs,
			sizeRatio_,
			pointPos,
			flags,
			diameter, 
			getCellIterator()
		);
	
		if(getFull)
		{
			// - resize the container
			//   note that getFull now shows the number of failed insertions.
			uint32 len = max(getFull,500u) ;
			
			auto oldCap = pairs.capacity();
			
			pairs.increaseCapacityBy(len);

			INFORMATION<< "The contact pair container capacity increased from "<<
			oldCap << " to "<<pairs.capacity()<<" in NBSLevel0."<<END_INFO;
			
		}

		Kokkos::fence();
	}
	
	return true;
}


pFlow::NBSLevel0::NBSLevel0
(
    const 	box& domain,
    real 	cellSize,
    real  sizeRatio,
    const deviceViewType1D<realx3>& pointPos,
    const pFlagTypeDevice& flags,
    bool 	adjustableBox
)
:
    mapperNBS
    (
        domain,
        cellSize,
        pointPos,
        flags,
        adjustableBox,
        true
    ),
    sizeRatio_(sizeRatio)
{
    
}

bool pFlow::NBSLevel0::broadSearch
(
	csPairContainerType &pairs, 
	const deviceViewType1D<realx3> &pointPos, 
	const pFlagTypeDevice &flags, 
	const deviceViewType1D<real> &diameter,
	bool& searchBoxChanged
)
{	
	if(!build(pointPos, flags, searchBoxChanged))
	{
		fatalErrorInFunction;
		return false;
	}

	if(!findPairs(pairs, pointPos, flags, diameter))
	{
		fatalErrorInFunction;
		return false;
	}

	return true;
}


