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

#include "dictionary.hpp"
#include "geometricRegion.hpp"
#include "peakableRegion.hpp"

namespace pFlow
{

template<typename GeomType>
class PeakableRegion : public peakableRegion
{
public:

	using RegionType = geometricRegion<GeomType>;

	using GeometryType = typename RegionType::GeometryType;

private:

	RegionType region_;

public:

	// type info
	TypeInfoTemplate11("peakable", GeometryType);

	//// - Constructors
	PeakableRegion(const word& type, const dictionary& dict);

	add_vCtor(peakableRegion, PeakableRegion, word);

	virtual uniquePtr<peakableRegion> clone() const override
	{
		return makeUnique<PeakableRegion<GeomType>>(*this);
	}

	virtual peakableRegion* clonePtr() const override
	{
		return new PeakableRegion<GeomType>(*this);
	}

	virtual ~PeakableRegion() = default;

	//// - Methods

	bool   isInside(const realx3& point) const override;

	realx3 peek() const override;

	const realx3& minPoint()const override
	{
		return region_.minPoint();
	}

	const realx3& maxPoint()const override
	{
		return region_.maxPoint();
	}

	//// - IO operatoins

	virtual bool   read(const dictionary& dict) override;

	virtual bool   write(dictionary& dict) const override;
};

} // pFlow

#include "PeakableRegion.cpp"

#endif
