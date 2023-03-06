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


#include "planeWall.hpp"
#include "line.hpp"

bool pFlow::planeWall::readPlaneWall
(
	const dictionary& dict
)
{
	auto p1 = dict.getVal<realx3>("p1");
	auto p2 = dict.getVal<realx3>("p2");
	auto p3 = dict.getVal<realx3>("p3");
	auto p4 = dict.getVal<realx3>("p4");

	auto numDiv12 = max(dict.getValOrSet<int32>("numDiv12",1),1);
	auto numDiv23 = max(dict.getValOrSet<int32>("numDiv23",1),1);


	if(!checkFlatness(p1,p2,p3,p4))
	{
		fatalErrorInFunction << 
		"points p1, p2, p3 and p4 do not form a plane wall in dictionary " << dict.globalName()<<endl;
		return false;	
	}

	if(!addPlaneWall(p1,p2,p3,p4,numDiv12,numDiv23))
	{
		fatalErrorInFunction<<
		"could not create plane wall from dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
	
}


bool pFlow::planeWall::addWall4(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4)
{

	if(!checkFlatness(p1,p2,p3,p4))return false;
	
	triangles_.push_back(realx3x3(p1,p2,p3));
	triangles_.push_back(realx3x3(p3,p4,p1));
	
	return true;
}

bool pFlow::planeWall::checkFlatness(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4)
{
	if( !Wall::checkTrianlge(p1,p2,p3) ) return false;
	if( !Wall::checkTrianlge(p3,p4,p1) ) return false;
	return true;
}

bool pFlow::planeWall::addPlaneWall(
	const realx3& p1,
	const realx3& p2,
	const realx3& p3,
	const realx3& p4,
	int32 numDiv12,
	int32 numDiv23 )
{
	real dt12 = 1.0/numDiv12;
	real dt23 = 1.0/numDiv23;

	real t12 = 0;
	

	line line12(p1,p2);
	line line43(p4,p3);

	for(int32 i=0; i<numDiv12; i++)
	{

		auto lp1 = line12.point(t12);
		auto lp4 = line43.point(t12);
		auto lp2 = line12.point(t12+dt12);
		auto lp3 = line43.point(t12+dt12);

		line line14(lp1,lp4);
		line line23(lp2,lp3);
		real t23 = 0;
		for(int32 j=0; j<numDiv23; j++)
		{
			if(
				!addWall4(
					line14.point(t23),
					line23.point(t23),
					line23.point(t23+dt23),
					line14.point(t23+dt23) )
				)
			{
				return false;
			}
			t23+=dt23;
		}
		t12+=dt12;
	}


	return true;
}

pFlow::planeWall::planeWall()
{}

pFlow::planeWall::planeWall
(
	const dictionary& dict
)
:
	Wall(dict)
{
	if(!readPlaneWall(dict))
	{
		fatalExit;
	}
}

pFlow::planeWall::planeWall(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4,
		int32 numDiv12,
		int32 numDiv23 )
{

	if(!checkFlatness(p1,p2,p3,p4))
	{
		fatalErrorInFunction<<
		"the input points p1, p2, p3, and p4 are not in the same plane "<<endl;
		fatalExit;
	}

	if(!addPlaneWall(p1,p2,p3,p4, numDiv12, numDiv23))
	{
		fatalErrorInFunction<<
		"could not create plane wall from input points "<<endl;
		fatalExit;
	}

}

