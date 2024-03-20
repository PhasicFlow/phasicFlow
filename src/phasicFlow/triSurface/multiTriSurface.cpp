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


#include "multiTriSurface.hpp"

pFlow::multiTriSurface::multiTriSurface
(
	const objectFile &obj, 
	repository *owner
)
:
	triSurface(obj, owner),
	subscriber("multiTriSurface")
{
	if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
}

pFlow::multiTriSurface::multiTriSurface
(
	const objectFile &objf, 
	repository *owner, 
	const multiTriSurface &surf
)
:
	triSurface(objf, owner, surf),
	subscriber("multiTriSurface"),
	subSurfaces_(surf.subSurfaces_)
{
	
}

bool pFlow::multiTriSurface::appendSurface
(
	const word &name, 
	const realx3x3Vector &triangles
)
{
	uint32 start = size();
	uint32 pointStart = numPoints();

	if(!triSurface::append(triangles))
	{
		fatalErrorInFunction;
		return false;
	}
	uint32 end = size();
	uint32 pointEnd = numPoints();
	subSurfaces_.emplace_back(name, start, end, pointStart, pointEnd);

    return true;
}

bool pFlow::multiTriSurface::read(iIstream &is, const IOPattern &iop)
{	
	subSurfaces_.clear();

	if( !is.findKeywordAndVal("subSurfaces", subSurfaces_ ) )
	{
		fatalErrorInFunction<<
		"Error in reading subSurfaces from stream "<< is.name()<<endl;
		return false;
	}

    return triSurface::read(is, iop);
}

bool pFlow::multiTriSurface::write
(
    iOstream &os,
    const IOPattern &iop
) const
{	

	
	if( iop.thisProcWriteData() )
	{
		os.writeWordEntry("subSurfaces", subSurfaces_);
		if(!os.check(FUNCTION_NAME))return false;
	}
	
	return triSurface::write(os,iop);
}
