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

#ifndef __positionOrdered_H__
#define __positionOrdered_H__

#include "positionParticles.H"

namespace pFlow
{


class positionOrdered 
:
	public positionParticles
{
protected:

	dictionary 	poDict_;

	real 		diameter_;

	size_t 		numPoints_;
	
	wordList 	axisOrder_;

	// - unit vector of the first axis
	realx3 		uVector1_;

	// - unit vector of the second axis
	realx3 		uVector2_;

	// - unit vector of the third axis
	realx3 		uVector3_;


	realx3Vector 	position_;

	bool findAxisIndex();

	bool positionPointsOrdered();

public:

	// - type Info
	TypeName("positionOrdered");

	positionOrdered(const dictionary& dict);

	// - add this class to vCtor selection table 
	add_vCtor(
		positionParticles,
		positionOrdered,
		dictionary);

	virtual ~positionOrdered() = default;

	//// - Methods 

	virtual label numPoints()const
	{
		return position_.size();
	}

	virtual label size()const
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



#endif // __positionOrdered_H__
