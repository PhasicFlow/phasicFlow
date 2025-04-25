#include "PostprocessOperationSum.hpp"
#include "dictionary.hpp"
#include "fieldsDataBase.hpp"
#include "operationFunctions.hpp"

namespace pFlow::postprocessData
{

/// Constructs sum processor and initializes result field based on input field type
PostprocessOperationSum::PostprocessOperationSum
(
    const dictionary &opDict, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    postprocessOperation(opDict, regPoints, fieldsDB)
{
    if( fieldType() == getTypeName<real>() )
    {
        processedRegField_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, real(0)));
    }
    else if( fieldType() == getTypeName<realx3>() )
    {
        processedRegField_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx3(0)));
    }
    else if( fieldType() == getTypeName<realx4>() )
    {
        processedRegField_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx4(0)));
    }
    else
    {
        fatalErrorInFunction<<" in dictionary "<< opDict.globalName()
            << " field type is not supported for sum operation"
            << " field type is "<< fieldType()
            << endl;
        fatalExit;
    }
}

/// Performs weighted sum of field values within each region
bool PostprocessOperationSum::execute
(
    const std::vector<span<real>>& weights,
    const regionField<real>& volFactor
)
{
    auto allField = database().updateFieldAll(fieldName());
    auto phi = database().updateFieldReal(
        phiFieldName());

    auto mask = getMask();
    word procName = processedFieldName();
    const auto& regP = regPoints();
    bool dbVol = divideByVolume();

    processedRegField_ = makeUnique<processedRegFieldType>
    (   
        std::visit([&](auto&& field)->processedRegFieldType
            { 
                return executeSumOperation(
                procName,
                field,
                volFactor,
                dbVol,
                weights,
                phi,
                mask);
            },
            allField)
    );
   
    return true;
}


}