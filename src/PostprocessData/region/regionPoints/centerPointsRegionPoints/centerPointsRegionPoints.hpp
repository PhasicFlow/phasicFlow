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

namespace pFlow::postprocessData
{

/**
 * @class centerPointsRegionPoints
 * @brief A region points implementation that selects particles based on their IDs
 * 
 * This class is responsible for selecting points (particles) by their IDs from
 * a simulation database and tracking their properties. It maintains information 
 * about the selected particles including their positions, volumes, and diameters.
 * 
 * The selection is performed based on IDs provided in the input dictionary.
 * Once selected, the particles' properties can be accessed through various 
 * methods. The update method allows refreshing the selection when particle data
 * changes. The selection occurs at startTime defined in the time control, and
 * there are some methods for selecting ids:
 *  - specifying ids
 *  - using selectors specified in pStructSelector class, which includes:
 *    - box: selects particles within a box region
 *    - sphere: selects particles within a spherical region
 *    - cylinder: selects particles within a cylindrical region
 *    - random: randomly selects a specified number of particles
 *    - strided: selects particles with a specified stride pattern
 * 
 * This class is useful for tracking specific particles of interest throughout
 * a simulation and analyzing their behavior.
 * 
 * @see regionPoints Base class providing the interface for different region 
 *                   point selections
 * @see pStructSelector Class providing different particle selection methods
 */
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

    /// Type info
    TypeInfo("centerPoints");

    centerPointsRegionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);

    ~centerPointsRegionPoints() override = default;

    /// @brief  Returns the number of selected points/particles
    /// @return Number of selected points/particles
    uint32 size()const override
    {
        return selectedPoints_.size();
    }

    /// @brief  Checks if there are no selected points
    /// @return True if no points are selected, false otherwise
    bool empty()const override
    {
        return selectedPoints_.empty();
    }

    /// @brief  Returns the volumes of the selected points (this is normally not used)
    span<const real> volumes()const override
    {
        return span<const real>(volume_.data(), volume_.size());
    }

    /// @brief  Returns the equivalent diameters of the regions (this is normally not used )
        span<const real> eqDiameters()const override
    {
        return span<const real>(diameter_.data(), diameter_.size());
    }

    /// @brief  Returns the center positions of the selected points
    /// @return Span containing the center positions of all selected points
    span<const realx3> centers()const override
    {
        return span<const realx3>(center_.data(), center_.size());
    }

    /// @brief  Returns the indices of the selected points (const version)
    /// @param elem Element index (not used in this implementation)
    /// @return Span containing the indices of all selected points
    span<const uint32> indices(uint32 elem)const override
    {
        return span<const uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    /// @brief  Returns the indices of the selected points (non-const version)
    /// @param elem Element index (not used in this implementation)
    /// @return Span containing the indices of all selected points
    span<uint32> indices(uint32 elem) override
    {
        return span<uint32>(selectedPoints_.data(), selectedPoints_.size());
    }

    /// @brief  Updates the selected points based on the particle IDs
    /// @return True if the operation is successful, false otherwise
    bool update() override;

    /// @brief  Checks if the data should be written to the same time file
    /// @return True if data should be written to the same time file, false otherwise
    bool writeToSameTimeFile()const override
    {
        return true;
    }

    /// @brief  Writes the data to the output stream
    /// @param os Output stream
    /// @return True if the operation is successful, false otherwise
    bool write(iOstream& os)const override;

}; // class centerPointsRegionPoints

} // namespace pFlow::postprocessData



#endif // __centerPointsRegionPoints_hpp__