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

#ifndef __postSimulationFieldsDataBase_hpp__
#define __postSimulationFieldsDataBase_hpp__

#include "fieldsDataBase.hpp"
#include "timeFolder.hpp"
#include "pointStructure.hpp"
#include "ListPtr.hpp"
#include "property.hpp"

namespace pFlow::postprocessData
{

class postSimulationFieldsDataBase
:
    public postprocessData::fieldsDataBase
{
    
    systemControl&              control_;

    timeFolder                  allValidFolders_;

    Map<word,word>              currentFileFields_;

    uniquePtr<pointStructure>   pStructPtr_ = nullptr;

    ListPtr<IOobject>           allPointFields_;

    mutable uniquePtr<shape>    shapePtr_ = nullptr;

    mutable uniquePtr<property> propertyPtr_ = nullptr;

    bool setToCurrentFolder();

protected:
    
    bool pointFieldNameExists(const word& name)const override;

    bool loadPointFieldToTime(const word& name) override;

    bool loadPointStructureToTime();

    const shape& getShape() const override;

    word getPointFieldType(const word& name)const override;

public:

    TypeInfo("fieldsDataBase<postSimulation>");

    postSimulationFieldsDataBase(
        systemControl& control,
        const dictionary& postDict,
        bool inSimulation,
        TimeValueType startTime);

    ~postSimulationFieldsDataBase() override = default;

    add_vCtor
    (
        fieldsDataBase,
        postSimulationFieldsDataBase,
        bool
    );

    const pointStructure& pStruct()const override;

    TimeValueType getNextTimeFolder()const override;

    TimeValueType setToNextTimeFolder() override;

    TimeValueType skipNextTimeFolder() override;
    
};

} // namespace pFlow

#endif // __postSimulationFieldsDataBase_hpp__
