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

#ifndef __PostprocessComponentUniform_hpp__
#define __PostprocessComponentUniform_hpp__

#include "PostprocessComponent.hpp"
#include "uniformDistribution.hpp"

namespace pFlow
{

template<typename RegionType>
class PostprocessComponentUniform
:
    public PostprocessComponent<RegionType, uniformDistribution>
{
public:

    /// type info
    TypeInfoTemplate12("PostprocessComponent", RegionType, uniformDistribution);

    PostprocessComponentUniform
    (
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl
    )
    :
        PostprocessComponent<RegionType,uniformDistribution>(dict, fieldsDB, defaultTimeControl)
    {
        /// initializes the Uniform distribution for all elements of region
        //const uint32 n = this->regPoints().size();
        auto d = this->regPoints().eqDiameters();
        auto c = this->regPoints().centers();
        auto& regs = this->regionProecessMethod();
        const uint32 n = d.size();
        for(uint32 i=0; i<n; i++)
        {
            regs[i] = uniformDistribution();
        }
    }

    // add the virtual constructor
    add_vCtor(
        postprocessComponent,
        PostprocessComponentUniform,
        dictionary
    );

};

}


#endif //__PostprocessComponentUniform_hpp__
