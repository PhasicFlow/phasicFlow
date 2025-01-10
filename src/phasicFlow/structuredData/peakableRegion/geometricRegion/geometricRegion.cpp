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

template<typename GeomType>
pFlow::geometricRegion<GeomType>::geometricRegion(const dictionary& dict)
  : geom_(dict)
  , random_()
{
	minPoint_ = geom_.minPoint();
	maxPoint_ = geom_.maxPoint();
}

template<typename GeomType>
inline bool
pFlow::geometricRegion<GeomType>::isInside(const realx3& p) const
{
	return geom_.isInside(p);
}

template<typename GeomType>
inline pFlow::realx3
pFlow::geometricRegion<GeomType>::peek() const
{
	for (uint32 i = 0u; i < maxTries_; i++)
	{
		realx3 p = random_.randomNumber(minPoint_, maxPoint_);

		if (isInside(p))
			return p;
	}

	fatalErrorInFunction << "cannot peek a random point from geometricRegion "
	                    << typeName() << endl;
	fatalExit;
	return 0;
}

template<typename GeomType>
bool
pFlow::geometricRegion<GeomType>::read(const dictionary& dict)
{
	return geom_.read(dict);
}

template<typename GeomType>
bool
pFlow::geometricRegion<GeomType>::write(dictionary& dict) const
{
	return geom_.write(dict);
}