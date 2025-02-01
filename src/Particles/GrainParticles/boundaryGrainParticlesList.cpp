#include "boundaryGrainParticlesList.hpp"

pFlow::boundaryGrainParticlesList::boundaryGrainParticlesList(
    const boundaryList &bndrs, 
    grainParticles &prtcls
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
            boundaryGrainParticles::create(boundaries_[i], prtcls)
        );
    }
    
}