
#include "scatteredMasterDistributeChar.hpp"

pFlow::MPI::scatteredMasterDistribute<char>::scatteredMasterDistribute
(
    size_t  sizeOfElement,
    const localProcessors& procs
)
:
    procCommunication(procs),
    indexedMap_(TypeNull, procs, true),
    sizeOfElement_(sizeOfElement)
{}


bool pFlow::MPI::scatteredMasterDistribute<char>::setDataMaps
(
    procVector<span<uint32>>& maps
)
{
    if(this->localMaster())
    {
        if(maps.size() != this->localSize() )
        {
            fatalErrorInFunction<<"size mismatch";
            return false;
        }

        freeIndexedMap();

        std::vector<MPI_Aint> index;

        for(auto proc = 0; proc< maps.size(); proc++)
        {
            auto m = maps[proc];
            index.resize(m.size());
            for(auto i=0; i<index.size(); i++ )
            {
                index[i] = m[i]*sizeOfElement_;
            }
            
            DataType dt;
            MPI_Type_create_hindexed_block(
                m.size(), 
                sizeOfElement_, 
                index.data(), 
                MPI_BYTE, 
                &dt);
            MPI_Type_commit(&dt);
                
            indexedMap_[proc] = dt;
            
        }
    }
    
    return true;
}

bool pFlow::MPI::scatteredMasterDistribute<char>::setDataMaps
(
    procVector<span<int32>>& maps
)
{
    if(this->localMaster())
    {
        if(maps.size() != this->localSize() )
        {
            fatalErrorInFunction<<"size mismatch";
            return false;
        }

        std::vector<MPI_Aint> index;
        freeIndexedMap();
        
        for(auto proc = 0; proc< maps.size(); proc++)
        {
            
            auto m = maps[proc];
            index.resize(m.size());
            for(auto i=0; i<index.size(); i++ )
            {
                index[i] = m[i]*sizeOfElement_;
            }
            
            DataType dt;
            MPI_Type_create_hindexed_block(
                index.size(), 
                sizeOfElement_, 
                index.data(), 
                MPI_CHAR, 
                &dt);
            MPI_Type_commit(&dt);
                
            indexedMap_[proc] = dt;
            
        }
    }

    return true;
}


void pFlow::MPI::scatteredMasterDistribute<char>::freeIndexedMap()
{
    for(auto i=0; i<indexedMap_.size(); i++)
    {
        if(indexedMap_[i]!= TypeNull)
        {
            TypeFree(&indexedMap_[i]);
            indexedMap_[i] = TypeNull;
        }
    }
}

bool pFlow::MPI::scatteredMasterDistribute<char>::distribute
(
    span<char>& sendBuff, 
    span<char>& recvb
)
{
    procVector<Request> requests(processors(), true);
    procVector<Status> statuses(processors(), true);


    if(this->localMaster())
    {
        bool res = true;
        for(int32 i = indexedMap_.size()-1; i>=0; i--)
        {
            res = res&&CheckMPI(
                MPI_Issend( 
                    sendBuff.data(), 
                    1, 
                    indexedMap_[i], 
                    i, 
                    0, 
                    localCommunicator(),
                    &requests[i]), 
                false);
        }

        if(!res)return false;		
    }

    Status stat;
    bool sucss = CheckMPI( 
        MPI_Recv(
            recvb.data(), 
            recvb.size(), 
            MPI_CHAR, 
            0, 
            0, 
            localCommunicator(),
            &stat),
        true); 
    
    if(this->localMaster())
    {
        CheckMPI(
            MPI_Waitall(requests.size(), requests.data(), statuses.data()),
            false
            );
    }
    
    return sucss;
}