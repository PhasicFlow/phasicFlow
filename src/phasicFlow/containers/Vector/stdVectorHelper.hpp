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


#ifndef __stdVectorHelper_hpp__
#define __stdVectorHelper_hpp__

#include <vector>
#include <algorithm>

#include "span.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"
#include "dataIO.hpp"


namespace pFlow
{
 

template <class T>
class noConstructAllocator
    : public std::allocator<T>
{
public:
    using std::allocator<T>::allocator;

    template <class U, class... Args> void construct(U*, Args&&...) {}
};

template<typename T>
using vecAllocator = std::allocator<T>;


template<typename T>
inline
bool writeSpan(
    iOstream& os, 
    const span<T>& sp, 
    IOPattern::IOType iotype)
{
   
   auto ioPtr = dataIO::create(iotype, IOPattern::exeMode());

   if(!ioPtr)
	{
		fatalErrorInFunction;
		return false;
	}

   if(!ioPtr().writeData(os, sp))
   {
   		fatalErrorInFunction;
   		return false;
   }
    return true; 
}


template<typename T, typename Allocator>
bool writeStdVector
(
	iOstream& os, 
	const std::vector<T,Allocator>& vec,
	IOPattern::IOType iotype 
)
{
	span<T> sp( const_cast<T*>(vec.data()), vec.size());

	return writeSpan(os, sp, iotype);
}

	


template<typename T, typename Allocator>
bool readStdVector
(
	iIstream& is, 
	std::vector<T,Allocator>& vec,
	IOPattern::IOType iotype 
)
{
	auto ioPtr = dataIO::create(iotype, IOPattern::exeMode());

	if(!ioPtr)
	{
		fatalErrorInFunction;
		return false;
	}

	if(!ioPtr().readData(is, vec))
	{
		fatalErrorInFunction;
		return false;
	}
	return true;
	
}

template<typename T, typename Allocator>
iOstream& operator<<( iOstream& os, const std::vector<T,Allocator>& vec)
{
	if(!writeStdVector(os, vec, IOPattern::AllProcessorsDifferent))
	{
		fatalErrorInFunction;
		fatalExit;
	}
	return os;
}

/// Always assume ASCII is in the input stream 
template<typename T, typename Allocator>
iIstream& operator>>(iIstream& is, std::vector<T,Allocator>& vec)
{
	if( !readStdVector(is,vec, IOPattern::MasterProcessorOnly))
	{
		fatalErrorInFunction;
		fatalExit;
	}
	return is;
}

} // pFlow


#endif