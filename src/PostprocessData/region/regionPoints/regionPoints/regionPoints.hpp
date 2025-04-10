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

class fieldsDataBase;
class Time;

class regionPoints
{
    using PointsTypeHost = typename pointStructure::PointsTypeHost;

    fieldsDataBase& fieldsDataBase_;

public:

    TypeInfo("regionPoints");


    regionPoints(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);
    
    virtual ~regionPoints() = default;

    const Time& time()const;

    const fieldsDataBase& database()const;

    fieldsDataBase& database();
    
    /// @brief size of elements 
    virtual 
    uint32 size()const = 0;
    
    /// @brief  check if the region is empty
    virtual 
    bool empty()const = 0;

    /*/// @brief return the type of the region
    virtual const word& type()const = 0;*/
    
    
    /// @brief  volume of elements
    /// @return sapn for accessing the volume of elements 
    virtual 
    span<const real> volumes()const =0;
    
    virtual
    span<const real> eqDiameters()const = 0; 

    /// center points of elements 
    virtual
    span<const realx3> centers()const = 0;

    /// indices of particles inside the element @var elem
    virtual 
    span<const uint32> indices(uint32 elem)const = 0;

    virtual
    span<uint32> indices(uint32 elem) = 0;

    virtual 
    bool update() = 0;
    
    virtual
    bool writeToSameTimeFile()const = 0;

    virtual 
    bool write(iOstream& os)const=0;

    /*static
    uniquePtr<regionPoints> create(
        const dictionary& dict,
        fieldsDataBase& fieldsDataBase);*/
    
};

}

#endif // __regionPoints_hpp__