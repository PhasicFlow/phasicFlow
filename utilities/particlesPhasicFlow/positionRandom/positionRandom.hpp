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


#ifndef __positionOrdered_hpp__
#define __positionOrdered_hpp__

#include "positionParticles.hpp"
#include "VectorSingles.hpp"
#include "VectorDuals.hpp"


namespace pFlow
{



class positionRandom
:
	public positionParticles
{
protected:

	dictionary 	prDict_;

	real 		diameter_;

	size_t 		numPoints_;

	size_t 		maxIterations_;
	
	realx3Vector 	position_;

	size_t 		reportInterval_;

	bool positionOnePass(int32 pass, int32 startNum);
	
	bool positionPointsRandom();

	bool inCollision(const realx3 &cntr, real diam);

	void fillPoints(
		uint numPoints,
		realx3Vector_HD& points,
		int32Vector_HD& flags );

public:

	// - type Info
	TypeInfo("positionRandom");

	positionRandom(
		systemControl& control,
		const dictionary& dict);

	// - add this class to vCtor selection table 
	add_vCtor(
		positionParticles,
		positionRandom,
		dictionary);

	virtual ~positionRandom() = default;

	//// - Methods 

	virtual uint64 numPoints()const
	{
		return position_.size();
	}

	virtual uint64 size()const
	{
		return position_.size();
	}

	real maxDiameter() const override
	{
		return diameter_;
	}

	// - const access to position
	virtual const realx3Vector& position()const 
	{
		return position_;
	}

	// - access to position 
	virtual realx3Vector& position()
	{
		return position_;
	}



	
};	


}



#endif // __positionOrdered_hpp__
