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
 * @class       multipleSpheresRegionPoints
 * @brief       A class to select and track particles contained within multiple 
 *              spherical regions
 * @details     This class defines multiple spherical regions in the simulation 
 *             domain and identifies which particles are contained within each 
 *             sphere at each time step. It inherits from the regionPoints base 
 *             class and specializes it for handling multiple spherical regions 
 *             simultaneously.
 *             
 *             The class reads a list of center points and radii from a dictionary,
 *             creates sphere objects for each, and provides methods to:
 *             - Track which particles are inside each spherical region
 *             - Return volumetric information about the regions
 *             - Access center points and size information for each sphere
 *
 *             
 * @note        Used in post-processing workflows to analyze particle behavior
 *             in specific regions of interest within the simulation domain.
 * 
 * @see        regionPoints Base class for all region-based point selection
 * @see        sphere Geometric primitive used to define spherical regions
 * @see        postprocessPhasicFlow Utility for post-processing simulation data
 * @see        fieldsDataBase Database containing simulation field data
 */

#ifndef __multipleSpheresRegionPoints_hpp__
#define __multipleSpheresRegionPoints_hpp__

#include "regionPoints.hpp"
#include "sphere.hpp"
#include "Vectors.hpp"

namespace pFlow
{

class multipleSpheresRegionPoints
:
    public regionPoints
{
private:

    /// Vector containing all spherical regions used for particle selection
    Vector<sphere>        sphereRegions_;

    /// Center coordinates of all spherical regions
    realx3Vector          centerPoints_;

    /// Diameters of all spherical regions
    realVector            diameters_;

    /// Volumes of all spherical regions
    realVector            volumes_;

    /// Vectors of point indices for particles contained in each spherical region
    /// Each element corresponds to a particular sphere region
    Vector<uint32Vector>  selectedPoints_;

public:

    /// Type identification for run-time type information
    TypeInfo("multipleSpheres");

    /// Constructor
    /// @param dict Dictionary containing multipleSpheresInfo for the regions
    /// @param fieldsDataBase Reference to the database containing field data
    multipleSpheresRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    /// Virtual destructor for proper inheritance cleanup
    ~multipleSpheresRegionPoints() override = default;

    /// Returns the number of spherical regions
    /// @return Number of spherical regions
    uint32 size()const override
    {
        return sphereRegions_.size();
    }

    /// Checks if there are any spherical regions defined
    /// @return True if no regions exist, false otherwise
    bool empty()const override
    {
        return sphereRegions_.empty();
    }

    /// Returns the volumes of all spherical regions
    /// @return Span containing the volumes of all regions
    span<const real> volumes()const override
    {
        return span<const real>(volumes_.data(), volumes_.size());
    }

    /// Returns the diameters of all spherical regions
    /// @return Span containing the diameters of all regions
    span<const real> diameters()const
    {
        return span<const real>(diameters_.data(), diameters_.size());
    }

    /// Returns the equivalent diameters of all spherical regions
    /// @return Span containing the equivalent diameters (same as diameters)
    span<const real> eqDiameters()const
    {
        return diameters();
    }

    /// Returns the center coordinates of all spherical regions
    /// @return Span containing the center points of all regions
    span<const realx3> centers()const override
    {
        return span<const realx3>(centerPoints_.data(), centerPoints_.size());
    }

    /// Returns the indices of particles contained in a specific spherical region
    /// @param elem Index of the spherical region to query
    /// @return Span containing indices of particles within the specified region
    span<const uint32> indices(uint32 elem)const override;

    /// Updates the selection of particles within each spherical region
    /// @return True if update was successful, false otherwise
    bool update() override;

    /// Determines if data should be written to the same time file
    /// @return True to indicate regions should be written to the same time file
    bool writeToSameTimeFile()const override
    {
        return true;
    }

    /// Writes region data to the output stream
    /// @param os Output stream to write data to
    /// @return True if write operation was successful, false otherwise
    bool write(iOstream& os) const override;
};
    
}


#endif // __multipleSpheresRegionPoints_hpp__