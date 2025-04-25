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


#include "stlWall.hpp"
#include "stlFile.hpp"

#include "streams.hpp"

bool pFlow::stlWall::readSTLWall
(
	const dictionary& dict
)
{
	auto fileName = dict.getVal<word>("file");

	real scale = dict.getValOrSet("scale", static_cast<real>(1.0));

	realx3 transform = dict.getValOrSet<realx3>("transform", realx3(0));

	auto scaleFirst = dict.getValOrSet("scaleFirst", Logical("Yes"));
	
	fileSystem file("./stl",fileName);

	stlFile stl(file);
	if(!stl.read())
	{
		fatalErrorInFunction <<
		"  error in reading stl file "<< file <<endl;
		return false;
	}

	// Scale and transform the stl vertex
	realx3x3Vector newStlVertx;
	for(uint64 i = 0; i < stl.size(); i++)
	{
		for(uint64 j = 0; j < stl.solid(i).size(); j++)
		{
			realx3x3 tri;

			if(scaleFirst)
			{
				tri.x() = stl.solid(i)[j].x() * scale + transform.x();
				tri.y() = stl.solid(i)[j].y() * scale + transform.y();
				tri.z() = stl.solid(i)[j].z() * scale + transform.z();
			}
			else
			{
				tri.x() = (stl.solid(i)[j].x() + transform.x()) * scale;
				tri.y() = (stl.solid(i)[j].y() + transform.y()) * scale;
				tri.z() = (stl.solid(i)[j].z() + transform.z()) * scale;
			}

			newStlVertx.push_back(tri);
		}
	}

	// Insert the new vertex to the triangles_
	for(uint64 i = 0; i < stl.size(); i++)
	{
		auto it = triangles_.end();
		triangles_.insert(it, newStlVertx.begin(), newStlVertx.end());
	}
	

	return true;
	
}

pFlow::stlWall::stlWall()
{}

pFlow::stlWall::stlWall
(
	const dictionary& dict
)
:
	Wall(dict)
{
	if(!readSTLWall(dict))
	{
		fatalExit;
	}
}
