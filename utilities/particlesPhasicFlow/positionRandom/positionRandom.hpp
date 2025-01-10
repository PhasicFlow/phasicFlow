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
#ifndef __positionRandom_hpp__
#define __positionRandom_hpp__

#include "positionParticles.hpp"
#include "dictionary.hpp"

namespace pFlow
{

class collisionCheck;

class positionRandom : public positionParticles
{
private:

	dictionary   prDict_;

	real         diameter_;

	uint32       numPoints_;

	uint32       maxIterations_;

	realx3Vector position_;

	realVector   diameters_;

	uint32       reportInterval_;

	bool         positionOnePass(collisionCheck& collCheck);

	bool         positionPointsRandom();

public:

	// - type Info
	TypeInfo("random");

	positionRandom(systemControl& control, const dictionary& dict);

	// - add this class to vCtor selection table
	add_vCtor
	(
		positionParticles, 
		positionRandom, 
		dictionary
	);

	~positionRandom() final = default;

	//// - Methods

	uint32 numPoints() const final
	{
		return position_.size();
	}

	uint32 size() const final
	{
		return position_.size();
	}

	real maxDiameter() const override
	{
		return diameter_;
	}

	// - const access to position
	const realx3Vector& position() const final
	{
		return position_;
	}

	// - access to position
	realx3Vector& position() final
	{
		return position_;
	}
};

}

#endif // __positionOrdered_hpp__
