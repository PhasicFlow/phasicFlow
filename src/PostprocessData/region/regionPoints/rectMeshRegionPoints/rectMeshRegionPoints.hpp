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
 * @file rectMeshRegionPoints.hpp
 * @brief A class representing a rectMesh region for point selection
 * 
 * This class provides functionality to select points within a rectMesh region
 * and to compute related properties such as volume and equivalent diameter.
 * It inherits from regionPoints and implements all required virtual methods.
 * 
 * @see regionPoints
 * @see rectMesh
 * @see fieldsDataBase
 */

#ifndef __rectMeshRegionPoints_hpp__
#define __rectMeshRegionPoints_hpp__

#include "regionPoints.hpp"
#include "box.hpp"
#include "Vectors.hpp"

namespace pFlow::postprocessData
{

class rectMeshRegionPoints
:
    public regionPoints
{
private:

    /// Sphere object defining the region for point selection
    Vector<box>  boxRegions_;

        /// Center coordinates of all rectMesh regions
    realx3Vector    minPoints_;

        /// Diameter of the rectMesh region
    realx3Vector    maxPoints_;

    /// Volume of the rectMesh region
    realVector    volumes_;

    /// Indices of points that are selected by this region
    Vector<uint32Vector>    selectedPoints_;

public:

    TypeInfo("rectMesh");

    /**
     * @brief Construct a rectMesh region for point selection
     * 
     * @param dict Dictionary containing sphereInfo dictionary 
     * @param fieldsDataBase Database containing fields data
     */
    rectMeshRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    /// Destructor
    ~rectMeshRegionPoints() override = default;
    
    /**
     * @brief Get the number of regions (always 1 for sphere)
     * @return Always returns 1
     */
    uint32 size()const override
    {
        return boxRegions_.size();
    }

    /**
     * @brief Check if the region is empty
     * @return Always returns false
     */
    bool empty()const override
    {
        return boxRegions_.empty();
    }

    /**
     * @brief Get the volume of the rectMesh region
     * @return A span containing the volume of the region
     */
    span<const real> volumes()const override
    {
        return span<const real>(volumes_.data(), volumes_.size());
    }

    /**
     * @brief Get the equivalent diameter of the rectMesh region
     * @return A span containing the diameter of the region
     */
    span<const real> eqDiameters()const override
    {
        return span<const real>(volumes_.data(), volumes_.size());
    }

    /**
     * @brief Get the center of the rectMesh region
     * @return A span containing the center point of the region
     */
    span<const realx3> centers()const override
    {
        return span<const realx3>(maxPoints_.data(), maxPoints_.size());
    }
    
    /**
     * @brief Get the indices of points within the region (const version)
     * @param elem Element index (ignored as there's only one sphere)
     * @return A span containing indices of points within the region
     */    
    span<const uint32> indices(uint32 elem)const override
    {
        if (elem >= size())
        {
        fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
            fatalExit;
        }

        return span<const uint32>(selectedPoints_[elem].data(), selectedPoints_[elem].size());
    }

    /**
     * @brief Get the indices of points within the region (non-const version)
     * @param elem Element index (ignored as there's only one sphere)
     * @return A span containing indices of points within the region
     */
    span<uint32> indices(uint32 elem) override
    {
        if (elem >= size())
        {
            fatalErrorInFunction
            << "The element index is out of range. elem: " << elem
            << " size: " << size() << endl;
            fatalExit;
        }

    return span<uint32>(selectedPoints_[elem].data(), selectedPoints_[elem].size());
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

#endif // __sphereRegionPoints_hpp__