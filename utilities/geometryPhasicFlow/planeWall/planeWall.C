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


#include "planeWall.H"



bool pFlow::planeWall::readPlaneWall
(
	const dictionary& dict
)
{
	auto p1 = dict.getVal<realx3>("p1");
	auto p2 = dict.getVal<realx3>("p2");
	auto p3 = dict.getVal<realx3>("p3");
	auto p4 = dict.getVal<realx3>("p4");

	
	if( Wall::checkTrianlge(p1,p2,p3) )
	{
		triangles_.push_back(realx3x3(p1,p2,p3));
	}else
	{
		fatalErrorInFunction << 
		"points p1, p2 and p3 do not form a plane wall in dictionary " << dict.globalName()<<endl;
		return false;
	}

	if( Wall::checkTrianlge(p3,p4,p1) )
	{
		triangles_.push_back(realx3x3(p3,p4,p1));
	}else
	{
		fatalErrorInFunction << 
		"points p3, p4 and p1 do not form a plane wall in dictionary " << dict.globalName()<<endl;
		return false;
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
