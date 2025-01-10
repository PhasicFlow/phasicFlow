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

#ifndef __boundaryFieldList_hpp__
#define __boundaryFieldList_hpp__

#include "boundaryField.hpp"
#include "boundaryList.hpp"
#include "ListPtr.hpp"


namespace pFlow
{

template< class T, class MemorySpace = void >
class boundaryFieldList
:
    public ListPtr< boundaryField<T, MemorySpace> >
{
public:

    using BoundaryFieldType = boundaryField<T, MemorySpace>;

    using InternalFieldType = typename BoundaryFieldType::InternalFieldType;
    

protected:

    const boundaryList&         boundaries_;

    uint32                      slaveToMasterUpdateIter_ = static_cast<uint32>(-1);

public:

    boundaryFieldList(const boundaryList& boundaries, InternalFieldType& internal)
    :
        ListPtr<BoundaryFieldType>(boundaries.size()),
        boundaries_(boundaries)
    {
        for(auto i=0; i<boundaries.size(); i++)
        {
            this->set
            (
                i, 
                BoundaryFieldType::create(boundaries_[i], boundaries_.pStruct() ,internal)
            );
        }
    }

    void updateBoundaries(uint32 iter, DataDirection direction)
    {
        if( direction == DataDirection::SlaveToMaster 
            && slaveToMasterUpdateIter_ == iter) return;
        
        
        
        // first step
        uint32 i=0;
        for(auto b:*this)
        {
            if(i==0 ) 
            {
                //pOutput<<"request for update boundaries for field "<< b->name()<<endl;
                i++;
            }
            b->updateBoundary(1, direction);
        }

        // second step
        for(auto b:*this)
        {
            b->updateBoundary(2, direction);
        }

        if(direction == DataDirection::SlaveToMaster)
        {
            slaveToMasterUpdateIter_ = iter;
        }
    }

    void fill(const T& val)
    {
        for(auto& bf: *this)
        {
            bf->fill(val);
        }
    }

    bool slaveToMasterUpdateRequested()const
    {
        return slaveToMasterUpdateIter_ != static_cast<uint32>(-1);
    }

    
};

}

#endif