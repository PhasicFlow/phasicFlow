#include "processorAB2BoundaryIntegration.hpp"
#include "AdamsBashforth2.hpp"
#include "AB2Kernels.hpp"
#include "boundaryConfigs.hpp"

pFlow::processorAB2BoundaryIntegration::processorAB2BoundaryIntegration(
    const boundaryBase &boundary, 
    const pointStructure &pStruct,  
    const word &method,
    integration& intgrtn
)
:
    boundaryIntegration(boundary, pStruct, method, intgrtn)
{}

bool pFlow::processorAB2BoundaryIntegration::correct(
    real dt, 
	const realx3PointField_D& y, 
	const realx3PointField_D& dy
)
{

#ifndef BoundaryModel1
    if(this->isBoundaryMaster())
    {
        const uint32 thisIndex = thisBoundaryIndex();
		const auto&  AB2 =  static_cast<const AdamsBashforth2&>(Integration());
        const auto& dy1View = AB2.BoundaryField(thisIndex).neighborProcField().deviceView();
		const auto& dyView = dy.BoundaryField(thisIndex).neighborProcField().deviceView();
		const auto& yView = y.BoundaryField(thisIndex).neighborProcField().deviceView();        
        const rangeU32 aRange(0u, dy1View.size());
        return  AB2Kernels::intAllActive(
			"AB2Integration::correct."+this->boundaryName(), 
			dt,
            aRange, 
			yView, 
			dyView, 
			dy1View
		); 
    }
#endif //BoundaryModel1
    
    
    return true;
}

bool pFlow::processorAB2BoundaryIntegration::correctPStruct(real dt, const realx3PointField_D &vel)
{

	#ifndef BoundaryModel1
    if(this->isBoundaryMaster())
    {
        const uint32 thisIndex = thisBoundaryIndex();
		const auto&  AB2 =  static_cast<const AdamsBashforth2&>(Integration());
        const auto& dy1View = AB2.BoundaryField(thisIndex).neighborProcField().deviceView();
		const auto& velView = vel.BoundaryField(thisIndex).neighborProcField().deviceView();
		const auto& xposView = boundary().neighborProcPoints().deviceView();        
        const rangeU32 aRange(0u, dy1View.size());
        return  AB2Kernels::intAllActive(
			"AB2Integration::correctPStruct."+this->boundaryName(), 
			dt,
            aRange, 
			xposView, 
			velView, 
			dy1View
		); 
    }
#endif //BoundaryModel1
    
    return true;
}
