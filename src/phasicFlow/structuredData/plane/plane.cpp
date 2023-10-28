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

#include "plane.hpp"

pFlow::plane::plane
(
	const realx3& p1, 
	const realx3& p2, 
	const realx3& p3, 
	const realx3& p4
)
:
	infinitePlane(p1,p2,p3),
	p1_(p1),
	p2_(p2),
	p3_(p3),
	p4_(p4)
{

	if(!pointOnPlane(p4))
	{
		fatalErrorInFunction<<
		"points "<< realx4x3(p1,p2,p3,p4)<<" do not form a planner surface"<<endl;
		fatalExit;
	}
}


pFlow::plane pFlow::plane::parallelPlane(real distance)const
{
	auto pp1 = line(normal_, p1_, true).point(distance);
	auto pp2 = line(normal_, p2_, true).point(distance);
	auto pp3 = line(normal_, p3_, true).point(distance);
	auto pp4 = line(normal_, p4_, true).point(distance);

	return plane(pp1, pp2, pp3, pp4);
}

bool pFlow::plane::validPlane4
(
	const realx3& p1, 
	const realx3& p2, 
	const realx3& p3, 
	const realx3& p4
)
{
	if( !validPlane3(p1,p2,p3)) return false;
	if( !infinitePlane(p1,p2,p3).pointOnPlane(p4)) return false;
	return true;
}