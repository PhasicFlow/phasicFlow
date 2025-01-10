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

#ifndef __positionParticles_hpp__
#define __positionParticles_hpp__

#include "virtualConstructor.hpp"
#include "Vectors.hpp"
#include "peakableRegion.hpp"

namespace pFlow
{

class dictionary;
class systemControl;


class positionParticles
{
private:

	uniquePtr<peakableRegion> 		pRegion_ = nullptr;

	word 			regionType_;

	uint32  		maxNumberOfParticles_ = 10000;

	Logical 		mortonSorting_;

	

	realx3Vector sortByMortonCode(const realx3Vector& position)const;

protected:

	static const uint32 numReports_ = 40;

	const auto& pRegion()const
	{
		return pRegion_();
	}
	
public:

	// - type Info
	TypeInfo("positionParticles");

	positionParticles(
		systemControl& control, 
		const dictionary& dict);

	create_vCtor
	(
		positionParticles,
		dictionary,
		(systemControl& control, const dictionary& dict),
		(control, dict)
	);

	virtual ~positionParticles() = default;

	//// - Methods

	bool mortonSorting()const
	{
		return mortonSorting_();
	} 

	inline
	auto maxNumberOfParticles()const
	{
		return maxNumberOfParticles_;
	}

	virtual uint32 numPoints()const = 0;

	virtual uint32 size()const = 0;

	virtual real maxDiameter() const = 0;

	// - const access to position
	virtual const realx3Vector& position()const = 0;
	

	// - access to position 
	virtual realx3Vector& position() = 0;
	
	virtual realx3Vector getFinalPosition();

	static
	uniquePtr<positionParticles> 
		create(systemControl& control, const dictionary & dict);
	
};	


}



#endif // __positionParticles_hpp__
