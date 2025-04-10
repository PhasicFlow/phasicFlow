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
#ifndef __postprocessData_hpp__
#define __postprocessData_hpp__

#include "auxFunctions.hpp"
#include "Logical.hpp"
#include "ListPtr.hpp"
#include "fileDictionary.hpp"
#include "baseTimeControl.hpp"
#include "fieldsDataBase.hpp"
#include "postprocessComponent.hpp"
#include "dictionaryList.hpp"

namespace pFlow 
{

class systemControl;
class Time;
class timeInfo;

/**
 * @class postprocessData
 * @brief An interface class for handling post-processing of simulation data.
 * 
 * This class provides methods and utilities to process and analyze
 * simulation data during/after simulation.
 */
class postprocessData
:
  public auxFunctions
{
    /// Indicates if a post-processing is active during simulatoin 
    Logical                 activeInSimulation_{false};

    /// a list of active post-process components 
    ListPtr<postprocessComponent>    postprocesses_;

    /// const ref to Time
    const Time&           time_;

    /// Database for all the points fields on the host 
    fieldsDataBase        fieldsDataBase_;

    /// file dictionary that is constructed from the file (postProcessDataDict)
    fileDictionary        dict_;

    /// list of dictionaries for postprocess components 
    dictionaryList        componentsDicts_;

    /// @brief  default time control that can be used for all post-process components
    uniquePtr<baseTimeControl> defaultTimeControlPtr_= nullptr;

public:

    TypeInfo("postprocessData");

    /// @brief  Construct from systemControl and a boolean flag
    ///  this constructor is used when postprocesing is active
    ///  during simulation.
    /// @param control const reference to systemControl 
    postprocessData(const systemControl& control);

    ~postprocessData()override = default;

    add_vCtor
    (
        auxFunctions,
        postprocessData,
        systemControl
    );

    bool execute() override;
    
    
     
    bool write()const override;
    

};

} // namespace pFlow

#endif // __postprocessData_hpp__