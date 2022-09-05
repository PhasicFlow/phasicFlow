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

#ifndef __positionParticles_H__
#define __positionParticles_H__

#include "virtualConstructor.H"
#include "Vectors.H"
#include "dictionary.H"

namespace pFlow
{


class positionParticles
{
protected:

	size_t  		maxNumberOfParticles_ = 10000;

	Logical 		mortonSorting_;

	static const size_t numReports_ = 40;

	realx3Vector sortByMortonCode(realx3Vector& position)const;
	
public:

	// - type Info
	TypeName("positionParticles");

	positionParticles(const dictionary& dict);

	create_vCtor
	(
		positionParticles,
		dictionary,
		(const dictionary& dict),
		(dict)
	);

	virtual ~positionParticles() = default;

	//// - Methods 

	virtual label numPoints()const = 0;

	virtual label size()const = 0;

	virtual real maxDiameter() const = 0;

	// - const access to position
	virtual const realx3Vector& position()const = 0;
	

	// - access to position 
	virtual realx3Vector& position() = 0;
	
	virtual realx3Vector getFinalPosition()
	{
		if(mortonSorting_)
		{
			return sortByMortonCode(position());
		}
		else
		{
			return position();
		}
	}


	static
	uniquePtr<positionParticles> create(const dictionary & dict);
};	


}



#endif // __positionParticles_H__
