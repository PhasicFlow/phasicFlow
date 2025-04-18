#include "Time.hpp"
#include "simulationFieldsDataBase.hpp"
#include "dynamicPointStructure.hpp"
#include "vocabs.hpp"

namespace pFlow
{
    bool pointFieldGetType(const word& TYPENAME, word& fieldType, word& fieldSpace);
}

bool pFlow::simulationFieldsDataBase::pointFieldNameExists(const word &name) const
{
    return time().lookupObjectName(name);
}

bool pFlow::simulationFieldsDataBase::loadPointFieldToTime(const word &name)
{
    return time().lookupObjectName(name);
}

bool pFlow::simulationFieldsDataBase::loadPointStructureToTime()
{
    // it is already in the Time object 
    return time().lookupObjectName(pointStructureFile__);
}

bool pFlow::simulationFieldsDataBase::checkTimeFolder(const word &fieldName) const
{
    return true;
}

const pFlow::shape& pFlow::simulationFieldsDataBase::getShape() const
{
    return shape_;
}

pFlow::simulationFieldsDataBase::simulationFieldsDataBase
(
    systemControl &control, 
    bool inSimulation
)
:
    fieldsDataBase(control, inSimulation),
    shape_
    (
        dynamic_cast<const shape&>(*control.caseSetup().lookupObjectPtr(shapeFile__))
    )
{
}

const pFlow::pointStructure &pFlow::simulationFieldsDataBase::pStruct() const
{
    return 
    static_cast<const pointStructure&>
    (
        time().lookupObject<dynamicPointStructure>(pointStructureFile__)
    );
}

void pFlow::simulationFieldsDataBase::resetTimeFolder()
{
}
