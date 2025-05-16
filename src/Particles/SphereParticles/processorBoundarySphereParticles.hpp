#ifndef __processorBoundarySphereParticles_hpp__
#define __processorBoundarySphereParticles_hpp__

#include "boundarySphereParticles.hpp"

namespace pFlow
{

class processorBoundarySphereParticles
:
    public boundarySphereParticles
{

public:

    /// type info
    TypeInfo("boundarySphereParticles<MPI,processor>");

    processorBoundarySphereParticles(
        const boundaryBase &boundary,
        sphereParticles&  prtcls
    );

    add_vCtor(
        boundarySphereParticles,
        processorBoundarySphereParticles,
        boundaryBase
    );

    bool acceleration(const timeInfo& ti, const realx3& g)override;
    

};

}


#endif