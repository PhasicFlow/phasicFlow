

#ifndef __processorAB2BoundaryIntegration_hpp__
#define __processorAB2BoundaryIntegration_hpp__

#include "boundaryIntegration.hpp"

namespace pFlow
{

class processorAB2BoundaryIntegration
:
    public boundaryIntegration
{   
public:

    TypeInfo("boundaryIntegration<processor,AdamsBashforth2>");

    processorAB2BoundaryIntegration(
        const boundaryBase& boundary,
        const pointStructure& pStruct, 
        const word& method,
        integration& intgrtn
    );

    ~processorAB2BoundaryIntegration()override=default;

    
	bool correct(
        real dt, 
        const realx3PointField_D& y, 
        const realx3PointField_D& dy)override;
	

    
    bool correctPStruct(real dt, const realx3PointField_D& vel)override;
    
    
    add_vCtor(
        boundaryIntegration,
        processorAB2BoundaryIntegration,
        boundaryBase
    );

    

};

}

#endif