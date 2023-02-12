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

#ifndef __span_hpp__
#define __span_hpp__

#include "types.hpp"
#include "iOstream.hpp"

namespace pFlow {


template<typename T>
class span
{

public:
	
	using iterator        = T*;

  	using constIterator   = const T*;
  	
	using reference       = T&;
  	
  	using constReference  = const T&;

	using valueType       = T;
  	
  	using pointer         = T*;
  	
  	using constPointer    = const T*;

protected:

    T* data_ 		= nullptr;

    label 	size_ 	= 0;

public:
    
    TypeInfoTemplateNV("span", T);

    /// Constructor
    INLINE_FUNCTION_HD
    span() = default;


    INLINE_FUNCTION_HD
    span(T* data, label size)
        : data_(data), size_(size)
    {}

    /// copy
    INLINE_FUNCTION_HD
    span(const span&) = default;

    /// assignment
    INLINE_FUNCTION_HD
    span& operator=(const span&) = default;

    /// move
    INLINE_FUNCTION_HD
    span(span&&) = delete;

    /// assignment
    INLINE_FUNCTION_HD
    span& operator=(span&) = delete;

    
    INLINE_FUNCTION_HD
    bool empty() const
    {
        return size_ == 0;
    }

    INLINE_FUNCTION_HD
    T* data() const
    {
        return data_;
    }

    /// Returns the number of elements in the span
    INLINE_FUNCTION_HD
    label size() const
    {
        return size_;
    }

    /// Returns an iterator to the beginning of the span
    INLINE_FUNCTION_HD
    constIterator begin() const
    {
        return data_;
    }

    /// Returns an iterator to the beginning of the span
    INLINE_FUNCTION_HD
    constIterator cbegin() const
    {
        return data_;
    }

    /// Returns an iterator to one past the end of the span
    INLINE_FUNCTION_HD
    constIterator end() const
    {
        return data_ + size_;
    }

    /// Returns an iterator to one past the end of the span
    INLINE_FUNCTION_HD
    constIterator cend() const
    {
        return data_ + size_;
    }

    INLINE_FUNCTION_HD
    T& operator[](int32 i)
    {
    	return data_[i];
    }

    INLINE_FUNCTION_HD
    const T& operator[](int32 i)const
    {
    	return data_[i];
    }

    INLINE_FUNCTION_HD
    T& operator[](label i)
    {
    	return data_[i];
    }

    INLINE_FUNCTION_HD
    const T& operator[](label i)const
    {
    	return data_[i];
    }

};

template<typename T>
inline 
iOstream& operator<<(iOstream& os, const span<T>& s)
{
    os << token::BEGIN_LIST;
    for(size_t i=0; i<s.size(); i++)
    {
        os << s[i]<<token::NL;
    }
    
    os << token::END_LIST;

    os.check(FUNCTION_NAME);

    return os;
}

} // pFlow

#endif //__span_hpp__
