
#ifndef __boundaryIntegration_hpp__
#define __boundaryIntegration_hpp__


#include "generalBoundary.hpp"
#include "virtualConstructor.hpp"
#include "pointFields.hpp"

namespace pFlow
{

class integration;

class boundaryIntegration
:
    public generalBoundary
{
private:

	const integration& 		integration_;

public:

    TypeInfo("boundaryIntegration<none>");

    boundaryIntegration(
        const boundaryBase& boundary,
        const pointStructure& pStruct, 
        const word& method,
        integration& intgrtn);

    ~boundaryIntegration()override = default;

    create_vCtor(
        boundaryIntegration,
        boundaryBase,
        (
            const boundaryBase& boundary,
            const pointStructure& pStruct, 
            const word& method,
			integration& intgrtn
        ),
        (boundary, pStruct, method, intgrtn)
    );
    
    add_vCtor(
        boundaryIntegration,
        boundaryIntegration,
        boundaryBase
    );

    bool hearChanges(
        real t,
        real dt,
        uint32 iter,
        const message &msg,
        const anyList &varList) override
    {
        return true;
    }

	const integration& Integration()const
	{
		return integration_;
	}

	virtual
	bool correct(real dt, const realx3PointField_D& y, const realx3PointField_D& dy)
	{
		return true;
	}

    virtual
    bool correctPStruct(real dt, const realx3PointField_D& vel)
    {
        return true;
    }

    static
    uniquePtr<boundaryIntegration> create(
        const boundaryBase& boundary,
        const pointStructure& pStruct, 
        const word& method,
		integration& intgrtn);

};

}

#endif