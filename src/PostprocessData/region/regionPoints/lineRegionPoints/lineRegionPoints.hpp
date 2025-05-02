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
 * @class lineRegionPoints 
 * @brief Spherical regions along a line for selecting points/particles
 * 
 * The lineRegionPoints class is responsible for selecting points/particles along a 
 * specified line and creating sphere regions around those points. It partitions
 * the line into multiple sphere regions (equally spaced) and keeps track of 
 * which points/particles in the simulation fall into each region.
 * 
 * This class is used for post-processing data by analyzing distributions of
 * particles along a linear path through the simulation domain. It maintains:
 *   - A line defining the sampling path
 *   - Spherical regions along this line that include particles
 *   - Center points for each region
 *   - Volumes and diameters of regions
 *   - Indices of points/particles contained in each region
 * 
 * The regions can be updated as the simulation progresses, and the data
 * can be written to output for analysis.
 *
 * @see regionPoints
 * @see line
 * @see sphere
 * @see fieldsDataBase
 */

#ifndef __lineRegionPoints_hpp__
#define __lineRegionPoints_hpp__


#include "regionPoints.hpp"
#include "line.hpp"
#include "sphere.hpp"
#include "Vectors.hpp"


namespace pFlow::postprocessData
{

class lineRegionPoints
:
    public regionPoints
{
private:

    /// Line path defining the axis of the spherical regions
    line                line_;

    /// Collection of sphere regions along the line
    Vector<sphere>        sphereRegions_;

    /// Center points of all spherical regions
    realx3Vector          centerPoints_;

    /// Volumes of all spherical regions
    realVector            volumes_;

    /// Diameters of all spherical regions
    realVector            diameters_;

    /// Point/particles indices selected by each region
    Vector<uint32Vector>  selectedPoints_;

public:

    /// Type information for runtime type identification
    TypeInfo(line::TYPENAME());

    /// Construct from dictionary that contains lineInfo and fields database
    lineRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    /// Default destructor
    ~lineRegionPoints() override = default;

    /// Return number of regions
    uint32 size()const override
    {
        return sphereRegions_.size();
    }

    /// Check if regions list is empty
    bool empty()const override
    {
        return sphereRegions_.empty();
    }

    /// Return volumes of all regions
    span<const real> volumes()const override
    {
        return span<const real>(volumes_.data(), volumes_.size());
    }

    /// Return equivalent diameters of all regions
    span<const real> eqDiameters()const override
    {
        return span<const real>(diameters_.data(), diameters_.size());
    }

    /// Return center points of all regions
    span<const realx3> centers()const override
    {
        return span<const realx3>(centerPoints_.data(), centerPoints_.size());
    }

    /// Return indices of points in the specified element/region
    span<const uint32> indices(uint32 elem)const override;

    span<uint32> indices(uint32 elem) override;
    
    /// Update regions based on current particle positions
    bool update() override;

    /// Whether to write all data to the same time file
    bool writeToSameTimeFile()const override
    {
        return true;
    }

    /// Write data to output stream
    bool write(iOstream& os) const override;
};
    
}


#endif // __lineRegionPoints_hpp__