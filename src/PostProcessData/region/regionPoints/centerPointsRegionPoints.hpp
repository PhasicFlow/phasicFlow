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

#ifndef __centerPointsRegionPoints_hpp__
#define __centerPointsRegionPoints_hpp__

#include "regionPoints.hpp"

namespace pFlow
{

class centerPointsRegionPoints
:
    public regionPoints
{
private:
    
    bool            firstTimeUpdate_ = true;

    /// the ids provided in the dictionary
    uint32Vector    ids_;   

    /// the volume of region
    realVector       volume_ {"volume"};

    realVector       diameter_{"diameter"};
    /// center point of the region
    /// this is not used in the idSelection region
    realx3Vector     center_ {"center"};

    /// the point indices that are selected by this region
    uint32Vector     selectedPoints_{"selectedPoints"};
    
    /// the name of the id field
    word             idName_= "id";

    /// keeps the dictionary for first update use 
    dictionary       probDict_;
    
    
    bool selectIds();

public:

    TypeInfo("centerPoints");

    centerPointsRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    ~centerPointsRegionPoints() override = default;

    uint32 size()const override
    {
        return selectedPoints_.size();
    }

    bool empty()const override
    {
        return selectedPoints_.empty();
    }

    span<const real> volumes()const override
    {
        return span<const real>(volume_.data(), volume_.size());
    }

    span<const real> eqDiameters()const override
    {
        return span<const real>(diameter_.data(), diameter_.size());
    }

    span<const realx3> centers()const override
    {
        return span<const realx3>(center_.data(), center_.size());
    }

    span<const uint32> indices(uint32 elem)const override
    {
        return span<const uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    span<uint32> indices(uint32 elem) override
    {
        return span<uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    /// @brief  update the selected points based on the ids
    /// @return true if the operation is successful
    bool update() override;


}; // class centerPointsRegionPoints

} // namespace pFlow



#endif // __centerPointsRegionPoints_hpp__