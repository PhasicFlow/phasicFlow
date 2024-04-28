

template<typename T>
pFlow::MPI::scatteredMasterDistribute<T>::scatteredMasterDistribute
(
    const localProcessors& procs
)
:
    procCommunication(procs),
    indexedMap_(TypeNull, procs, true)
{
    
}

template<typename T>
bool pFlow::MPI::scatteredMasterDistribute<T>::setDataMaps
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

        std::vector<int32> index;

        freeIndexedMap();

        for(auto proc = 0; proc< maps.size(); proc++)
        {
            auto m = maps[proc];
            index.resize(m.size());
            for(auto i=0; i<index.size(); i++ )
            {
                index[i] = m[i];
            }

            DataType dt;
            
            if(! typeCreateIndexedBlock<T>( makeSpan(index), dt)) 
            {
                fatalErrorInFunction;
                return false;
            }
            else
            {
                indexedMap_[proc] = dt;
            }
        }
    }
    return true;
}


template<typename T>
bool pFlow::MPI::scatteredMasterDistribute<T>::setDataMaps
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
        
        freeIndexedMap();
        

        for(auto proc = 0; proc< maps.size(); proc++)
        {
            DataType dt;
            if( !typeCreateIndexedBlock<T>(maps[proc], dt) )
            {
                fatalErrorInFunction;
                return false;
            }
            else
            {
                indexedMap_[proc] = dt;
            }
        }
    }
    return true;
}

template<typename T>
void pFlow::MPI::scatteredMasterDistribute<T>::freeIndexedMap()
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


template<typename T>
bool pFlow::MPI::scatteredMasterDistribute<T>::distribute
(
    span<T>& sendBuff, 
    span<T>& recvb
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
            recvb.size()*sFactor<T>(), 
            Type<T>(), 
            0, 
            0, 
            localCommunicator(),
            &stat),
        false);
            
    if(this->localMaster())
    {
        CheckMPI(
            MPI_Waitall(requests.size(), requests.data(), statuses.data()),
            false
            );
    }

    return sucss;
}