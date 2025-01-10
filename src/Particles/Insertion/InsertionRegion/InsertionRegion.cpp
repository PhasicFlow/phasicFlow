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
	wordVector& 	names,
	realx3Vector& 	positions,
	bool& insertionOccured
)
{
	insertionOccured = false;

	if(!insertionTime(iter, t, dt)) return true;

	uint32 newNum = numberToBeInserted(iter, t, dt);
	if(newNum == 0) return true;

	// get the internal box 
	auto internalBox =  pStruct().internalDomainBox();
	if( !(internalBox.minPoint() < internalBox.maxPoint()))
	{
		WARNING<<"Minimum point of internal point is not lower than "<<
		"the maximum point \n"<<
		"minimum point: "<< internalBox.minPoint()<<
		"\nmaximum point:"<<internalBox.maxPoint()<<END_WARNING;

		return false;
	}

	

	/// reserve enough space 
	names.reserve(max(newNum,names.capacity()));
	names.clear();
	
	positions.reserve(max(newNum,positions.capacity()));
	positions.clear();

	auto allPositions = realx3Vector("allPositions");
	auto allDiameters = realVector("allDiameters");

	auto& mix = mixture();
	auto& pReg = pRegion();

	real maxDiam = shapes_.maxBoundingSphere();
	auto minP = pReg.minPoint() - maxDiam;
	auto maxP = pReg.maxPoint() + maxDiam;
	
	if(Insertion().checkForCollision())
	{
		// check for collision with already inserted particles 	
		// so, use selector to select particles in the simulation 
		auto bDict = dictionary("boxInfo");
		bDict.add("min", minP);
		bDict.add("max", maxP);
		
		auto selector = pStructSelector::create(
			"box", 
			pStruct(), 
			bDict);
		
		allPositions = selector().selectedPointPositions();
		allDiameters = selectedFieldVals<real>
		(
			selector(), 
			Insertion().diameterName()
		);
	}
	
	auto collCheck = collisionCheck( 
		{minP, maxP}, 
		maxDiam,
		allPositions,
		allDiameters);

	uint32 numInserted = 0;

	uint32 idx;
	word name = mix.getNextShapeName();
	shapes_.shapeNameToIndex(name, idx);
	real d = shapes_.boundingDiameter(idx);

	for(uint32 i=0; i< 100*newNum ; ++i)
	{
		realx3 p = pReg.peek();
		// check if point is inside internal box 
		if(!internalBox.isInside(p))continue;

		if( collCheck.checkPoint( p, d) )
		{
			names.push_back(name);
			positions.push_back(p);
			numInserted++;
			if( numInserted == newNum ) break;
			
			// add this new particle to collision check set 
			allDiameters.push_back(d);
			allPositions.push_back(p);
			collCheck.mapLastAddedParticle();
							
			// obtain next shape name and diameter 
			name = mix.getNextShapeName();
			shapes_.shapeNameToIndex(name, idx);
			d = shapes_.boundingDiameter(idx);
		}
		
	}
	
	insertionOccured = true;
	addToNumInserted(numInserted);
	return numInserted == newNum;
}



/*if(!checkForCollision)
	{
		realVector diams("diams", newNum, 0, RESERVE());
		
		uint32 idx;
		word name = mix.getNextShapeName();
		shapes_.shapeNameToIndex(name, idx);
		real d = shapes_.boundingDiameter(idx);	

		for(uint32 i=0; i< 100*newNum ; ++i)
		{
			realx3 p = pReg.peek();
			// check if point is inside internal box 
			if(!internalBox.isInside(p))continue;

			if( !checkForContact(positions, diams, p, d) )
			{
				names.push_back(name);
				positions.push_back(p);
				diams.push_back(d);
				numInserted++;

				if( numInserted == newNum ) break;
				
				name = mix.getNextShapeName();
				shapes_.shapeNameToIndex(name, idx);
				d = shapes_.boundingDiameter(idx);
			}
		}
	}
	else
	{
		real maxDiam = shapes_.maxBoundingSphere();
		auto minP = pReg.minPoint() - maxDiam;
		auto maxP = pReg.maxPoint() + maxDiam;
		auto bDict = dictionary("boxInfo");
		bDict.add("min", minP);
		bDict.add("max", maxP);
		auto selector = pStructSelector::create(
			"box", 
			pStruct(), 
			bDict);
			
		auto allPositions = selector().selectedPointPositions();
		auto allDiameters = selectedFieldVals<real>(selector(), "diameter");
		auto collCheck = collisionCheck(
			{minP, maxP},
			maxDiam,
			allPositions,
			allDiameters);
		
		uint32 idx;
		word name = mix.getNextShapeName();
		shapes_.shapeNameToIndex(name, idx);
		real d = shapes_.boundingDiameter(idx);

		for(uint32 i=0; i< 100*newNum ; ++i)
		{
			realx3 p = pReg.peek();
			// check if point is inside internal box 
			if(!internalBox.isInside(p))continue;

			if( collCheck.checkPoint( p, d) )
			{
				names.push_back(name);
				positions.push_back(p);
				numInserted++;
				if( numInserted == newNum ) break;
				
				// add this new particle to collision check set 
				allDiameters.push_back(d);
				allPositions.push_back(p);
				collCheck.mapLastAddedParticle();
								
				// obtain next shape name and diameter 
				name = mix.getNextShapeName();
				shapes_.shapeNameToIndex(name, idx);
				d = shapes_.boundingDiameter(idx);
			}
			
		}
	}*/