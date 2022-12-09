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


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator+ (const Vector<T, Allocator>& op1, const T& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator+ (const T& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator+ (const Vector<T, Allocator>& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator - (const Vector<T, Allocator>& op1, const T& op2 );

template<typename T, typename Allocator>
inline Vector<T, Allocator> operator - (const T& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator - (const Vector<T, Allocator>& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator* (const Vector<T, Allocator>& op1, const T& op2 );

template<typename T, typename Allocator>
inline Vector<T, Allocator> operator* (const T& op1, const Vector<T, Allocator>& op2 );

template<typename T, typename Allocator>
inline Vector<T, Allocator> operator* (const Vector<T, Allocator>& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator / (const Vector<T, Allocator>& op1, const T& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator / (const T& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline Vector<T, Allocator> operator / (const Vector<T, Allocator>& op1, const Vector<T, Allocator>& op2 );


template<typename T, typename Allocator>
inline auto count(const Vector<T, Allocator>& vec, const T& val);

template<typename T, typename Allocator, typename UnaryPredicate>
inline auto count_if(const Vector<T, Allocator>& vec, UnaryPredicate p);

template<typename T, typename Allocator>
inline void fill_n(Vector<T, Allocator>& vec, size_t n, const T& val);

template<typename T, typename Allocator>
inline void fill(Vector<T, Allocator>& vec, const T& val);

template<typename T, typename Allocator>
inline void sort(Vector<T, Allocator>& vec);
