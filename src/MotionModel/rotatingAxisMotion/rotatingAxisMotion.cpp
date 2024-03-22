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

#include "rotatingAxisMotion.hpp"

void pFlow::rotatingAxisMotion::impl_setTime
(
	uint32 iter, 
	real t, 
	real dt
)const
{
	auto motion = motionComponents_.deviceViewAll();
	Kokkos::parallel_for(
		"rotatingAxisMotion::impl_setTime",
		deviceRPolicyStatic(0, numComponents_),
		LAMBDA_HD(uint32 i){
			motion[i].setTime(t);
		});
	Kokkos::fence();
}

pFlow::rotatingAxisMotion::rotatingAxisMotion(
    const objectFile &objf,
    repository *owner)
    : fileDictionary(objf, owner)
{

	if(! impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

pFlow::rotatingAxisMotion::rotatingAxisMotion
(
	const objectFile &objf, 
	const dictionary &dict, 
	repository *owner
)
:
	fileDictionary(objf, dict, owner)
{
	if(!impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

bool pFlow::rotatingAxisMotion::write
(
	iOstream &os, 
	const IOPattern &iop
) const
{
	// a global dictionary
	dictionary newDict(fileDictionary::dictionary::name(), true);
	if( iop.thisProcWriteData() )
	{
		if( !this->impl_writeDictionary(newDict) || 
			!newDict.write(os))
		{
			fatalErrorInFunction<<
			" error in writing to dictionary "<< newDict.globalName()<<endl;
			return false;
		}
	}
    return true;
}
