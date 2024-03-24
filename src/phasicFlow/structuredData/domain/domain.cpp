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

#include "domain.hpp"

FUNCTION_H 
pFlow::domain::domain(const box& db)
:
	domainBox_(db),
	left_(
		db.minPoint(),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.minPoint().z()),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.maxPoint().z()),
		realx3(db.minPoint().x(), db.minPoint().y(), db.maxPoint().z())),
	right_(
		db.maxPoint(),
		realx3(db.maxPoint().x(), db.maxPoint().y(), db.minPoint().z()),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.minPoint().z()),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.maxPoint().z())),
	bottom_(
		db.minPoint(),
		realx3(db.minPoint().x(), db.minPoint().y(), db.maxPoint().z()),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.maxPoint().z()),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.minPoint().z())),
	top_(
		db.maxPoint(),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.maxPoint().z()),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.minPoint().z()),
		realx3(db.maxPoint().x(), db.maxPoint().y(), db.minPoint().z())),
	rear_(
		db.minPoint(),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.minPoint().z()),
		realx3(db.maxPoint().x(), db.maxPoint().y(), db.minPoint().z()),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.minPoint().z())),
	front_(
		db.maxPoint(),
		realx3(db.maxPoint().x(), db.minPoint().y(), db.maxPoint().z()),
		realx3(db.minPoint().x(), db.minPoint().y(), db.maxPoint().z()),
		realx3(db.minPoint().x(), db.maxPoint().y(), db.maxPoint().z()))
{

}


FUNCTION_H
pFlow::iIstream& pFlow::operator >>(
    pFlow::iIstream& is, 
    pFlow::domain& d)
{
    box b;
    is>>b; 
    d = domain(b);
    return is;
}

FUNCTION_H
pFlow::iOstream& pFlow::operator << (
    pFlow::iOstream& os, 
    const pFlow::domain& d)
{
    os<< d.domainBox();
    return os;
}
