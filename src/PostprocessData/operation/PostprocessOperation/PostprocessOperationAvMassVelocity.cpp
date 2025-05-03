#include "PostprocessOperationAvMassVelocity.hpp"

namespace pFlow::postprocessData
{

PostprocessOperationAvMassVelocity::PostprocessOperationAvMassVelocity
(
    const dictionary &opDict, 
    const regionPoints &regPoints, 
    fieldsDataBase &fieldsDB
)
:
    PostprocessOperationAverage
    (
        opDict,
        opDict.getValOrSet<word>("velocityName", "velocity"),
        opDict.getValOrSet<word>("massName", "mass"),
        "all",
        regPoints,
        fieldsDB
    )
{
}

}