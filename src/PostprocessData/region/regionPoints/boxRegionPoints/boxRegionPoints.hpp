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

/**
 * @file boxRegionPoints.hpp
 * @brief A class representing a box region for point selection
 * 
 * This class provides functionality to select points within a box region
 * and to compute related properties such as volume and equivalent diameter.
 * It inherits from regionPoints and implements all required virtual methods.
 * 
 * @see regionPoints
 * @see box
 * @see fieldsDataBase
 */

#ifndef __boxRegionPoints_hpp__
#define __boxRegionPoints_hpp__

#include "regionPoints.hpp"
#include "box.hpp"
#include "Vectors.hpp"

namespace pFlow::postprocessData
{

class boxRegionPoints
:
    public regionPoints
{
private:

    /// box object defining the region for point selection
    box  boxRegion_;

    /// Volume of the box region
    real    volume_;

    /// Diameter of the box region
    real    diameter_;

    /// Indices of points that are selected by this region
    uint32Vector    selectedPoints_;

public:

    TypeInfo(box::TYPENAME());

    /**
     * @brief Construct a box region for point selection
     * 
     * @param dict Dictionary containing boxInfo dictionary 
     * @param fieldsDataBase Database containing fields data
     */
    boxRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    /// Destructor
    ~boxRegionPoints() override = default;
    
    /**
     * @brief Get the number of regions (always 1 for box)
     * @return Always returns 1
     */
    uint32 size()const override
    {
        return 1;
    }

    /**
     * @brief Check if the region is empty
     * @return Always returns false
     */
    bool empty()const override
    {
        return false;
    }

    /**
     * @brief Get the volume of the box region
     * @return A span containing the volume of the region
     */
    span<const real> volumes()const override
    {
        return span<const real>(&volume_, 1);
    }

    /**
     * @brief Get the equivalent diameter of the box region
     * @return A span containing the diameter of the region
     */
    span<const real> eqDiameters()const override
    {
        return span<const real>(&diameter_, 1);
    }

    /**
     * @brief Get the center of the box region
     * @return A span containing the center point of the region
     */
    span<const realx3> centers()const override
    {
        realx3 center = 0.5 * (boxRegion_.minPoint() + boxRegion_.maxPoint());
        return span<const realx3>(&center, 1);
    }
    
    /**
     * @brief Get the indices of points within the region (const version)
     * @param elem Element index (ignored as there's only one box)
     * @return A span containing indices of points within the region
     */    
    span<const uint32> indices(uint32 elem)const override
    {
        return span<const uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    /**
     * @brief Get the indices of points within the region (non-const version)
     * @param elem Element index (ignored as there's only one box)
     * @return A span containing indices of points within the region
     */
    span<uint32> indices(uint32 elem) override
    {
        return span<uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    /**
     * @brief Update the points selected by this region
     * @return True if update was successful
     */
    bool update()override;

    /**
     * @brief Determine if data should be written to the same time file
     * @return Always returns true
     */
    bool writeToSameTimeFile()const override
    {
        return true;
    }

    /**
     * @brief Write region data to output stream
     * @param os Output stream to write to
     * @return True if write was successful
     */
    bool write(iOstream& os)const override;
    
};

}

#endif // __boxRegionPoints_hpp__