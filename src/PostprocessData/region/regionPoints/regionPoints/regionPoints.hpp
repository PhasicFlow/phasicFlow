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

#ifndef __regionPoints_hpp__
#define __regionPoints_hpp__

#include "iOstream.hpp"
#include "dictionary.hpp"
#include "pointStructure.hpp"

namespace pFlow
{
    class Time;
}

namespace pFlow::postprocessData
{

class fieldsDataBase;


/**
 * @class regionPoints
 * @brief Abstract base class for managing and processing volumetric regions 
 * in the simulation. Particles are selected based on their positions within
 * these defined regions.
 * 
 * This class provides an interface for accessing and manipulating data 
 * related to regions of points (particles), including their volumes, equivalent 
 * diameters, center points, and particle indices that they contain. It interacts with the 
 * fieldsDataBase and Time classes to retrieve simulation data. 
 */
class regionPoints
{
    using PointsTypeHost = typename pointStructure::PointsTypeHost;

    /// Reference to the fields database containing simulation data
    fieldsDataBase& fieldsDataBase_;

    /// default precision for output
    int precision_ = 6;

    /// if scientific notation is used for output
    Logical scientific_;

public:

    TypeInfo("regionPoints");

    /// Constructor with dictionary and fields database reference
    regionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);
    
    /// Default virtual destructor
    virtual ~regionPoints() = default;

    /// Returns reference to the time object from the database
    const Time& time()const;

    /// Returns const reference to the fields database
    const fieldsDataBase& database()const;

    /// Returns non-const reference to the fields database
    fieldsDataBase& database();

    int precision() const
    {
        return precision_;
    }
    
    bool scientific()const
    {
        return scientific_(); 
    }
    /// @brief size of elements 
    virtual 
    uint32 size()const = 0;

    virtual 
    uint32x3 shape()const
    {
      return uint32x3(size(), 1u, 1u);
    }
    
    /// @brief  check if the region is empty
    virtual 
    bool empty()const = 0;  
    
    /// by default it does nothing
    /// But, it can be used for the methods that needs to search for 
    /// particles which are beyound the region 
    virtual void applyRegionExtension()
    {}

    virtual 
    real regionExtensionRatio()const
    {
        return 1.0;
    }

    /// @brief  volume of elements
    /// @return sapn for accessing the volume of elements 
    virtual 
    span<const real> volumes()const =0;
    
    virtual
    span<const real> eqDiameters()const = 0; 

    /// center points of elements
    virtual
    span<const realx3> centers()const = 0;

    /// Returns const span of particle indices inside the specified element region
    virtual 
    span<const uint32> indices(uint32 elem)const = 0;

    /// Returns non-const span of particle indices inside the specified element region
    virtual
    span<uint32> indices(uint32 elem) = 0;

    /// Updates the points (particles) inside regions based on current particle positions
    virtual 
    bool update() = 0;
    
    /// Returns true if the region should be written to the same time file
    virtual
    bool writeToSameTimeFile()const = 0;

    /// Writes region data to the output stream
    virtual 
    bool write(iOstream& os)const=0;
    
};

}

#endif // __regionPoints_hpp__