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


#ifndef __VectorSingleMath_hpp__
#define __VectorSingleMath_hpp__




namespace pFlow
{

template<typename T, typename MemorySpace>
INLINE_FUNCTION_H 
size_t count(const VectorSingle<T,MemorySpace>& vec, const T& val)
{
	return count( vec.deviceVectorAll(), 0, vec.size(), val);
}

template<class T, class MemorySpace>
INLINE_FUNCTION_H T min( const VectorSingle<T,MemorySpace>& vec) 
{
	return min(
		vec.deviceVectorAll(),
		0, vec.size()
		);	
}

template<class T, class MemorySpace>
INLINE_FUNCTION_H T max( const VectorSingle<T, MemorySpace>& vec) 
{
	return min(
		vec.deviceVectorAll(),
		0, vec.size()
		);		
}



}


#endif // __VectorSingleMath_hpp__
