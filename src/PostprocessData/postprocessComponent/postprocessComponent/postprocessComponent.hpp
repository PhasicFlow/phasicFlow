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

#ifndef __postprocessComponent_hpp__
#define __postprocessComponent_hpp__

#include "postprocessTimeControl.hpp"
#include "dictionary.hpp"
#include "virtualConstructor.hpp"

namespace
{
    class dictionary;
}

namespace pFlow::postprocessData
{


class regionPoints;
class fieldsDataBase;

class postprocessComponent
{
private:

    postprocessTimeControl  timeControl_;

    fieldsDataBase&         fieldsDataBase_;

public:

    /// type info
    TypeInfo("postprocessComponent");

    postprocessComponent( 
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl);
    
    virtual ~postprocessComponent() = default;


    create_vCtor
    (
        postprocessComponent,
        dictionary,
        (
            const dictionary& dict, 
            fieldsDataBase& fieldsDB, 
            const baseTimeControl& defaultTimeControl
        ),
        (dict, fieldsDB, defaultTimeControl)
    );

    /// @brief  return the time control for this post-process component
    /// @return reference to the time control object
    const postprocessTimeControl& timeControl()const
    {
        return timeControl_;
    }
    
    /// @brief  return the fields database
    /// @return const reference to the fields database
    const fieldsDataBase& database()const
    {
        return fieldsDataBase_;
    }

    /// @brief  return the fields database
    /// @return reference to the fields database
    fieldsDataBase& database()
    {
        return fieldsDataBase_;
    }

    virtual 
    word name()const = 0;

    virtual 
    regionPoints& regPoints() = 0;

    virtual 
    const regionPoints& regPoints() const = 0;

    virtual 
    bool execute(const timeInfo& ti, bool forceExecute = false) = 0;

    virtual 
    bool executed()const = 0;

    virtual 
    bool write(const fileSystem& parDir)const = 0; 

    static
    uniquePtr<postprocessComponent> create(
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl);

};

} // namespace pFlow::postprocessData

#endif // __postprocessComponent_hpp__