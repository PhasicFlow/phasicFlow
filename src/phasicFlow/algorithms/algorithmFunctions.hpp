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


#ifndef __algorithmFunctions_hpp__
#define __algorithmFunctions_hpp__

#include "pFlowMacros.hpp"

namespace pFlow::algorithms
{

template<typename T>
 struct greater
 {
   INLINE_FUNCTION_HD
   bool operator()(const T &lhs, const T &rhs) const {
   	return lhs > rhs; }
 };

template<typename T>
struct less
{
   INLINE_FUNCTION_HD
   bool operator()(const T &lhs, const T &rhs) const {
    return lhs < rhs; }
};


template<typename T>
struct maximum
{
  INLINE_FUNCTION_HD
  bool operator()(const T &lhs, const T &rhs) const {
    return lhs < rhs ? rhs : lhs; }
};

template<typename T>
struct minimum
{
  INLINE_FUNCTION_HD
  bool operator()(const T &lhs, const T &rhs) const {
      return lhs < rhs ? lhs : rhs; }
};


template<typename T>
INLINE_FUNCTION_HD
int binarySearch(const T* array, int length, const T& val)
{
    
    int low = 0;            
    int high = length - 1;  

    while (low <= high) 
    {
        int mid = low + (high - low)/2;
        
        if ( array[mid] > val)
        { 
            high = mid - 1;  
        }
        else if ( array[mid] < val)
        { 
            low = mid + 1;        
        }
        else 
        {           
            return mid; 
        }
    }

    return -1; // val not found in array[0, length)
}


} 

#endif // __algorithmFunctions_hpp__