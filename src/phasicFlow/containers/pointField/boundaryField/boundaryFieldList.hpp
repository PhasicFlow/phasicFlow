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
#include "boundaryListPtr.hpp"


namespace pFlow
{

template< class T, class MemorySpace = void >
class boundaryFieldList
:
    public boundaryListPtr< boundaryField<T, MemorySpace> >
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
        boundaryListPtr<BoundaryFieldType>(),
        boundaries_(boundaries)
    {
        ForAllBoundaries(i, *this)
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
        ForAllBoundaries(i,*this)
        {
            this->boundaryPtr(i)->updateBoundary(1, direction);        
        }

        // second step
        ForAllBoundaries(i,*this)
        {
            this->boundaryPtr(i)->updateBoundary(1, direction);
        }
        
        if(direction == DataDirection::SlaveToMaster)
        {
            slaveToMasterUpdateIter_ = iter;
        }
    }

    void fill(const T& val)
    {
        ForAllBoundaries(i, *this)
        {
            this->boundaryPtr(i)->fill(val);
        }
    }

    bool slaveToMasterUpdateRequested()const
    {
        return slaveToMasterUpdateIter_ != static_cast<uint32>(-1);
    }

    
};

}

#endif