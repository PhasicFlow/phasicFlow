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

#include "positionOrdered.hpp"
#include "error.hpp"


#include "streams.hpp"


bool pFlow::positionOrdered::findAxisIndex()
{
	if( axisOrder_.size() != 3 )
	{
		fatalErrorInFunction <<
		"axisOrder should have 3 components, but " << axisOrder_.size() << 
		" has been provided. \n";
		return false;
	}


	if( axisOrder_[0] == axisOrder_[1] || 
		axisOrder_[0] == axisOrder_[2] ||
		axisOrder_[1] == axisOrder_[2] )
	{
		fatalErrorInFunction<<
		"invalid/repeated axis names in axisOrder. This is provided: " << axisOrder_ <<endl;
		return false;
	}

	realx3 uV[3];
	size_t i=0;
	for(auto& ca: axisOrder_)
	{
		if(ca == "x")
		{
			uV[i] = realx3(1.0, 0.0, 0.0);
		}
		else if(ca == "y")
		{
			uV[i] = realx3(0.0, 1.0, 0.0);
		}
		else if(ca == "z")
		{
			uV[i] = realx3(0.0, 0.0, 1.0);
		}
		else
		{
			fatalErrorInFunction << 
			"unknown name for axis in axisOrder: " << ca <<endl; 
			return false;
		}
		i++;
	}

	uVector1_ = uV[0];
	uVector2_ = uV[1];
	uVector3_ = uV[2];

	return true;
}

bool pFlow::positionOrdered::positionPointsOrdered()
{
	position_.clear();

	realx3 dl(diameter_);
	auto minP =  region_->minPoint();
	auto maxP =  region_->maxPoint();

	auto cntr = minP;

	size_t n = 0;
	while( n < numPoints_ )
	{
		if(region_->isInside(cntr))
		{
			position_.push_back(cntr);
			n++;
		}

		cntr += dl*uVector1_;
		
		if( dot(uVector1_, cntr) > dot(uVector1_, maxP) )
		{
			cntr = (minP*uVector1_) + ( (cntr+dl) * uVector2_) + (cntr*uVector3_);

			if( dot(uVector2_, cntr) > dot(uVector2_, maxP) )
			{
				cntr = (cntr*uVector1_) + (minP*uVector2_) + ((cntr+dl)*uVector3_);

				if( dot(uVector3_,cntr) > dot(uVector3_, maxP) )
				{
					fatalErrorInFunction<<
					"positioned " << n << " points in the domain and it is full. \n" <<
					"request to position "<< numPoints_<< " points has failed.\n";
					return false;
				}
			}
		}
		
	}

	return true;
}

pFlow::positionOrdered::positionOrdered
(
	systemControl& control,
	const dictionary& dict
)
:
	positionParticles(control, dict),
	poDict_
	(  
		dict.subDict("positionOrderedInfo")
	),
	diameter_
	(
		poDict_.getVal<real>("diameter")
	),
	numPoints_
	(
		poDict_.getVal<uint64>("numPoints")
	),
	axisOrder_
	(
		poDict_.getValOrSet("axisOrder", wordList{"x", "y", "z"})
	),
	position_
	(
		"positionOrdered", maxNumberOfParticles_, numPoints_ ,RESERVE()
	)
{
	
	if( !findAxisIndex() )
	{
		fatalExit;
	}

	if(!region_)
	{
		fatalErrorInFunction<<"You must provided a region (box, cylinder, ...) for positioning particles in dictionary "<<
		dict.globalName()<<endl;
		fatalExit;
	}

	if(!positionPointsOrdered())
	{
		fatalExit;
	}
}