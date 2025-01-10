

#ifndef __boundarySphereParticlesList_hpp__
#define __boundarySphereParticlesList_hpp__

#include "ListPtr.hpp"
#include "boundaryList.hpp"
#include "boundarySphereParticles.hpp"

namespace pFlow
{

class boundarySphereParticlesList
:
    public ListPtr<boundarySphereParticles>
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