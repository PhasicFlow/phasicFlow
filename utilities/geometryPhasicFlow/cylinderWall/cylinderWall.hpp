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


#ifndef __cylinderWall_hpp__
#define __cylinderWall_hpp__

#include "Wall.hpp"
#include "zAxis.hpp"

namespace pFlow
{

class cylinderWall
:
	public Wall
{
protected:

	bool readCylinderWall(const dictionary& dict);

	bool createCylinder(const realx3& p1, const realx3& p2, real rad1, real rad2, int32 numDiv);

public:

	TypeInfo("cylinderWall");

	cylinderWall();

	cylinderWall(const dictionary& dict);

	add_vCtor
	(
		Wall,
		cylinderWall,
		dictionary
	);

};

} // pFlow




#endif //__cylinderWall_hpp__