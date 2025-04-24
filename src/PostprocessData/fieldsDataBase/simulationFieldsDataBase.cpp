#include "Time.hpp"
#include "simulationFieldsDataBase.hpp"
#include "dynamicPointStructure.hpp"
#include "vocabs.hpp"

namespace pFlow
{
    bool pointFieldGetType(const word& TYPENAME, word& fieldType, word& fieldSpace);
}

bool pFlow::postprocessData::simulationFieldsDataBase::pointFieldNameExists(const word &name) const
{
    return time().lookupObjectName(name);
}

bool pFlow::postprocessData::simulationFieldsDataBase::loadPointFieldToTime(const word &name)
{
    return time().lookupObjectName(name);
}

bool pFlow::postprocessData::simulationFieldsDataBase::loadPointStructureToTime()
{
    // it is already in the Time object 
    return time().lookupObjectName(pointStructureFile__);
}


const pFlow::shape& pFlow::postprocessData::simulationFieldsDataBase::getShape() const
{
    return shape_;
}

pFlow::word pFlow::postprocessData::simulationFieldsDataBase::getPointFieldType
(
    const word &name
) const
{
    word pfType =  time().lookupObjectTypeName(name);
    word type, space;
    if(!pointFieldGetType(pfType, type, space))
    {
        fatalErrorInFunction
            <<"Error in retriving the type of pointField "
            << pfType<<endl;
        fatalExit;
    }
    return type;
}

pFlow::postprocessData::simulationFieldsDataBase::simulationFieldsDataBase
(
    systemControl &control,
    const dictionary& postDict, 
    bool inSimulation,
    timeValue startTime
)
:
    fieldsDataBase(control, postDict, inSimulation, startTime),
    shape_
    (
        dynamic_cast<const shape&>(*control.caseSetup().lookupObjectPtr(shapeFile__))
    )
{
}

const pFlow::pointStructure &pFlow::postprocessData::simulationFieldsDataBase::pStruct() const
{
    return 
    static_cast<const pointStructure&>
    (
        time().lookupObject<dynamicPointStructure>(pointStructureFile__)
    );
}

