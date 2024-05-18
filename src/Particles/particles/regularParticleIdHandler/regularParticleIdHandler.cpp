#include "regularParticleIdHandler.hpp"



pFlow::regularParticleIdHandler::regularParticleIdHandler
(
    pointStructure& pStruct
)
:
    particleIdHandler(pStruct)
{
    initialIdCheck();
}

pFlow::Pair<pFlow::uint32, pFlow::uint32> 
    pFlow::regularParticleIdHandler::getIdRange(uint32 nNewParticles)
{
    
    if(nNewParticles==0) return {0,0};

    uint32 startId;
    if(maxId_== static_cast<uint32>(-1))
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

bool pFlow::regularParticleIdHandler::initialIdCheck()
{
    /// empty point structure / no particles in simulation
    if( pStruct().empty() ) return true;

    uint32 maxId = max( *this );
    
    /// particles should get ids from 0 to size-1
    if(maxId == static_cast<uint32>(-1))
    {
        fillSequence(*this,0u);
        maxId_ = size()-1;
    }
    else
    {
        maxId_ = maxId;
    }

    return true;
}
