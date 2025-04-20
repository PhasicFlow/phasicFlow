
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
#include "postprocessGlobals.hpp"
#include "postprocessComponent.hpp"

pFlow::postprocessData::postprocessData
(
    const systemControl &control,
    timeValue startTime
)
:
    auxFunctions(control),
    inSimulation_(startTime<0.0?true:false),
    time_(control.time()),
    dict_
    (
        objectFile
        (
            "postprocessDataDict",
            control.settings().path(),
            objectFile::READ_IF_PRESENT,
            objectFile::WRITE_NEVER
        )
    )
{
    postProcessGlobals::defaultDir__ = CWD()/pFlow::postProcessGlobals::defaultRelDir__;
    
    // if dictionary is not provided, no extra action is required. 
    if( !dict_.fileExist() || !dict_.headerOk() )
    {
        WARNING<<"You requested postprocessData function while,"
            <<" the dictionary system/postprocessDataDict does not exist."
            <<" This feature is disabled in the current run."<<END_WARNING;
        return;
    }
    
    fieldsDataBasePtr_= fieldsDataBase::create
        (
            const_cast<systemControl&>(control),
            dict_,
            inSimulation_,
            startTime
        );
    

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
    
    componentsDictsPtr_ = makeUnique<dictionaryList>(readDictList("components", dict_));

    for(auto& compDict:*componentsDictsPtr_)
    {
        postprocesses_.push_back( postprocessComponent::create(
            compDict, 
            fieldsDataBasePtr_(), 
            defaultTimeControlPtr_() ));
    }

}

bool pFlow::postprocessData::execute() 
{
    const auto& ti = time_.TimeInfo();

    for(auto& component:postprocesses_)
    {
        if(!component->execute(ti, !inSimulation_) )
        {
            fatalErrorInFunction
                <<"Error occurred in executing postprocess component: "
                <<component->name()<<endl;
            return false;
        }
    }

    return true;
}

bool pFlow::postprocessData::write() const
{
    for(auto& component:postprocesses_)
    {   
        if(!component->executed())
        {
            continue;
        }

        if(!component->write(postProcessGlobals::defaultDir__/component->name()))
        {
            fatalErrorInFunction
                <<"Error occured in writing postprocess component: "
                <<component->name()<<endl;
            fatalExit;
        }
    }
    return true;
}

void pFlow::postprocessData::setOutputDirectory(const fileSystem &path) const
{
    postProcessGlobals::defaultDir__ = path;
}
