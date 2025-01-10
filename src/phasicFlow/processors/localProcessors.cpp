/*------------------------------- phasicFlow ---------------------------------
    O        C enter of
   O O       E ngineering and
  O   O      M ultiscale modeling of
 OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/
#include "localProcessors.hpp"

pFlow::localProcessors::localProcessors(const word &name)
:
    processors(),
    localSize_(processors::globalSize()),
    localRank_(processors::globalRank()),
    name_(name)
{
#ifdef pFlow_Build_MPI
    parrentCommunicator_ = MPI_COMM_WORLD;
    localCommunicator_ = MPI_COMM_WORLD;
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
    parrentCommunicator_ = MPI_COMM_WORLD;
    
    if(ranks.size()> this->globalSize() )
    {
        fatalErrorInFunction<<
        "the size of ranks is larger than parrent communicator size "<<endl;
        fatalExit;
    }

    MPI_Group globalGroup;
    MPI_Comm_group(parrentCommunicator_, &globalGroup);

    MPI_Group localGroup;
    MPI_Group_incl(globalGroup, ranks.size(), ranks.data(), &localGroup);
 
    // Create the new communicator from that group of processes.
    
    CheckMPI( MPI_Comm_create(parrentCommunicator_, localGroup, &localCommunicator_), true);
    
    isPartOfLocal_ = localCommunicator_ != MPI_COMM_NULL;
    
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
    MPI_Comm parrentComm, 
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

    MPI_Group parGroup;
    MPI_Comm_group(parrentCommunicator_, &parGroup);

    MPI_Group localGroup;
    MPI_Group_incl(parGroup, ranks.size(), ranks.data(), &localGroup);
 
    // Create the new communicator from that group of processes.
    
    CheckMPI( MPI_Comm_create(parrentCommunicator_, localGroup, &localCommunicator_), true);
    
    isPartOfLocal_ = localCommunicator_ != MPI_COMM_NULL;
    
    if(isPartOfLocal_)
    {
        MPI_Comm_set_name(localCommunicator_, name.c_str());

        MPI_Comm_size(localCommunicator_, &localSize_);
        MPI_Comm_rank(localCommunicator_, &localRank_);
    }

}

#endif