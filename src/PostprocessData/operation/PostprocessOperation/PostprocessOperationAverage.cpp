#include "PostprocessOperationAverage.hpp"
#include "dictionary.hpp"
#include "fieldsDataBase.hpp"
#include "operationFunctions.hpp"

namespace pFlow::postprocessData
{

/// Constructs average processor and initializes result field based on input field type
PostprocessOperationAverage::PostprocessOperationAverage
(
    const dictionary &opDict, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    postprocessOperation(opDict, regPoints, fieldsDB),
    calculateFluctuation2_(opDict.getValOrSet<Logical>("fluctuation2", Logical(false)))
{
    if( fieldType() == getTypeName<real>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, real(0)));
    }
    else if( fieldType() == getTypeName<realx3>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx3(0)));
    }
    else if( fieldType() == getTypeName<realx4>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx4(0)));
    }
    else
    {
        fatalErrorInFunction<<" in dictionary "<< opDict.globalName()
            << " field type is not supported for average operation"
            << " field type is "<< fieldType()
            << endl;
        fatalExit;
    }
}

PostprocessOperationAverage::PostprocessOperationAverage
(
    const dictionary &opDict, 
    const word &fieldName, 
    const word &phiName,
    const word &includeName, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    postprocessOperation(opDict, fieldName, phiName, includeName, regPoints, fieldsDB),
    calculateFluctuation2_(opDict.getValOrSet<Logical>("fluctuation2", Logical(false)))
{
    if( fieldType() == getTypeName<real>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, real(0)));
    }
    else if( fieldType() == getTypeName<realx3>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx3(0)));
    }
    else if( fieldType() == getTypeName<realx4>() )
    {
        processedRegFieldPtr_ = makeUnique<processedRegFieldType>(
            regionField(processedFieldName(), regPoints, realx4(0)));
    }
    else
    {
        fatalErrorInFunction<<" in dictionary "<< opDict.globalName()
            << " field type is not supported for average operation"
            << " field type is "<< fieldType()
            << endl;
        fatalExit;
    }
}


/// Performs weighted average of field values within each region
bool PostprocessOperationAverage::execute
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

    processedRegFieldPtr_ = makeUnique<processedRegFieldType>
    (   
        std::visit([&](auto&& field)->processedRegFieldType
            { 
                return executeAverageOperation(
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
    
    if(calculateFluctuation2_())
    {
        auto& processedRegField = processedRegFieldPtr_();
        fluctuation2FieldPtr_ = makeUnique<processedRegFieldType>
        (
            std::visit([&](auto& field)->processedRegFieldType
            { 
                using T = typename std::decay_t<std::remove_reference_t< decltype(field)>>::valueType;
                if constexpr( std::is_same_v<T,real> ||
                              std::is_same_v<T,realx3>||
                              std::is_same_v<T,realx4>)
                {
                    return executeFluctuation2Operation(
                        procName,
                        field,
                        std::get<regionField<T>>(processedRegField),
                        volFactor,
                        dbVol,
                        weights,
                        mask);
                }
            },
            allField)
        );
    }
    

    return true;
}

bool PostprocessOperationAverage::write(const fileSystem &parDir) const
{   
    if(! postprocessOperation::write(parDir))
    {
        return false;
    }
    if(!calculateFluctuation2_())
    {
        return true;
    }
    
    auto ti = time().TimeInfo();

    if(!os2Ptr_)
    {
        fileSystem path = parDir+(
            processedFieldName()+"_prime2" + ".Start_" + ti.timeName());
        os2Ptr_ = makeUnique<oFstream>(path);
        
        regPoints().write(os2Ptr_());
    }

    
    std::visit
    (
        [&](auto&& arg)->bool
        {
            return writeField(os2Ptr_(), ti.t(), arg, threshold());
        },
        fluctuation2FieldPtr_()
    );

    return true;
}

} // namespace pFlow::postprocessData