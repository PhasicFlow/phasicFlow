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

#ifndef __PostProcessComponent_hpp__
#define __PostProcessComponent_hpp__

#include <vector>

#include "ListPtr.hpp"
#include "List.hpp"
#include "postprocessComponent.hpp"
#include "postprocessOperation.hpp"
#include "dictionaryList.hpp"
#include "fieldsDataBase.hpp"
#include "regionPoints.hpp"
#include "regionField.hpp"

namespace pFlow
{

template<typename RegionType, typename ProcessMethodType>
class PostprocessComponent
:
    public postprocessComponent
{
private:


    ListPtr<postprocessOperation> operatios_; 

    /// Region type for selecting a subset of particles for processing
    uniquePtr<RegionType>         regionPointsPtr_;
    
    /// Method for processing the selected particles data 
    std::vector<ProcessMethodType>  regionsProcessMethod_;

    regionField<real>               volumeFactor_;

    bool    executed_{false};

    dictionaryList                  operationDicts_;

protected:
    
    std::vector<ProcessMethodType>& regionProecessMethod()
    {
        return regionsProcessMethod_;
    }

    regionField<real>& volumeFactor()
    {
        return volumeFactor_;
    }
    
    const regionField<real>& volumeFactor()const
    {
        return volumeFactor_;
    }

public:

    // type info 
    TypeInfoTemplate12(
        "PostprocessComponent",
        RegionType,
        ProcessMethodType);

    PostprocessComponent(
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl);
    
    ~PostprocessComponent() override = default;

    // add the virtual constructor
    add_vCtor(
        postprocessComponent,
        PostprocessComponent,
        dictionary
    );   

    word name()const override
    {
        return operationDicts_.parrent().name();
    }

    regionPoints& regPoints() override
    {
        return static_cast<regionPoints&>(regionPointsPtr_());
    }

    const regionPoints& regPoints()const override
    {
        return static_cast<const regionPoints&>(regionPointsPtr_());
    }

    bool execute(const timeInfo& ti, bool forceUpdate = false) override;

    bool executed()const override
    {
        return executed_;
    }

    bool write(const fileSystem& parDir)const override;

};



}

#include "PostprocessComponent.cpp"

#endif