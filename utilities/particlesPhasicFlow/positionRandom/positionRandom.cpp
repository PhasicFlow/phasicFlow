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


#include "positionRandom.hpp"
#include "collisionCheck.hpp"



bool pFlow::positionRandom::positionOnePass(collisionCheck& collCheck)
{

	auto const& region = pRegion();
	
	auto n = static_cast<uint32>(position_.size());

	for(uint32 iter = 0; iter<numPoints_; iter++)
	{
		realx3 p = region.peek();

		if( collCheck.checkPoint(p, diameter_) )
		{
			position_.push_back(p);
			diameters_.push_back(diameter_);
			
			if(!collCheck.mapLastAddedParticle())
			{
				fatalErrorInFunction;
				return false;
			}
			n++;

			if(n == numPoints_) break;
		}
	}
	
	return true;	
}

bool pFlow::positionRandom::positionPointsRandom()
{
	
	position_.clear();
	diameters_.clear();
	if(numPoints_ == 0)return true;
	
	uint32 pass = 0;
	collisionCheck collCheck(
		box(pRegion().minPoint(), pRegion().maxPoint()),
		diameter_,
		position_,
		diameters_);
	

	while ( pass <maxIterations_)
	{
		if( !positionOnePass(collCheck) )
		{
			return false;
		}
		pass++;
		REPORT(1)<<"Positioning "<< Green_Text("(Pass #"<< pass<<")")<<
		": number of non-colliding spheres is "<<
		Yellow_Text(position_.size())<<END_REPORT;

		if( position_.size() == numPoints_ ) 
		{
			REPORT(0)<<END_REPORT;
			return true;
		}
	}	
	
	fatalErrorInFunction<< 
	"  Cannot position "<< numPoints_ << " in the domain in " <<
	maxIterations_ << " iterations.\n" <<
	"  you may increase maxIterations for positioning points.\n";

	return false;
}


pFlow::positionRandom::positionRandom
(
	systemControl& control,
	const dictionary& dict
)
:
	positionParticles(control, dict),
	prDict_
	(  
		dict.subDict("randomInfo")
	),
	diameter_
	(
		prDict_.getVal<real>("diameter")
	),
	numPoints_
	(
		prDict_.getVal<uint32>("numPoints")
	),
	maxIterations_
	(
		prDict_.getValOrSet("maxIterations", 10u)
	),
	position_
	(
		"position",
		1,
		0,
		RESERVE()
	),
	diameters_
	(
		"diameters",
		1,
		0,
		RESERVE()
	)
{

	reportInterval_ = max(numPoints_/numReports_, static_cast<uint32>(2));

	if( !positionPointsRandom() )
	{
		fatalExit;
	}

}


