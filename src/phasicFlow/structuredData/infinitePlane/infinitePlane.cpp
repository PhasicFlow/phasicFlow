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

#include "infinitePlane.hpp"

pFlow::infinitePlane::infinitePlane
(
  const realx3& p1, 
  const realx3& p2, 
  const realx3& p3
)
{
	auto ln = cross(p2-p1, p3-p1);
	
	if( equal(ln.length(),zero) )
	{
		fatalErrorInFunction<<
		"invalid input to form a infinte wall "<< realx3x3(p1,p2,p3)<<endl;
		fatalExit;
	}
	normal_ = normalize(ln);
	d_ = -dot(normal_, p1);
}

bool pFlow::infinitePlane::write(iOstream& os)const
{
	os.writeWordEntry("normal", normal_);
	os.writeWordEntry("d", d_);
	return os.check(FUNCTION_NAME);
}

bool pFlow::infinitePlane::read(iIstream & is)
{
	if(!is.nextData<realx3>("normal", normal_)) return false;
	if(!is.nextData<real>("d", d_)) return false;
	return true;
}

bool pFlow::infinitePlane::validPlane3
(
  const realx3& p1, 
  const realx3& p2, 
  const realx3& p3
)
{
  return !equal(cross(p2-p1, p3-p1).length(), zero);
}