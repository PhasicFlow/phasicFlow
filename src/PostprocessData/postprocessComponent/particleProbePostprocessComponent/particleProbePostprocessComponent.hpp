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

#ifndef __particleProbePostprocessComponent_hpp__
#define __particleProbePostprocessComponent_hpp__

#include "postprocessComponent.hpp"
#include "fieldsDataBase.hpp"
#include "centerPointsRegionPoints.hpp"
#include "regionField.hpp"
#include "oFstream.hpp"

namespace pFlow::postprocessData
{


class particleProbePostprocessComponent
:
    public postprocessComponent
{
private:

    using processedRegFieldType = std::variant
    <
        regionField<real>,
        regionField<realx3>,
        regionField<realx4>
    >;

    bool                                executed_{false};

    uniquePtr<centerPointsRegionPoints> regionPointsPtr_;

    uniquePtr<processedRegFieldType>    processedField_ = nullptr;

    word                                fieldName_; 

    word                                name_;

    mutable uniquePtr<oFstream>         osPtr_ = nullptr;

public:

    TypeInfo("PostprocessComponent<centerPoints,particleProbe>");

    particleProbePostprocessComponent
    (
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl
    );
    
    ~particleProbePostprocessComponent()override = default;

    add_vCtor
    (
        postprocessComponent,
        particleProbePostprocessComponent,
        dictionary
    );
     
    word name()const override
    {
        return name_;
    }
    
    regionPoints& regPoints() override
    {
        return regionPointsPtr_();
    }

    const regionPoints& regPoints() const override
    {
        return regionPointsPtr_();
    }

    bool execute(const timeInfo& ti, bool forceExecute = false) override;

    bool executed() const override
    {
        return executed_;
    }

    bool write(const fileSystem& parDir)const override;

};
    
}

#endif //__particleProbePostprocessComponent_hpp__