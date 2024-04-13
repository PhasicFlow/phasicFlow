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
	const word& name,
	const insertion& instn, 
	const ShapeType& shapes
)
:
	insertionRegion(name, instn),
	shapes_(shapes)
{}


template<typename ShapeType>
bool pFlow::InsertionRegion<ShapeType>::insertParticles
(
	uint32 iter, 
	real t,
	real dt,
	wordVector& names,
	realx3Vector& pos,
	bool& insertionOccured
)
{
	insertionOccured = false;

	if(!insertionTime(iter, t, dt)) return true;

	uint32 newNum = numberToBeInserted(iter, t, dt);
	
	if(newNum == 0) return true;

	names.reserve(max(newNum,names.capacity()));
	pos.reserve(max(newNum,pos.capacity()));
	names.clear();
	pos.clear();

	realVector diams("diams", newNum, 0, RESERVE());

	for(uint32 i=0; i<newNum; i++)
	{
		uint32 idx;
		shapes_.shapeNameToIndex(names[i], idx);
		diams[i] = shapes_.boundingDiameter(idx);
	}

	size_t n = 0;
	uint32 idx;
	auto& mix = mixture();
	auto& pReg = pRegion();
	word name = mix.getNextShapeName();
	shapes_.shapeNameToIndex(name, idx);
	real d = shapes_.boundingDiameter(idx);

	for(uint32 i=0; i< 100*newNum ; ++i)
	{
		
		realx3 p = pReg.peek();
		if( !checkForContact(pos, diams, p, d) )
		{
			names.push_back(name);
			pos.push_back(p);
			diams.push_back(d);
			n++;

			if( n == newNum ) 
			{
				addToNumInserted(newNum);
				insertionOccured = true;
				return true;
			}

			name = mix.getNextShapeName();
			shapes_.shapeNameToIndex(name, idx);
			d = shapes_.boundingDiameter(idx);
		}

	}

	addToNumInserted(n);
	insertionOccured = true;
	return false;
}