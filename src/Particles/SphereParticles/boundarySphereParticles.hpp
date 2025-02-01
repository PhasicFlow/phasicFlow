

#ifndef __boundarySphereParticles_hpp__
#define __boundarySphereParticles_hpp__

#include "generalBoundary.hpp"
#include "virtualConstructor.hpp"
#include "timeInfo.hpp"

namespace pFlow
{

class sphereParticles;

class boundarySphereParticles
    : public generalBoundary
{
private:

    sphereParticles&        particles_;

public:

    /// type info
    TypeInfo("boundarySphereParticles<none>");

    boundarySphereParticles(
        const boundaryBase &boundary,
        sphereParticles&  prtcls
    );

    create_vCtor(
        boundarySphereParticles,
        boundaryBase,
        (
            const boundaryBase &boundary,
            sphereParticles&  prtcls
        ),
        (boundary, prtcls)
    );

    add_vCtor(
        boundarySphereParticles,
        boundarySphereParticles,
        boundaryBase
    );

    sphereParticles& Particles();

    const sphereParticles& Particles()const;

    bool hearChanges(
        real t,
        real dt,
        uint32 iter,
        const message &msg,
        const anyList &varList) override
    {
        return true;
    }

    virtual
    bool acceleration(const timeInfo& ti, const realx3& g)
    {
        return true;
    }

    bool isActive()const override
    {
        return false;
    }

    static
    uniquePtr<boundarySphereParticles> create(
        const boundaryBase &boundary,
        sphereParticles&  prtcls);
    
};


}



#endif
