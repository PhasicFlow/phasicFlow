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


#ifndef __PeakableRegion_hpp__
#define __PeakableRegion_hpp__


#include "peakableRegion.hpp"
#include "dictionary.hpp"

namespace pFlow
{




template<typename RegionType>
class PeakableRegion
:
	public peakableRegion
{
protected:

	RegionType 		region_;

public:

	// type info
	TypeInfoTemplate("peakableRegion", RegionType);

	//// - Constructors 
		PeakableRegion(const word& type, const dictionary& dict);

		add_vCtor(
			peakableRegion,
			PeakableRegion,
			word
			);

		virtual uniquePtr<peakableRegion> clone()const override
		{
			return makeUnique<PeakableRegion<RegionType>>(*this);
		}
		
		virtual peakableRegion* clonePtr()const override
		{
			return new PeakableRegion<RegionType>(*this);
		}


		virtual ~PeakableRegion() = default;

	//// - Methods

		virtual bool  isInside(const realx3& point)const override;

		virtual realx3 peek()const override;


	//// - IO operatoins 

		virtual bool read(const dictionary& dict) override;

		virtual bool write(dictionary& dict)const override;

};

} // pFlow

#include "PeakableRegion.cpp"

#endif
