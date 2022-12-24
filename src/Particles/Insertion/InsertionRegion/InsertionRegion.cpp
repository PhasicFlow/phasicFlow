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

template<typename ShapeType>
bool pFlow::InsertionRegion<ShapeType>::checkForContact
(
	const realx3Vector& pos,
	const realVector& diams,
	const realx3& p,
	const real& d
)
{

	ForAll(i, pos)
	{
		if( length(pos[i]-p) < 0.5*(diams[i]+d) ) return true;
	}

	return false;
}

template<typename ShapeType>
pFlow::InsertionRegion<ShapeType>::InsertionRegion
(
	const dictionary& dict,
	const ShapeType& shapes
)
:
	insertionRegion(dict),
	shapes_(shapes)
{

}


template<typename ShapeType>
bool pFlow::InsertionRegion<ShapeType>::insertParticles
(
	real currentTime,
	real dt,
	wordVector& names,
	realx3Vector& pos,
	bool& insertionOccured
)
{
	insertionOccured = false;

	if(!insertionTime( currentTime, dt)) return true;

	size_t newNum = numberToBeInserted(currentTime);
	
	if(newNum == 0) return true;

	names.reserve(max(newNum,names.capacity()));
	pos.reserve(max(newNum,pos.capacity()));
	names.clear();
	pos.clear();

	realVector diams(newNum, RESERVE());

	mixture_->getNextShapeNameN(newNum, names);

	if(!shapes_.shapeToDiameter(names,diams))
	{
		fatalErrorInFunction<<
		"  error occured in insertion region "<< name() << 
		" while converting shapes to diameter. \n";
		return false;
	}

	size_t n = 0;

	for(label iter=0; iter< 10*newNum ; ++iter)
	{
		if( !(n < newNum) ) 
		{
			addToNumInserted(newNum);
			insertionOccured = true;
			return true;
		}
		realx3 p = pRegion_().peek();
		real d = diams[pos.size()];
		if( !checkForContact(pos, diams, p, d) )
		{
			pos.push_back(p);
			n++;
		}

	}

	fatalErrorInFunction<<
	"  Cannot insert "<< newNum << " new particles from region "<< name()<<". \n"
	"  pFlow could position only "<< n<< " particles in this region. \n";
	addToNumInserted(n);
	insertionOccured = false;
	return false;

}