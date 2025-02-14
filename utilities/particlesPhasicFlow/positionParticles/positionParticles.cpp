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

#include "positionParticles.hpp"
#include "vocabs.hpp"
#include "dictionary.hpp"
#include "systemControl.hpp"

pFlow::realx3Vector pFlow::positionParticles::sortByMortonCode(
	const realx3Vector& position)const
{
	struct indexMorton
	{
		uint64 morton;
		uint64 index;
	};

    WARNING<<"Morton sorting is inactive!"<<END_WARNING;
	return position;
}

pFlow::positionParticles::positionParticles
(
	systemControl& control,
	const dictionary& dict
)
:
	regionType_(dict.getValOrSet<word>("regionType", "domain")),
	mortonSorting_(dict.getValOrSet("mortonSorting", Logical("Yes")))
{
	if( regionType_ != "domain" )	
	{
		pRegion_ = peakableRegion::create(
			regionType_,
			dict.subDict(regionType_+"Info"));
	}	
	else
	{
		fileDictionary domainDictionary
		(
			objectFile
			{
				domainFile__,
				"",
				objectFile::READ_ALWAYS,
				objectFile::WRITE_NEVER
			},
			&control.settings()
		);
		pRegion_ = peakableRegion::create("box", domainDictionary.subDict("globalBox"));
	}
}

pFlow::realx3Vector pFlow::positionParticles::getFinalPosition()
{
	if(mortonSorting_)
	{
		return sortByMortonCode(position());
	}
	else
	{
		realx3Vector vec("final position",position().capacity(), 0 , RESERVE());
		vec.assign( position().begin(), position().end());	
		return vec;
	}
}

pFlow::uniquePtr<pFlow::positionParticles> 
	pFlow::positionParticles::create
(
    systemControl& control,
    const dictionary & dict
)
{
	word method = dict.getVal<word>("method");

	if( dictionaryvCtorSelector_.search(method) )
	{
		return dictionaryvCtorSelector_[method] (control, dict);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< method << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}

	return nullptr;
}
