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

#ifndef __PostprocessOperationSolidVolFraction_hpp__
#define __PostprocessOperationSolidVolFraction_hpp__

/*!
 * @class PostprocessOperationSolidVolFraction
 * @brief Calculates solid volume fraction in the regions
 * 
 *
 * @see PostprocessOperationSum
 */

#include "PostprocessOperationSum.hpp"

namespace pFlow::postprocessData
{

class PostprocessOperationSolidVolFraction
:
    public PostprocessOperationSum
{

public:

    TypeInfo("PostprocessOperation<solidVolFraction>");

    /// @brief Constructs average operation processor
    /// @param opDict Operation parameters dictionary
    /// @param regPoints Region points data
    /// @param fieldsDB Fields database
    PostprocessOperationSolidVolFraction(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
    /// destructor 
    ~PostprocessOperationSolidVolFraction() override = default;

    /// add this virtual constructor to the base class
    add_vCtor
    (
        postprocessOperation,
        PostprocessOperationSolidVolFraction,
        dictionary
    );

    bool divideByVolume()const override
    {
        return true;
    }

};

} 

#endif //__PostprocessOperationSolidVolFraction_hpp__


