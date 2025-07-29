#include "PostprocessOperationSolidVolFraction.hpp"

namespace pFlow::postprocessData
{

PostprocessOperationSolidVolFraction::PostprocessOperationSolidVolFraction
(
    const dictionary &opDict, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    PostprocessOperationSum
    (
        opDict,
        "volume",
        "one",
        "all",
        regPoints,
        fieldsDB
    )
{
}

}