#include "PostprocessOperationBulkDensity.hpp"

namespace pFlow::postprocessData
{

PostprocessOperationBulkDensity::PostprocessOperationBulkDensity
(
    const dictionary &opDict, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    PostprocessOperationSum
    (
        opDict,
        "mass",
        "one",
        "all",
        regPoints,
        fieldsDB
    )
{
}

}