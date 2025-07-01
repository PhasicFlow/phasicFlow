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

#ifndef __regionField_hpp__
#define __regionField_hpp__

#include "types.hpp"
#include "regionPoints.hpp"
#include "Field.hpp"
#include "cellMapper.hpp"

namespace pFlow::postprocessData
{

template<typename T> 
class regionField
{
public:
    using FieldType         = Field<T, HostSpace>;

    using iterator        	= typename FieldType::iterator;

    using const_iterator   	= typename FieldType::const_iterator;

    using reference       	= typename FieldType::reference;
    
    using const_reference  	= typename FieldType::const_reference;

    using value_type       	= typename FieldType::value_type;
    
    using pointer         	= typename FieldType::pointer;
    
    using const_pointer    	= typename FieldType::const_pointer;
private:
           
    /// the field value
    Field<T, HostSpace>      field_;
    
    /// the region points
    const regionPoints& regionPoints_;
public:

    TypeInfoTemplateNV11("regionField", T);

    regionField(
        const word& name, 
        const regionPoints& rPoints,
        const T defaultVal);
    
    regionField(const regionField&) = default;

    regionField(regionField&&) = default;

    regionField& operator=(const regionField&) = default;

    regionField& operator=(regionField&&)  = default;

    ~regionField() = default;

    const regionPoints& regPoints() const
    {
        return regionPoints_;
    }

    /// get the field value
    T& operator[] (const uint32 i)
    {
        return field_[i];
    }

    /// get the field value
    const T& operator[] (const uint32 i)const
    {
        return field_[i];
    }

    /// get field name 
    word name()const
    {
        return field_.name();
    }

    auto size()const
    {
        return field_.size();
    }

    uint32x3 shape()const
    {
        return regionPoints_.shape();
    }

    bool empty()const
    {
        return field_.empty();
    }

    /// @brief  write the field to a file 
    /// @param os output file stream
    /// @return true if successful and false if fails 
    bool writeFieldToFile(iOstream& os)const;

    /// @brief  write the field to vtk format (cell based)
    /// @param os output file stream
    /// @return true if successful and false if fails 
    bool writeFieldToVtk(iOstream& os)const;

};


} // namespace pFlow::postprocessData

#include "regionFieldTemplate.cpp"

#endif // __regionField_hpp__