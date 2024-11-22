#include "boundaryGrainParticlesList.hpp"

pFlow::boundaryGrainParticlesList::boundaryGrainParticlesList(
    const boundaryList &bndrs, 
    grainParticles &prtcls
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
            boundaryGrainParticles::create(boundaries_[i], prtcls)
        );
    }
}