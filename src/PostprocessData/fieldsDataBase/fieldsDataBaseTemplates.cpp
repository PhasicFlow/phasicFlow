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

#ifndef __fieldsDataBaseTemplates_hpp__
#define __fieldsDataBaseTemplates_hpp__

#include "fieldsDataBase.hpp"

template<pFlow::postprocessData::ValidFieldType T>
inline
pFlow::span<T> pFlow::postprocessData::fieldsDataBase::updateField(const word& name, bool forceUpdate)
{
  
    bool shouldUpdate = checkForUpdate(name, forceUpdate);
        
    if(shouldUpdate)
    {
        if(reservedFieldNames_.contains(name))
        {
            return updateReservedField<T>(name, true);
        }   
        else
        {
            if( loadPointFieldToTime(name) )
            {
                const auto& pField = time_.template lookupObject<pointField_D<T>>(name);
                allFields_.template emplaceBackOrReplace<FieldTypeHost<T>>(
                name, 
                pField.activeValuesHost());
            }
            else
            {
                fatalErrorInFunction<<"Error in loading the pointField "<<name<<endl;
                fatalExit;
            }
            
        }
    }

    auto& field = allFields_.getObject<FieldTypeHost<T>>(name);
    
    return span<T>(
        field.data(), 
        field.size());
    
}


template<pFlow::postprocessData::ValidFieldType T>
inline
pFlow::span<T> pFlow::postprocessData::fieldsDataBase::updateReservedField
(
    const word& name, 
    bool forceUpdate
)
{
    if(name == "one")
    {   
        if constexpr( std::same_as<T,real>)
        {
            return createOrGetOne(forceUpdate);
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field one."<<endl;
            fatalExit;
        }
    }
    else if(name == "volume")
    {
        if constexpr( std::same_as<T,real>)
        {
            return createOrGetVolume(forceUpdate);
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field volume."<<endl;
            fatalExit;
        }
    }
    else if( name == "density")
    {
        if constexpr( std::same_as<T,real>)
        {
            return createOrGetDensity(forceUpdate);
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field density."<<endl;
            fatalExit;
        }
    }
    else if( name == "mass")
    {
        if constexpr( std::same_as<T,real>)
        {
            return createOrGetMass(forceUpdate);
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field mass."<<endl;
            fatalExit;
        }
    }
    else if( name == "I")
    {
        if constexpr( std::same_as<T,real>)
        {
            return createOrGetI(forceUpdate);
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field I."<<endl;
            fatalExit;
        }
    }
    else if( name == "position")
    {
        if constexpr( std::same_as<T, realx3>)
        {
            return updatePoints(forceUpdate);            
        }
        else
        {
            fatalErrorInFunction
                << "This type: "
                << getTypeName<T>()
                <<" is not supported for field position."<<endl;
            fatalExit;
        }
    }
    
    fatalErrorInFunction<<"Not supported field name "<<endl;
    fatalExit;
    return span<T>(nullptr, 0);

}

#endif //__fieldsDataBaseTemplates_hpp__