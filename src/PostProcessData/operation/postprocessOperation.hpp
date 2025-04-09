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
#ifndef __postprocessOperation_hpp__
#define __postprocessOperation_hpp__

#include "virtualConstructor.hpp"
#include "Logical.hpp"
#include "dictionary.hpp"
#include "span.hpp"
#include "includeMask.hpp"


namespace pFlow
{

class fieldsDataBase;
class regionPoints;

/*!
 *  @brief Base class for post-processing operations.
 *         This class provides the basic structure and functionality
 *         for performing post-processing operations on simulation data.
 */
class postprocessOperation
{
public:

    using Mask = typename includeMask::Mask;

private:

    /// Dictionary containing operation-specific parameters.
    dictionary              operationDict_;

    /// This Threshold is used to exclude the regions which contain
    /// fewer than this value. 
    uint32                  threshold_;
    
    /// Logical flag to determine if the result is divided by volume.
    Logical                 divideByVolume_;

    /// Reference to the region points used in the operation.
    const regionPoints&     regionPoints_;

    /// Reference to the fields database containing field data.
    fieldsDataBase&         database_;

    /// Name of the field to be processed.
    word                    fieldName_;

    /// Type of the field to be processed.
    word                    fieldType_;

    /// Name of the phi field to be processed.
    word                     phiFieldName_;

    /// Pointer to the include mask used for masking operations.
    uniquePtr<includeMask>  includeMask_ = nullptr;
    
public:


    TypeInfo("postprocessOperation");

    postprocessOperation(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB );

    virtual ~postprocessOperation()=default;

    create_vCtor(
        postprocessOperation,
        dictionary,
        (
            const dictionary& opDict,
            const regionPoints& regPoints,
            fieldsDataBase& fieldsDB
        ),
        (opDict, regPoints, fieldsDB));
    
    const regionPoints& regPoints()const
    {
        return regionPoints_;
    }

    const fieldsDataBase& database()const
    {
        return database_;
    }

    fieldsDataBase& database()
    {
        return database_;
    }

    word processedFieldName()const
    {
        return operationDict_.name();
    }
    
    const word& fieldName()const
    {
        return fieldName_;
    }

    const word& fieldType()const
    {
        return fieldType_;
    }

    const word& phiFieldName()const
    {
        return phiFieldName_;
    }

    const dictionary& operationDict()const
    {
        return operationDict_;
    }
    const uint32 threshold()const
    {
        return threshold_;
    }

    bool divideByVolume()const
    {
        return divideByVolume_();
    }

    Mask getMask()
    {
        return includeMask_().getMask();
    }


    virtual 
    bool execute(const std::vector<span<real>>& weights) = 0;

    static
    uniquePtr<postprocessOperation> create(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
};

}

#endif //__postprocessOperation_hpp__