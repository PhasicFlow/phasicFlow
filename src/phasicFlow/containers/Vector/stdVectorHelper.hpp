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
#include "pFlowProcessors.hpp"

namespace pFlow
{
 

template<typename T>
inline
span<T> makeSpan(std::vector<T>& container)
{
    return span<T>(container.data(), container.size());
}

template<typename T>
inline
span<T> makeSpan(const std::vector<T>& container)
{
    return span<T>(
		const_cast<T*>(container.data()), 
		container.size());
}


template<typename T>
inline
bool writeSpan(
    iOstream& os, 
    span<T> sp)
{
	return writeDataAsciiBinary(os, sp);
}

template<typename T>
inline
bool writeSpan(
    iOstream& os, 
    span<T> sp, 
    const IOPattern& iop)
{
   
   auto ioPtr = dataIO<T>::create(iop);
       
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
	const std::vector<T,Allocator>& vec
)
{
	auto sp = makeSpan(vec);
	return writeSpan(os, sp);
}

template<typename T, typename Allocator>
bool writeStdVector
(
	iOstream& os, 
	const std::vector<T,Allocator>& vec,
	const IOPattern& iop 
)
{
	auto sp = makeSpan(vec);
	return writeSpan(os, sp, iop);
}

template<typename T, typename Allocator>
bool readStdVector
(
	iIstream& is, 
	std::vector<T,Allocator>& vec
)
{

	return readDataAsciiBinary(is, vec);
	
}

template<typename T, typename Allocator>
bool readStdVector
(
	iIstream& is, 
	std::vector<T,Allocator>& vec,
	const IOPattern& iop 
)
{
	auto ioPtr = dataIO<T>::create(iop);
	
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
	if(!writeStdVector(os, vec))
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
	if( !readStdVector(is,vec))
	{
		fatalErrorInFunction;
		fatalExit;
	}
	return is;
}

} // pFlow


#endif