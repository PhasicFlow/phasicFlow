

#ifndef __boundaryGrainParticles_hpp__
#define __boundaryGrainParticles_hpp__

#include "generalBoundary.hpp"
#include "virtualConstructor.hpp"
#include "timeInfo.hpp"

namespace pFlow
{

class grainParticles;

class boundaryGrainParticles
    : public generalBoundary
{
private:

    grainParticles&        particles_;

public:

    /// type info
    TypeInfo("boundaryGrainParticles<none>");

    boundaryGrainParticles(
        const boundaryBase &boundary,
        grainParticles&  prtcls
    );

    create_vCtor(
        boundaryGrainParticles,
        boundaryBase,
        (
            const boundaryBase &boundary,
            grainParticles&  prtcls
        ),
        (boundary, prtcls)
    );

    add_vCtor(
        boundaryGrainParticles,
        boundaryGrainParticles,
        boundaryBase
    );

    grainParticles& Particles();

    const grainParticles& Particles()const;

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

    static
    uniquePtr<boundaryGrainParticles> create(
        const boundaryBase &boundary,
        grainParticles&  prtcls);
    
};


}



#endif
