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

#ifndef __selectorGeometric_hpp__
#define __selectorGeometric_hpp__

#include "pStructSelector.hpp"
#include "PeakableRegion.hpp"
#include "pointStructure.hpp"
#include "dictionary.hpp"

namespace pFlow
{

template<typename GeomType>
class selectorGeometric 
: 
    public pStructSelector
{
private:

	word 					 type_;

	PeakableRegion<GeomType> pRegion_;

	uint32Vector             selectedPoints_{"selectedPoints"};

	bool selectPoints();

public:

	TypeInfoTemplate11("selector", GeomType);

	selectorGeometric(const pointStructure& pStruct, const dictionary& dict);

	add_vCtor
    (
        pStructSelector, 
        selectorGeometric, 
        dictionary
    );

	~selectorGeometric() override = default;

	const uint32Vector& selectedPoints()const override
	{
		return selectedPoints_;
	}

	uint32Vector& selectedPoints() override
	{
		return selectedPoints_;
	}
};

}

#include "selectorGeometric.cpp"

#endif //__selectorGeometric_hpp__