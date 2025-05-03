#include "boundaryIntegrationList.hpp"
#include "integration.hpp"

pFlow::boundaryIntegrationList::boundaryIntegrationList(
    const pointStructure &pStruct, 
    const word &method, 
    integration &intgrtn
)
:
    boundaryListPtr<boundaryIntegration>(),
	boundaries_(pStruct.boundaries())
{
	
	ForAllBoundariesPtr(i, this)
	{
		this->set(
			i,
			boundaryIntegration::create(
				boundaries_[i],
				pStruct, 
				method,
				intgrtn
			)
		);
	}	
}

bool pFlow::boundaryIntegrationList::correct(real dt, realx3PointField_D &y, realx3PointField_D &dy)
{
	ForAllActiveBoundariesPtr(i,this)
	{
		if(!boundaryPtr(i)->correct(dt, y, dy))
		{
			fatalErrorInFunction<<"Error in correcting boundary "<<
			boundaryPtr(i)->boundaryName()<<endl;
			return false;
		}
	}
    return true;
}

bool pFlow::boundaryIntegrationList::correctPStruct(
	real dt, 
	pointStructure &pStruct, 
	const realx3PointField_D &vel)
{
	ForAllActiveBoundariesPtr(i,this)
	{
		if(!boundaryPtr(i)->correctPStruct(dt, vel))
		{
			fatalErrorInFunction<<"Error in correcting boundary "<<
			boundaryPtr(i)->boundaryName()<<" in pointStructure."<<endl;
			return false;
		}
	}
	
    return true;
}
