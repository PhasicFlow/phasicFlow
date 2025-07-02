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

#ifndef __PostprocessComponentGaussian_hpp__
#define __PostprocessComponentGaussian_hpp__

#include "PostprocessComponent.hpp"
#include "GaussianDistribution.hpp"
#include "numericConstants.hpp"

namespace pFlow::postprocessData
{

template<typename RegionType>
class PostprocessComponentGaussian
:
    public PostprocessComponent<RegionType, GaussianDistribution>
{
public:

    /// type info
    TypeInfoTemplate12("PostprocessComponent", RegionType, GaussianDistribution);

    PostprocessComponentGaussian
    (
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl
    )
    :
        PostprocessComponent<RegionType,GaussianDistribution>(dict, fieldsDB, defaultTimeControl)
    {

        this->regPoints().applyRegionExtension();
        auto d = this->regPoints().eqDiameters();
        auto c = this->regPoints().centers();
        auto& regs = this->regionProecessMethod();
        auto& volFactor = this->volumeFactor();

        const uint32 n = d.size();
        for(uint32 i=0; i<n; i++)
        {
            auto r = d[i]/2;
            regs[i] = GaussianDistribution(c[i], pow(r/3.0,2));
            volFactor[i] = 0.677683 / (4.0/3.0*Pi*r);
        }
    }

    // add the virtual constructor
    add_vCtor
    (
        postprocessComponent,
        PostprocessComponentGaussian,
        dictionary
    );

};

}


#endif //__PostprocessComponentGaussian_hpp__
