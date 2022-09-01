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


#ifndef __peakableRegion_H__
#define __peakableRegion_H__

#include "types.H"
#include "virtualConstructor.H"


namespace pFlow
{


class dictionary;

class peakableRegion
{
protected:

	
public:

	// type info
	TypeName("peakableRegion");

		peakableRegion(const word& type, const dictionary& dict);

		create_vCtor(
			peakableRegion,
			word,
			(const word& type, const dictionary& dict),
			(type, dict)
			);

		virtual uniquePtr<peakableRegion> clone()const = 0;
		
		virtual peakableRegion* clonePtr()const = 0;

		virtual ~peakableRegion() = default;

	//// - Methods

		virtual bool  isInside(const realx3& point)const = 0;

		virtual realx3 peek()const = 0;


	//// - IO operatoins 

		virtual bool read(const dictionary& dict) = 0;

		virtual bool write(dictionary& dict)const = 0;

	// - static create 
	static uniquePtr<peakableRegion> 
		create(const word& type, const dictionary& dict);

};

} // pFlow


#endif
