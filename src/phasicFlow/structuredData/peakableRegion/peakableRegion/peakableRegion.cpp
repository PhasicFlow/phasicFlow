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


template<typename RegionType>
pFlow::PeakableRegion<RegionType>::PeakableRegion
(
	const word& type, 
	const dictionary& dict
)
:
	peakableRegion(type, dict),
	region_(dict)
{
	
}

template<typename RegionType>
bool  pFlow::PeakableRegion<RegionType>::isInside
(
	const realx3& point
)const
{
	return region_.isInside(point);
}


template<typename RegionType>
pFlow::realx3 pFlow::PeakableRegion<RegionType>::peek()const
{
	return region_.peek();
}
	
template<typename RegionType>
bool pFlow::PeakableRegion<RegionType>::read(const dictionary& dict)
{
	return region_.read(dict);
}

template<typename RegionType>
bool pFlow::PeakableRegion<RegionType>::write(dictionary& dict)const
{
	return region_.write(dict);
}


