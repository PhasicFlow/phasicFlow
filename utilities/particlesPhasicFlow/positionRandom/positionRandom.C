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


#include "positionRandom.H"
#include "uniformRandomReal.H"
#include "NBS.H"
#include "unsortedPairs.H"
#include "box.H"



namespace pFlow
{

using SearchType = NBS<DefaultExecutionSpace, int32> ;
using ContainerType = unsortedPairs<DefaultExecutionSpace, int32>;



int32 findCollisions(
		ContainerType& pairs,
		int32Vector_HD& flags);

int32 findCollisions(int32 num, realx3Vector_HD& points, real diam)
{
	int32 res =0;
	for(auto i=0; i<num;i++)
	{
		for(auto j=i+1; j<num; j++)
		{
			if(sphereSphereCheck(points[i],points[j],diam,diam))res++;
		}
	}

	return res;
}

}


bool pFlow::positionRandom::positionOnePass(int32 pass, int32 startNum)
{

	realVector_D diameter(startNum , diameter_);
	int32Vector_HD  flagHD(startNum, 0);
	realx3Vector_HD positionHD(startNum);
	
	auto minP = region_->minPoint();
	auto maxP = region_->maxPoint();

	SearchType search(
					box(minP, maxP),
					diameter_,
					positionHD.deviceVectorAll(),
					diameter.deviceVectorAll());

	ContainerType pairs(3*startNum);
		
	Report(1)<< "Positioning "<<
		greenText("(Pass #"<< pass+1<<")")<< 
		": started with "<< startNum <<" points."<<endReport;
	
	fillPoints(startNum, positionHD, flagHD);
		
	search.broadSearch(pairs, range(0, startNum), true);

	
	int32 numCollisions = findCollisions(pairs, flagHD);
	
	
	Report(2)<< "Positioned " << cyanText(startNum - numCollisions) <<
	" without collision \n"<<endReport; 

	if(startNum-numCollisions >= numPoints_ )
	{
		
		Report(1)<<"Selected "<< cyanText(numPoints_)<< " for the final field.\n"<<endReport;

		positionHD.syncViews();
		position_.clear();
		int32 n=0;
		for(int32 i=0; i<startNum; i++)
		{
			if(flagHD[i] == 0 )
			{
				position_.push_back( positionHD[i]);
				n++;
				if(n==numPoints_)break;	
			}
			
		}
		
		return true;
	}
	
	
	return false;
	
	
}

bool pFlow::positionRandom::positionPointsRandom()
{
	
	position_.clear();

	if(numPoints_ == 0)return true;
	
	size_t pass = 0;
	int32 startNum = numPoints_;

	while ( pass <maxIterations_)
	{
		if( positionOnePass(pass, startNum) )return true;
		startNum = 1.1*startNum+1;
		pass++;
	}	
	
	
	fatalErrorInFunction<< 
	"  cannot position "<< numPoints_ << " in the domain in " << maxIterations_ << " iterations.\n" <<
	"  you may increase maxIterations for positioning points.\n";

	return false;
}

bool pFlow::positionRandom::inCollision
(
	const realx3 &cntr,
	real diam
)
{
	for(const auto& cp: position_)
	{
		if( length(cp-cntr) <= diam ) return true;
	}

	return false;
}

pFlow::positionRandom::positionRandom
(
	const dictionary& dict
)
:
	positionParticles(dict),
	prDict_
	(  
		dict.subDict("positionRandomInfo")
	),
	diameter_
	(
		prDict_.getVal<real>("diameter")
	),
	numPoints_
	(
		prDict_.getVal<size_t>("numPoints")
	),
	maxIterations_
	(
		prDict_.getValOrSet("maxIterations", 10)
	),
	position_
	(
		maxNumberOfParticles_, RESERVE()
	)
{

	reportInterval_ = max(numPoints_/numReports_, static_cast<size_t>(2));

	if( !positionPointsRandom() )
	{
		fatalExit;
	}

	if(!region_)
	{
		fatalErrorInFunction<<"You must provided a region (box, cylinder, ...) for positioning particles in dictionary "<<
		dict.globalName()<<endl;
		fatalExit;
	}

}

void pFlow::positionRandom::fillPoints(
	uint numPoints,
	realx3Vector_HD& points,
	int32Vector_HD& flags )
{

	uniformRandomReal rand;
	
	auto minP = region_().minPoint();
	auto maxP = region_().maxPoint();

	for(size_t i=0; i<numPoints; i++)
	{
		if(flags[i] == 0)
		{
			bool loop=true;
			size_t n=0;
			while (loop)
			{

				auto pos = rand(minP, maxP);
				if( region_().isInside(pos))
				{
					points[i] =pos;
					loop = false;		
				}
				n++;

				if(n>100)
				{
					fatalErrorInFunction<< 
					"could not find a point inside region"<<region_->name()<<endl;
					fatalExit;
				}
				
			}
			
		}
	}
	points.modifyOnHost();
	points.syncViews();
}

pFlow::int32 pFlow::findCollisions(
		ContainerType& pairs,
		int32Vector_HD& flags)
{
	auto allPairs = pairs.getPairs();
	auto num = pairs.capacity();
	auto dFlags = flags.deviceVector();
	

	int32 numCollisions = 0;

	Kokkos::parallel_reduce(
		"positionRandom::findCollisions",
		num,
		LAMBDA_HD(int32 i, int32& valueToUpdate){
			if(allPairs.isValid(i))
			{
				auto pair = allPairs.getPair(i);
				if( dFlags[pair.first] ==0 )
				{
					dFlags[pair.first] = 1;
					valueToUpdate++;	
				}
			}
		}, numCollisions);

	flags.modifyOnDevice();
	flags.syncViews();	

	return numCollisions;
}


