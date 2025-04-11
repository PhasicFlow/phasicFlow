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

#ifndef __lineRegionPoints_hpp__
#define __lineRegionPoints_hpp__

#include "regionPoints.hpp"
#include "line.hpp"
#include "sphere.hpp"
#include "Vectors.hpp"

namespace pFlow
{

class lineRegionPoints
:
    public regionPoints
{
private:

    /// line region for selecting points
    line                line_;

    /// all sphere regions 
    Vector<sphere>        sphereRegions_;

    /// center poitns of regions/elements
    realx3Vector          centerPoints_;

    /// volumes of all elements/regions
    realVector            volumes_;

    realVector            diameters_;

    /// the point indices that are selected by this region
    Vector<uint32Vector>  selectedPoints_;

public:

    TypeInfo(line::TYPENAME());

    lineRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    ~lineRegionPoints() override = default;

    uint32 size()const override
    {
        return sphereRegions_.size();
    }

    bool empty()const override
    {
        return sphereRegions_.empty();
    }

    span<const real> volumes()const override
    {
        return span<const real>(volumes_.data(), volumes_.size());
    }

    span<const real> eqDiameters()const override
    {
        return span<const real>(diameters_.data(), diameters_.size());
    }

    span<const realx3> centers()const override
    {
        return span<const realx3>(centerPoints_.data(), centerPoints_.size());
    }

    span<const uint32> indices(uint32 elem)const override;
    

    bool update() override;

    bool writeToSameTimeFile()const override
    {
        return true;
    }

    bool write(iOstream& os) const override;
};
    
}


#endif // __lineRegionPoints_hpp__