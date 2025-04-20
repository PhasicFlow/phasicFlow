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

#ifndef __simulationFieldsDataBase_hpp__
#define __simulationFieldsDataBase_hpp__

#include "fieldsDataBase.hpp"

namespace pFlow
{

class simulationFieldsDataBase 
: 
    public fieldsDataBase
{
private:

    const   shape&       shape_;

protected:
    
    /// check if pointField name exists in Time or time folder 
    bool pointFieldNameExists(const word& name)const override;
 

    /// Loads a pointField with a given name to the Time object. 
    /// For simulation, it just checks if the name exists 
    bool loadPointFieldToTime(const word& name) override;

    /// Loads pointStructure to the Time object
    /// For simulation, it just checks if the name exists
    bool loadPointStructureToTime() override;

    const shape& getShape() const override;  

    word getPointFieldType(const word& name)const override;
    
public:

    TypeInfo("fieldsDataBase<simulation>");

    simulationFieldsDataBase(
        systemControl& control, 
        const dictionary& postDict,
        bool inSimulation,
        timeValue startTime);

    ~simulationFieldsDataBase() override = default;

    add_vCtor
    (
        fieldsDataBase,
        simulationFieldsDataBase,
        bool
    );

    const pointStructure& pStruct()const override;

};


}


#endif //__simulationFieldsDataBase_hpp__