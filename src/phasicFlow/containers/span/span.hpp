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

/// 
/// @class span
/// @brief Represents a view into a contiguous sequence of elements.
/// 
/// `span` provides a lightweight way to work with arrays and other contiguous 
/// sequences without the overhead of copying. This will word on both GPU and CPU
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

private:

    mutable T*     data_ = nullptr;
 
    index          size_ = 0;

public:
    
    TypeInfoTemplateNV11("span", T);

    /// Constructor with no arguments
    INLINE_FUNCTION_HD
    span() = default;


    /// Constructor that takes a pointer to the beginning of the data and the size of the span
    INLINE_FUNCTION_HD
    span(T* data, uint32 size)
        : data_(data), size_(size)
    {}

    /// copy constructor
    INLINE_FUNCTION_HD
    span(const span&) = default;

    /// copy assignment operator
    INLINE_FUNCTION_HD
    span& operator=(const span&) = default;

    /// move constructor
    INLINE_FUNCTION_HD
    span(span&&) = default;

    /// move assignment operator
    INLINE_FUNCTION_HD
    span& operator=(span&) = default;

    
    INLINE_FUNCTION_HD
    bool empty() const
    {
        return size_ == 0;
    }

    /// Returns a pointer to the beginning of the data
    INLINE_FUNCTION_HD
    T* data() const
    {
        return data_;
    }

    /// Returns the number of elements in the span
    INLINE_FUNCTION_HD
    index size() const
    {
        return size_;
    }

    /// Returns an iterator to the beginning of the span
    INLINE_FUNCTION_HD
    constIterator begin() const
    {
        return data_;
    }

    /// Returns an iterator to the beginning of the span (const version)
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

    /// Returns an iterator to one past the end of the span (const version)
    INLINE_FUNCTION_HD
    constIterator cend() const
    {
        return data_ + size_;
    }

    /// Returns a reference to the element at the specified index
    INLINE_FUNCTION_HD
    T& operator[](index i)
    {
        return data_[i];
    }

    /// Returns a const reference to the element at the specified index
    INLINE_FUNCTION_HD
    T& operator[](index i)const
    {
        return data_[i];
    }

};


/// Creates a span of char from a span of any type.
/// 
/// This helper function reinterprets the underlying memory of the given span 
/// as a span of char.
template<typename T>
inline
span<char> charSpan(span<T> s)
{
	auto el = sizeof(T);
	return span<char>(
		reinterpret_cast<char*>(s.data()),
		s.size()*el);
}

/// Creates a span of const char from a span of const any type.
/// 
/// This helper function reinterprets the underlying memory of the given span 
/// as a span of const char.
template<typename T>
inline
span<const char> charSpan(span<const T> s)
{
	auto el = sizeof(T);
	return span<const char>(
		reinterpret_cast<const char*>(s.data()),
		s.size()*el);
}


} // pFlow

#endif //__span_hpp__