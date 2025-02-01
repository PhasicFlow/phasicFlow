#include "boundarySphereParticlesList.hpp"

pFlow::boundarySphereParticlesList::boundarySphereParticlesList(
    const boundaryList &bndrs, 
    sphereParticles &prtcls
)
:
    boundaryListPtr(), 
    boundaries_(bndrs)
{
    ForAllBoundariesPtr(i, this)
    {
        this->set
        (
            i, 
            boundarySphereParticles::create(boundaries_[i], prtcls)
        );
    }
}