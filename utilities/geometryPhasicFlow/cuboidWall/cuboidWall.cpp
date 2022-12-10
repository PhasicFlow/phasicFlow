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


#include "cuboidWall.hpp"
#include "planeWall.hpp"



bool pFlow::cuboidWall::readcuboidWall
(
	const dictionary& dict
)
{
	
	auto 	center = dict.getVal<realx3>("center");
	auto 	edgeLength= dict.getVal<realx3>("edgeLength");
	auto numDivs = dict.getValOrSet<int32x3>("numDivs", int32x3(1,1,1));

	numDivs = max(numDivs, int32x3(1,1,1));

	realx3 p1,p2,p3,p4;

	// left plane 
	p1 = center + edgeLength*realx3(-0.5,-0.5,-0.5);
	p2 = center + edgeLength*realx3(-0.5, 0.5,-0.5);
	p3 = center + edgeLength*realx3(-0.5, 0.5, 0.5);
	p4 = center + edgeLength*realx3(-0.5,-0.5, 0.5);

	planeWall left(p1,p2,p3,p4, numDivs.y(), numDivs.z());

	for(const auto& t:left.triangles())
	{
		triangles_.push_back(t);
	}

	// right plane
	p1 = center + edgeLength*realx3( 0.5,-0.5,-0.5);
	p2 = center + edgeLength*realx3( 0.5,-0.5, 0.5);
	p3 = center + edgeLength*realx3( 0.5, 0.5, 0.5);
	p4 = center + edgeLength*realx3( 0.5, 0.5,-0.5);

	planeWall right(p1,p2,p3,p4, numDivs.z(), numDivs.y());

	for(const auto& t:right.triangles())
	{
		triangles_.push_back(t);
	}

	// bottom plane 
	p1 = center + edgeLength*realx3(-0.5,-0.5,-0.5);
	p2 = center + edgeLength*realx3(-0.5,-0.5, 0.5);
	p3 = center + edgeLength*realx3( 0.5,-0.5, 0.5);
	p4 = center + edgeLength*realx3( 0.5,-0.5,-0.5);

	planeWall bottom(p1,p2,p3,p4, numDivs.z(), numDivs.x());

	for(const auto& t:bottom.triangles())
	{
		triangles_.push_back(t);
	}

	// top plane 
	p1 = center + edgeLength*realx3(-0.5, 0.5,-0.5);
	p2 = center + edgeLength*realx3( 0.5, 0.5,-0.5);
	p3 = center + edgeLength*realx3( 0.5, 0.5, 0.5);
	p4 = center + edgeLength*realx3(-0.5, 0.5, 0.5);

	planeWall top(p1,p2,p3,p4, numDivs.x(), numDivs.z());

	for(const auto& t:top.triangles())
	{
		triangles_.push_back(t);
	}

	// back plane 
	p1 = center + edgeLength*realx3(-0.5,-0.5,-0.5);
	p2 = center + edgeLength*realx3( 0.5,-0.5,-0.5);
	p3 = center + edgeLength*realx3( 0.5, 0.5,-0.5);
	p4 = center + edgeLength*realx3(-0.5, 0.5,-0.5);


	planeWall back(p1,p2,p3,p4, numDivs.x(), numDivs.y());

	for(const auto& t:back.triangles())
	{
		triangles_.push_back(t);
	}


	// fron plane 
	p1 = center + edgeLength*realx3(-0.5,-0.5, 0.5);
	p2 = center + edgeLength*realx3(-0.5, 0.5, 0.5);
	p3 = center + edgeLength*realx3( 0.5, 0.5, 0.5);
	p4 = center + edgeLength*realx3( 0.5,-0.5, 0.5);

	planeWall front(p1,p2,p3,p4, numDivs.y(), numDivs.x());

	for(const auto& t:front.triangles())
	{
		triangles_.push_back(t);
	}


	return true;
	
}

pFlow::cuboidWall::cuboidWall()
{}

pFlow::cuboidWall::cuboidWall
(
	const dictionary& dict
)
:
	Wall(dict)
{
	if(!readcuboidWall(dict))
	{
		fatalExit;
	}
}
