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


template <typename method>
pFlow::particleWallContactSearchs<method>::particleWallContactSearchs
(
    const dictionary &dict, 
    const box &domain, 
    real minSize, 
    real maxSize, 
    const ViewType1D<realx3, memory_space> &position,
    const pFlagTypeDevice &flags,  
    const ViewType1D<real, memory_space> &diam
)
:
    domainBox_(domain),
    updateInterval_
    (
        max(dict.getValOrSet<uint32>("updateInterval", 1),1u)
    )
{
    
}

template <typename method>
bool pFlow::particleWallContactSearchs<method>::broadSearch
	(
		uint32 iter,
		real t,
		real dt,
		csPairContainerType& ppPairs,
		csPairContainerType& pwPairs,
		const deviceViewType1D<realx3>& pointPos,
		const pFlagTypeDevice& flags,
		const deviceViewType1D<real>& diameter,
		bool  force
	)
	{
		
		performedSearch_ = false;
		if( !performSearch(iter, force) ) return true;

		if(!getMethod().impl_broadSearch(
			ppPairs,
			pwPairs,
			pointPos,
			flags,
			diameter))
		{
			fatalErrorInFunction<<
			"Error in performing particle-particle broadSearch in method"<<
			getMethod().typeName()<<endl;
			return false;
		}

		lastUpdated_ = iter;
		
		performedSearch_ = true;		
		return true;
	}