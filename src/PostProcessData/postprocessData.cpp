
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

#include "List.hpp"
#include "systemControl.hpp"
#include "postprocessData.hpp"
#include "fileDictionary.hpp"
#include "postprocessComponent.hpp"

pFlow::postprocessData::postprocessData(const systemControl &control)
:
    auxFunctions(control),
    time_(control.time()),
    fieldsDataBase_(control.time(), true),
    dict_
    (
        objectFile
        (
            "postprocessDataDict",
            control.settings().path(),
            objectFile::READ_IF_PRESENT,
            objectFile::WRITE_NEVER
        )
    ),
    componentsDicts_(readDictList("components", dict_))
{
    // if dictionary is not provided, no extra action is required. 
    if( !dict_.fileExist() )
    {
        return;
    }

    activeInSimulation_ = dict_.getValOrSet<Logical>(
        "activeInSimulation", 
        Logical{true});

    if(dict_.containsDictionay("defaultTimeControl"))
    {
        defaultTimeControlPtr_ = 
            makeUnique<baseTimeControl>(
                dict_.subDict("defaultTimeControl"),
                "execution");
    }
    else
    {
        // default time control from settings
        defaultTimeControlPtr_ = makeUnique<baseTimeControl>(
            control.time().startTime(), 
            control.time().endTime(),
            control.time().saveInterval(),
            "execution");
    }
    
    for(auto& compDict:componentsDicts_)
    {
        postprocesses_.push_back( postprocessComponent::create(
            compDict, 
            fieldsDataBase_, 
            defaultTimeControlPtr_() ));
    }

}

bool pFlow::postprocessData::execute() 
{
    
    const auto& ti = time_.TimeInfo();

    for(auto& component:postprocesses_)
    {
        if(!component->execute(ti))
        {
            fatalErrorInFunction
                <<"Error occured in executing postprocess component: "
                <<component->name()<<endl;
            return false;
        }
    }

    return true;
}