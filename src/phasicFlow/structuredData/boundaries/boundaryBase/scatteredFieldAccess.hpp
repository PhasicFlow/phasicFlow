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

#ifndef __scatteredFieldAccess_hpp__
#define __scatteredFieldAccess_hpp__


#include "phasicFlowKokkos.hpp"

namespace pFlow
{


template<typename T, typename MemorySpace=void>
class scatteredFieldAccess
{
public:
	
	using viewType 			= ViewType1D<T, MemorySpace>;

    using device_type 		= typename viewType::device_type;

    using memory_space 		= typename viewType::memory_space;

    using execution_space 	= typename viewType::execution_space;

private:

	uint32 								size_ = 0;

	ViewType1D<uint32, memory_space> 	indices_;

	viewType 		fieldVals_;

public:

    scatteredFieldAccess():
        indices_(),
        fieldVals_()    
    {}

	scatteredFieldAccess(
		uint32 	sz,
		const ViewType1D<uint32, memory_space>& ind,
		const ViewType1D<T, memory_space>& fVals)
	:
		size_(sz),
		indices_(ind),
		fieldVals_(fVals)
	{}

	scatteredFieldAccess(const scatteredFieldAccess&) = default;

	scatteredFieldAccess(scatteredFieldAccess&&) = default;

	scatteredFieldAccess& operator=(const scatteredFieldAccess&) = default;

	scatteredFieldAccess& operator=(scatteredFieldAccess&&) = default;

	~scatteredFieldAccess() = default;

	// - Methods

    INLINE_FUNCTION_HD
	T& operator()(uint32 i)
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	const T& operator()(uint32 i)const
	{
		return fieldVals_(indices_(i));
	}

    INLINE_FUNCTION_HD
	T& operator[](uint32 i)
	{
		return fieldVals_[indices_[i]];
	}

    INLINE_FUNCTION_HD
	const T& operator[](uint32 i)const
	{
		return fieldVals_[indices_[i]];
	}

	INLINE_FUNCTION_HD
	const viewType& field()const
	{
		return fieldVals_;
	}

	INLINE_FUNCTION_HD
	viewType& field()
	{
		return fieldVals_;
	}

	uint32 index(uint32 i)const
	{
		return indices_[i];
	}

    INLINE_FUNCTION_HD
	uint32 	size()const
	{
		return size_;
	}

    INLINE_FUNCTION_HD
	bool empty()const
	{
		return size_ == 0;
	}

	T getFirstCopy()const
	{
		T val;
		uint32 n = indices_(0);
		getNth(val, fieldVals_, n);
		return val;
	}

	T getLastCopy()const 
	{
		T val;
		if(empty())return val;
		uint32 n = indices_(size()-1);
		getNth(val, fieldVals_, n);
		return val;
	}

};

template<typename T>
	using deviceScatteredFieldAccess = 
		scatteredFieldAccess<T, typename DefaultExecutionSpace::memory_space>;

template<typename T>
	using hostScatteredFieldAccess = 
		scatteredFieldAccess<T,HostSpace>;

} // pFlow


#endif //__scatteredFieldAccess_hpp__

