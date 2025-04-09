#include "PostprocessComponent.hpp"
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

template<typename RegionType, typename ProcessMethodType>
pFlow::PostprocessComponent<RegionType,ProcessMethodType>::PostprocessComponent
(
    const dictionary& dict,
    fieldsDataBase& fieldsDB,
    const baseTimeControl& defaultTimeControl
)
:
    postprocessComponent(dict, fieldsDB, defaultTimeControl),
    regionPointsPtr_
    (
        makeUnique<RegionType>(dict, fieldsDB)
    ),
    regionsProcessMethod_
    (
        regionPointsPtr_().size()
    ),
    operationDicts_(readDictList("operations", dict))
{

    for(auto& opDict:operationDicts_)
    {
        operatios_.push_back
        (
            postprocessOperation::create
            (
                opDict,
                regionPointsPtr_(), 
                this->database()
            )
        );
    }
}


template <typename RegionType, typename ProcessMethodType>
bool pFlow::PostprocessComponent<RegionType, ProcessMethodType>::execute
(
    const timeInfo &ti, 
    bool forceUpdate
)
{
    
    if( !timeControl().eventTime(ti))
    {
        executed_ = false;
        return true;
    }

    // update processing methods
    
    auto centers = this->regPoints().centers();
    const uint32 n     = centers.size();
    auto points  = this->database().updatePoints();
    
    for(uint32 i=0; i<n; i++)
    {
        auto indices = this->regPoints().indices(i);
        regionsProcessMethod_[i].updateWeights(
            centers[i],
            indices,
            points);
    }

    std::vector<span<real>> weights(n);

    for(uint32 i=0; i<n; i++)
    {
        // get the span of weight of each region 
        weights[i] = regionsProcessMethod_[i].getWeights();
    }
    
    for(auto& op:operatios_)
    {
        if( op->execute(weights) )
        {
            fatalErrorInFunction
                <<"error occured in executing operatoin defined in dict "
                << op->operationDict()
                <<endl;
            return false;
        }
    }

    executed_ = true;

    return true;
}