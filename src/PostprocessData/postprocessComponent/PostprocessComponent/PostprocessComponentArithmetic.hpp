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

#ifndef __PostprocessComponentArithmetic_hpp__
#define __PostprocessComponentArithmetic_hpp__

#include "PostprocessComponent.hpp"
#include "arithmetic.hpp"

namespace pFlow
{

template<typename RegionType>
class PostprocessComponentArithmetic
:
    public PostprocessComponent<RegionType, arithmetic>
{
public:

    /// type info
    TypeInfoTemplate12("PostprocessComponent", RegionType, arithmetic);

    PostprocessComponentArithmetic  
    (
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl
    )
    :
        PostprocessComponent<RegionType, arithmetic>(dict, fieldsDB, defaultTimeControl)
    {
        /// initializes the arithmetic distribution for all elements of region
        //const uint32 n = this->regPoints().size();
        auto d = this->regPoints().eqDiameters();
        auto c = this->regPoints().centers();
        auto& regs = this->regionProecessMethod();
        auto& volFactor = this->volumeFactor();
        const uint32 n = d.size();
        for(uint32 i=0; i<n; i++)
        {
            regs[i] = arithmetic(); // Changed from uniformDistribution() to arithmetic()
            volFactor[i] = 1.0;
        }
    }

    // add the virtual constructor
    add_vCtor(
        postprocessComponent,
        PostprocessComponentArithmetic,
        dictionary
    );

};

}


#endif //__PostprocessComponentArithmetic_hpp__
