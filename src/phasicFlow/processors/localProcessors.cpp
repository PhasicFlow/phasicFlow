#include "localProcessors.hpp"

pFlow::localProcessors::localProcessors(const word &name)
:
    processors(),
    localSize_(processors::globalSize()),
    localRank_(processors::globalRank()),
    name_(name)
{
#ifdef pFlow_Build_MPI
    parrentCommunicator_ = pFlow::MPI::CommWorld;
    localCommunicator_ = pFlow::MPI::CommWorld;
#endif

}

pFlow::localProcessors::localProcessors
(
    const word &name, 
    const std::vector<int> &ranks
)
:
    processors(),
    name_(name)
{
#ifdef pFlow_Build_MPI
    parrentCommunicator_ = pFlow::MPI::CommWorld;
    
    if(ranks.size()> this->globalSize() )
    {
        fatalErrorInFunction<<
        "the size of ranks is larger than parrent communicator size "<<endl;
        fatalExit;
    }

    pFlow::MPI::Group globalGroup;
    MPI_Comm_group(parrentCommunicator_, &globalGroup);

    pFlow::MPI::Group localGroup;
    MPI_Group_incl(globalGroup, ranks.size(), ranks.data(), &localGroup);
 
    // Create the new communicator from that group of processes.
    
    CheckMPI( MPI_Comm_create(parrentCommunicator_, localGroup, &localCommunicator_), true);
    
    isPartOfLocal_ = localCommunicator_ != MPI::CommNull;
    
    if(isPartOfLocal_)
    {
        MPI_Comm_set_name(localCommunicator_, name.c_str());

        MPI_Comm_size(localCommunicator_, &localSize_);
        MPI_Comm_rank(localCommunicator_, &localRank_);
    }

#else

    if(ranks.size()> 1 )
    {
        fatalErrorInFunction<< 
        "rank size for processors should not be greater than 1.";
        fatalExit;
    }
    localSize_ = processors::globalSize();
    localRank_ = processors::globalRank();

#endif
}

#ifdef pFlow_Build_MPI

pFlow::localProcessors::localProcessors
(
    pFlow::MPI::Comm parrentComm, 
    const word &name, 
    const std::vector<int> &ranks
)
:
    processors(),
    name_(name)
{
    parrentCommunicator_ = parrentComm;
    
    int parSize;

    MPI_Comm_size(parrentCommunicator_, &parSize);

    if(ranks.size()> parSize )
    {
        fatalErrorInFunction<<
        "the size of ranks is larger than parrent communicator size "<<endl;
        fatalExit;
    }

    pFlow::MPI::Group parGroup;
    MPI_Comm_group(parrentCommunicator_, &parGroup);

    pFlow::MPI::Group localGroup;
    MPI_Group_incl(parGroup, ranks.size(), ranks.data(), &localGroup);
 
    // Create the new communicator from that group of processes.
    
    CheckMPI( MPI_Comm_create(parrentCommunicator_, localGroup, &localCommunicator_), true);
    
    isPartOfLocal_ = localCommunicator_ != MPI::CommNull;
    
    if(isPartOfLocal_)
    {
        MPI_Comm_set_name(localCommunicator_, name.c_str());

        MPI_Comm_size(localCommunicator_, &localSize_);
        MPI_Comm_rank(localCommunicator_, &localRank_);
    }

}

#endif