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

	
	fileSystem file("./stl",fileName);

	stlFile stl(file);
	if(!stl.read(scale))
	{
		fatalErrorInFunction <<
		"  error in reading stl file "<< file <<endl;
		return false;
	}

	for(uint64 i=0; i<stl.size(); i++)
	{
		auto it = triangles_.end();
		triangles_.insert(it, stl.solid(i).begin(), stl.solid(i).end());
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
