#include "boundarySphereParticlesList.hpp"

pFlow::boundarySphereParticlesList::boundarySphereParticlesList(
    const boundaryList &bndrs, 
    sphereParticles &prtcls
)
:
    ListPtr(bndrs.size()), 
    boundaries_(bndrs)
{
    for(auto i=0; i<boundaries_.size(); i++)
    {
        this->set
        (
            i, 
            boundarySphereParticles::create(boundaries_[i], prtcls)
        );
    }
}