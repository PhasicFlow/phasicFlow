

#ifndef __boundaryGrainParticlesList_hpp__
#define __boundaryGrainParticlesList_hpp__

#include "boundaryListPtr.hpp"
#include "boundaryList.hpp"
#include "boundaryGrainParticles.hpp"

namespace pFlow
{

class boundaryGrainParticlesList
:
    public boundaryListPtr<boundaryGrainParticles>
{
private:

    const boundaryList&         boundaries_;

public:

    boundaryGrainParticlesList(
        const boundaryList& bndrs,
        grainParticles& prtcls
    );

    ~boundaryGrainParticlesList()=default;

};

}



#endif 