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
#ifndef __generalBoundary_hpp__
#define __generalBoundary_hpp__

// from STL
#include <any>

// from phasicFlow
#include "types.hpp"
#include "observer.hpp"
#include "boundaryBase.hpp"

namespace pFlow
{

//- forward 
class pointStructure; 
class Time;

class generalBoundary
:
    public observer 
{
private:

    const boundaryBase&     boundary_;

    const pointStructure&   pStruct_;

    const bool              isBoundaryMaster_;

	static inline
	const message defaultMessage_{message::BNDR_RESET};

public:

	TypeInfo("generalBoundary");

	generalBoundary(
		const boundaryBase& boundary,
        const pointStructure& pStruct,
        const word&  dataType,
        const word&  option);
    
    ~generalBoundary()override = default;
	
    inline
    uint32 thisBoundaryIndex()const
    {
        return boundary_.thisBoundaryIndex();
    }

    inline
    uint32 mirrorBoundaryindex()const
    {
        return boundary_.mirrorBoundaryIndex();
    }

	inline
	auto size()const
	{
		return boundary_.size();
	}

    inline
	auto capacity()const
	{
		return boundary_.capacity();
	}

    inline 
    auto neighborProcSize()const
    {
        return boundary_.neighborProcSize();
    }

    inline 
    int neighborProcessorNo()const
    {
        return boundary_.neighborProcessorNo();
    }

    inline
    int thisProcessorNo()const
    {
        return boundary_.thisProcessorNo();
    }

    inline 
    const auto& boundary()const
    {
        return boundary_;
    }

    inline
    const auto& indexList()const
	{
		return boundary_.indexList();
	}

    inline
    const auto& mirrorBoundary()const
    {
        return boundary_.mirrorBoundary();
    }

    inline
    const word& boundaryName()const
    {
        return boundary_.name();
    }

    inline 
    const word& type()const
    {
        return boundary_.type();
    }

    inline
    const auto& pStruct()const
    {
        return pStruct_;
    }

    inline
    bool isBoundaryMaster()const
    {
        return isBoundaryMaster_;
    }

    const Time& time()const;
	
};

} // pFlow


#endif

