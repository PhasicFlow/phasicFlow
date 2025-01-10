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

#ifndef __geometricRegion_hpp__
#define __geometricRegion_hpp__


#include "dictionary.hpp"
#include "uniformRandomReal.hpp"
#include "typeInfo.hpp"

namespace pFlow
{

template<typename GeomType>
class geometricRegion
{
public:

	using GeometryType = GeomType;

private:

	GeomType                  geom_;

	realx3                    minPoint_;

	realx3                    maxPoint_;

	mutable uniformRandomReal random_;

	static constexpr uint32   maxTries_ = 100;

public:

	// - type info
	TypeInfoTemplateNV11("geometricRegion", GeomType);

	explicit geometricRegion(const dictionary& dict);

	~geometricRegion() = default;

	//// - methods
	bool   isInside(const realx3& p) const;

	realx3 peek() const;

	const auto& minPoint()const
	{
		return minPoint_;
	}

	const auto& maxPoint()const
	{
		return maxPoint_;
	}

	//// IO operation
	bool   read(const dictionary& dict);

	bool   write(dictionary& dict) const;
};

} // namespace pFlow

#include "geometricRegion.cpp"

#endif
