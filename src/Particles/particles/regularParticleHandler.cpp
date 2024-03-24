#include "regularParticleHandler.hpp"



pFlow::regularParticleHandler::regularParticleHandler
(
    uint32PointField_D & id
)
:
    particleIdHandler(id)
{
}

pFlow::Pair<pFlow::uint32, pFlow::uint32> 
    pFlow::regularParticleHandler::getIdRange(uint32 nNewParticles)
{
    uint32 startId;
    if(maxId_==-1)
    {
        startId = 0;
    }
    else
    {
        startId = maxId_+1;
    }
    uint32 endId = startId+nNewParticles-1;
    maxId_ = endId;
    return {startId, endId};
}

bool pFlow::regularParticleHandler::initialIdCheck()
{
    /// empty point structure / no particles in simulation
    if( id().pStruct().empty() ) return true;

    uint32 maxId = max(id());
    
    /// particles should get ids from 0 to size-1
    if(maxId == -1)
    {
        fillSequence(id(),0u);
        maxId_ = id().size()-1;
    }
    else
    {
        maxId_ = maxId;
    }

    return true;
}
