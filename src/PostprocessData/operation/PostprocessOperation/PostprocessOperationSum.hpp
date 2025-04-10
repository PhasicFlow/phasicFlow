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

#ifndef __PostprocessOperationSum_hpp__
#define __PostprocessOperationSum_hpp__

#include <variant>
#include <vector>

#include "postprocessOperation.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"

namespace pFlow
{


class PostprocessOperationSum
:
    public postprocessOperation
{
private:
           
    /// Pointer to the include mask used for masking operations.
    uniquePtr<processedRegFieldType>  processedRegField_ = nullptr;

public:

    TypeInfo("PostprocessOperation<sum>");

    PostprocessOperationSum(
        const dictionary& opDict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);
    
    ~PostprocessOperationSum() override = default;

    add_vCtor
    (
        postprocessOperation,
        PostprocessOperationSum,
        dictionary
    );

    const processedRegFieldType& processedField()const override
    {
        return processedRegField_();
    }

    bool execute(const std::vector<span<real>>& weights) override;

}; 


}

#endif //__PostprocessOperation_hpp__