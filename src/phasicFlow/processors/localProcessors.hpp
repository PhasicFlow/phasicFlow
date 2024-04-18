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
#ifndef __pFlowProcessors_hpp__
#define __pFlowProcessors_hpp__


#include "processors.hpp"
#include "types.hpp"

#ifdef pFlow_Build_MPI
    #include "mpiTypes.hpp"
#endif


namespace pFlow
{

class localProcessors
:
    public processors
{
private:

#ifdef pFlow_Build_MPI
    pFlow::MPI::Comm    parrentCommunicator_;

    pFlow::MPI::Comm    localCommunicator_;
#endif

    int localSize_ = 1 ;

    int localRank_ = 0;

    bool isPartOfLocal_ = true;

    word name_;

public:

        /// @brief Construct a local processor communication similar 
        /// to global with specified name
        /// @param name a given name to the local communicator 
        localProcessors(const word& name);

        
        /// @brief Construct a local processor communication from global
        /// communicator using a name and ranks 
        /// @param name a given name to the local communicator 
        /// @param ranks ranks to be included in the new communicator 
        localProcessors(
            const word& name, 
            const std::vector<int>& ranks);

    #ifdef pFlow_Build_MPI
        localProcessors(
            pFlow::MPI::Comm parrentComm, 
            const word& name, 
            const std::vector<int>& ranks);
    #endif

    // - Access 

        /// @brief return name of the local communicator 
        inline 
        const auto& name()const
        {
            return name_;
        }

        inline
        auto localRank()const
        {
            return localRank_;
        }

        inline 
        auto localSize()const
        {
            return localSize_;
        }

    // - Methods 

        inline
        bool localMaster()const
        {
            return localRank_ == localMasterNo();
        }

        inline 
        int localMasterNo()const
        {
            return 0;
        }

        inline 
        bool localParallel()const
        {
            return localSize_>1;
        }

        inline
        const char* localRunTypeName()
        {
            if(localParallel())
                return "MPI";
            else
                return "regular";
        }

        inline
        bool isThisActive()const
        {
            return isPartOfLocal_;
        }

    #ifdef pFlow_Build_MPI
        inline 
        auto parrentCommunicator()const
        {
            return parrentCommunicator_;
        }

        inline 
        auto localCommunicator()const
        {
            return localCommunicator_;
        }
    #endif

};

}

#endif