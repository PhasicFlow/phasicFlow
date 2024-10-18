#include "boundaryIntegrationList.hpp"
#include "integration.hpp"

pFlow::boundaryIntegrationList::boundaryIntegrationList(
    const pointStructure &pStruct, 
    const word &method, 
    integration &intgrtn
)
:
    ListPtr<boundaryIntegration>(6),
	boundaries_(pStruct.boundaries())
{
    
    for(uint32 i=0; i<6; i++)
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
	for(auto& bndry:*this)
	{
		if(!bndry->correct(dt, y, dy))
		{
			fatalErrorInFunction<<"Error in correcting boundary "<<
			bndry->boundaryName()<<endl;
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
	for(auto& bndry:*this)
	{
		if(!bndry->correctPStruct(dt, vel))
		{
			fatalErrorInFunction<<"Error in correcting boundary "<<
			bndry->boundaryName()<<" in pointStructure."<<endl;
			return false;
		}
	}
    return true;
}
