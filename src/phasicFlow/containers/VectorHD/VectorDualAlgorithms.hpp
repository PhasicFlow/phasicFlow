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


#ifndef __VectorDualMath_hpp__
#define __VectorDualMath_hpp__



#include "baseAlgorithms.hpp"


namespace pFlow
{



// - select the side (HostSide or DeviceSide)
template<typename side, typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 count(const VectorDual<T,MemorySpace>& vec, const T& val)
{
	if constexpr (std::is_same<side,HostSide>::value)
	{
		return count( vec.hostVectorAll(), static_cast<size_t>(0), vec.size(), val);
	}
	else
	{
		return count( vec.deviceVectorAll(), static_cast<size_t>(0), vec.size(), val);
	}

	return -1;
}

// default to device side
template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 count(const VectorDual<T,MemorySpace>& vec, const T& val)
{
	return count<DeviceSide>( vec, val);
}

template<typename side, typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 min(const VectorDual<T,MemorySpace>& vec)
{
	if constexpr (std::is_same<side,HostSide>::value)
	{
		return min( vec.hostVectorAll(), static_cast<size_t>(0), vec.size());
	}
	else
	{
		return min( vec.deviceVectorAll(), static_cast<size_t>(0), vec.size());
	}

	return 0.0;
}

// default to device side
template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 min(const VectorDual<T,MemorySpace>& vec)
{
	return min<DeviceSide>( vec);
}

template<typename side, typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 max(const VectorDual<T,MemorySpace>& vec)
{
	if constexpr (std::is_same<side,HostSide>::value)
	{
		return max( vec.hostVectorAll(), static_cast<size_t>(0), vec.size());
	}
	else
	{
		return max( vec.deviceVectorAll(), static_cast<size_t>(0), vec.size());
	}

	return 0.0;
}

// default to device side
template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
int64 max(const VectorDual<T,MemorySpace>& vec)
{
	return max<DeviceSide>( vec);
}



} // pFlow


#endif // __VectorSingleMath_hpp__
