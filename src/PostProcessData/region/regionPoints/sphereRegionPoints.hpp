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

#ifndef __sphereRegionPoints_hpp__
#define __sphereRegionPoints_hpp__

#include "regionPoints.hpp"
#include "sphere.hpp"
#include "Vectors.hpp"

namespace pFlow
{
class sphereRegionPoints
:
    public regionPoints
{
private:

    /// spehre region for selecting points
    sphere  sphereRegion_;

    /// the volume of region
    real    volume_;

    real    diameter_;

    /// the point indices that are selected by this region
    uint32Vector    selectedPoints_;

public:

    TypeInfo(sphere::TYPENAME());

    sphereRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    ~sphereRegionPoints() override = default;
    
    uint32 size()const override
    {
        return 1;
    }

    bool empty()const override
    {
        return false;
    }

    span<const real> volumes()const override
    {
        return span<const real>(&volume_, 1);
    }

    span<const real> eqDiameters()const override
    {
        return span<const real>(&diameter_, 1);
    }

    span<const realx3> centers()const override
    {
        return span<const realx3>(&sphereRegion_.center(), 1);
    }
        
    span<const uint32> indices(uint32 elem)const override
    {
        return span<const uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    span<uint32> indices(uint32 elem) override
    {
        return span<uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    bool update()override;
    
};

}

#endif // __sphereRegionPoints_hpp__