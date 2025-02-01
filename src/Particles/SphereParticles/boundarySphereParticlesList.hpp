

#ifndef __boundarySphereParticlesList_hpp__
#define __boundarySphereParticlesList_hpp__

#include "boundaryListPtr.hpp"
#include "boundaryList.hpp"
#include "boundarySphereParticles.hpp"

namespace pFlow
{

class boundarySphereParticlesList
:
    public boundaryListPtr<boundarySphereParticles>
{
private:

    const boundaryList&         boundaries_;

public:

    boundarySphereParticlesList(
        const boundaryList& bndrs,
        sphereParticles& prtcls
    );

    ~boundarySphereParticlesList()=default;

};

}



#endif 