
#ifndef __boundaryIntegrationList_hpp__
#define __boundaryIntegrationList_hpp__


#include "boundaryList.hpp"
#include "boundaryListPtr.hpp"
#include "boundaryIntegration.hpp"


namespace pFlow
{

class integration;

class boundaryIntegrationList
:
    public boundaryListPtr<boundaryIntegration>
{
private:

    const boundaryList&         boundaries_;

public:

    boundaryIntegrationList(
        const pointStructure& pStruct, 
        const word& method,
		integration& intgrtn
    );

    ~boundaryIntegrationList()=default;

    bool correct(
		real dt, 
		realx3PointField_D& y, 
		realx3PointField_D& dy);

    bool correctPStruct(
        real dt, 
        pointStructure& pStruct, 
        const realx3PointField_D& vel);

};



}


#endif //__boundaryIntegrationList_hpp__