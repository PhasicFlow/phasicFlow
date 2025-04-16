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

template<pFlow::ValidFieldType T>
inline
pFlow::span<T> pFlow::fieldsDataBase::updateField(const word& name, bool forceUpdate)
{
  
    bool shouldUpdate = checkForUpdate(name, forceUpdate);
        
    if(shouldUpdate)
    {
        if(name == "one")
        {   
            allFields_.emplaceBackOrReplace<FieldTypeHost<T>>
            (
                "one",
                FieldTypeHost<T>
                (
                    "one", 
                    "value",
                    pointFieldSize(),
                    T{1}
                )
            );
        }
        else if( name == "position")
        {
            if constexpr( std::same_as<T, realx3>)
            {
                return updatePoints(true);            
            }
            else
            {
                fatalErrorInFunction<< "Error in getting the type name of field: "<<
                    name<<", with type name: "<< getTypeName<T>() <<endl;
                fatalExit;
                return span<T>(nullptr, 0);
            }
        }
        else
        {
            const auto& pField = time_.lookupObject<pointField_D<T>>(name);
            allFields_.emplaceBackOrReplace<FieldTypeHost<T>>(
            name, 
            pField.activeValuesHost());
        }
    }

    auto& field = allFields_.getObject<FieldTypeHost<T>>(name);
    
    return span<T>(
        field.data(), 
        field.size());
    
}

#endif //__fieldsDataBaseTemplates_hpp__