#include "particleProbePostprocessComponent.hpp"

pFlow::particleProbePostprocessComponent::particleProbePostprocessComponent
(
    const dictionary &dict, 
    fieldsDataBase &fieldsDB, 
    const baseTimeControl &defaultTimeControl
)
:
    postprocessComponent(dict, fieldsDB, defaultTimeControl),
    regionPointsPtr_
    (
        makeUnique<centerPointsRegionPoints>(dict, fieldsDB)
    ),
    name_(dict.name())
{}

bool pFlow::particleProbePostprocessComponent::execute
(
    const timeInfo &ti, 
    bool forceExecute
)
{
    
    return false;
}
