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


#ifndef __planeWall_H__
#define __planeWall_H__


#include "Wall.H"
#include "types.H"

namespace pFlow
{

class planeWall
:
	public Wall
{
protected:

	bool readPlaneWall(const dictionary& dict);

	bool addWall4(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4);

	bool checkFlatness(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4);

	bool addPlaneWall(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4,
		int32 numDiv12 = 1,
		int32 numDiv23 = 1);

public:

	TypeName("planeWall");

	planeWall();

	planeWall(const dictionary& dict);

	planeWall(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3,
		const realx3& p4,
		int32 numDiv12 = 1,
		int32 numDiv23 = 1);

	add_vCtor
	(
		Wall,
		planeWall,
		dictionary
	);


};

} // pFlow


#endif
