#include "MPIParticleIdHandler.hpp"
#include "procCommunication.hpp"

pFlow::MPI::MPIParticleIdHandler::MPIParticleIdHandler
(
    pointStructure& pStruct
)
:
    particleIdHandler(pStruct)
{
    initialIdCheck();
}

pFlow::Pair<pFlow::uint32, pFlow::uint32> 
    pFlow::MPI::MPIParticleIdHandler::getIdRange(uint32 nNewParticles)
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

bool pFlow::MPI::MPIParticleIdHandler::initialIdCheck()
{
    /// empty point structure / no particles in simulation
    uint32 maxId = -1;
    if( !pStruct().empty() ) 
    {
        maxId = max( *this );
    }    
    
    auto maxIdAll = procVector<uint32>(pFlowProcessors());
    auto numAll = procVector<uint32>(pFlowProcessors());
    auto comm = procCommunication(pFlowProcessors());

    comm.collectAllToAll(maxId, maxIdAll);
    comm.collectAllToAll(size(),numAll);
    
    uint32 n = 0;
    for(uint32 i=0; i<maxIdAll.size(); i++)
    {
        if( maxIdAll[i]==-1 && numAll[i]!= 0)
        {
            if(comm.localRank() == i)
            {
                fillSequence(*this, n);
                maxId_ = size()-1 + n;
            }
            
        }
        else
        {
            if(comm.localRank() == i)
            {
                maxId_ = maxIdAll[i];
            }
        }
        n += numAll[i];
    }
    
    return true;
}
