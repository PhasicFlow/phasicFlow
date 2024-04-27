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
#ifndef __gatherMaster_hpp__
#define __gatherMaster_hpp__

#include <numeric>

#include "procCommunication.hpp"
#include "stdVectorHelper.hpp"

namespace pFlow::MPI
{

template<typename T>
class gatherMaster
:
    public procCommunication
{
protected:

    std::vector<T> buffer_;

public:

    gatherMaster(const localProcessors& procs)
    :
        procCommunication(procs)
    {}

    span<T> getData()
    {
        if(this->localMaster())
            return span<T>( buffer_.data(), buffer_.size());
        else
            return span<T>(nullptr, 0);
    }

    std::vector<T> moveData()
    {
        return std::move(buffer_);
    }

    bool gatherData(span<T> data)
    {
        int thisN = data.size();

        bool succss;
        
        procVector<int> numElems(this->processors(), true);
        procVector<int> displ(this->processors(), true);      
        
        if( !this->collectAllToMaster(thisN, numElems) )
        {
            fatalErrorInFunction<<
            "error in collecting number of elements from processors"<<endl;
            return false;
        }
        auto totalN = std::accumulate(
            numElems.begin(), 
            numElems.end(),
            static_cast<int>(0));
        
        buffer_.resize(totalN);
    
        std::exclusive_scan(
            numElems.begin(), 
            numElems.end(),
            displ.begin(),
            0);
        
        auto bufferSpan = span<T>(this->buffer_.data(),this->buffer_.size() );

        return CheckMPI( 
            Gatherv(
                data, 
                bufferSpan, 
                numElems.getSpan(), 
                displ.getSpan(), 
                this->localMasterNo(), 
                this->localCommunicator()),
            false);
        
    }


};
}

#endif