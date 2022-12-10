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
#include "box.hpp"
#include "cylinder.hpp"
#include "sphere.hpp"
#include "cells.hpp"
#include "contactSearchFunctions.hpp"

#include "streams.hpp"


pFlow::realx3Vector pFlow::positionParticles::sortByMortonCode(realx3Vector& position)const
{
	struct indexMorton
	{
		size_t morton;
		size_t index;
	};

	realx3 minP = min(position);
	realx3 maxP = max(position);
	real cellsize = maxDiameter();
	cells<size_t> allCells( box(minP, maxP), cellsize);

	Vector<indexMorton> indMor(position.size(),RESERVE());

	indMor.clear();

	size_t ind=0;
	for(const auto& p:position)
	{
		auto cellInd = allCells.pointIndex(p);
		indMor.push_back( 
			{ xyzToMortonCode64(cellInd.x(), cellInd.y(), cellInd.z()),
			ind++});
	}

	Info<<"Performing morton sorting."<<endInfo;
	std::sort(
		indMor.begin(),
		indMor.end(),
		[]( const indexMorton &lhs, const indexMorton &rhs){
			return lhs.morton < rhs.morton; } );

	realx3Vector sortedPos(position.capacity(), RESERVE());
	sortedPos.clear();


	for(auto& ind:indMor)
	{
		sortedPos.push_back( position[ind.index] );
	}

	return sortedPos;
}


pFlow::positionParticles::positionParticles
(
	const dictionary& dict
)
{
	maxNumberOfParticles_ = dict.getValOrSet("maxNumberOfParticles", static_cast<size_t>(10000));
	
	mortonSorting_ = dict.getValOrSet("mortonSorting", Logical("Yes"));

	if( dict.containsDictionay("box") )	
	{
		region_ = makeUnique<region<box>>(dict.subDict("box"));
	}
	else if(dict.containsDictionay("cylinder"))
	{
		region_ = makeUnique<region<cylinder>>(dict.subDict("cylinder"));
	}
	else if(dict.containsDictionay("sphere"))
	{
		region_ = makeUnique<region<sphere>>(dict.subDict("sphere"));
	}
}




pFlow::uniquePtr<pFlow::positionParticles> pFlow::positionParticles::create(const dictionary & dict)
{

	word method = dict.getVal<word>("method");


	if( dictionaryvCtorSelector_.search(method) )
	{
		return dictionaryvCtorSelector_[method] (dict);
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
