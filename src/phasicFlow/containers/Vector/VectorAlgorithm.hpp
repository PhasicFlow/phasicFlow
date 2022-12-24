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


namespace pFlow
{
	
template<typename T, typename Allocator>
inline auto count(const Vector<T, Allocator>& vec, const T& val)
{
	return std::count(vec.begin(), vec.end(), val);
}

template<typename T, typename Allocator, typename UnaryPredicate>
inline auto count_if(const Vector<T, Allocator>& vec, UnaryPredicate p)
{
	return std::count_if(vec.begin(), vec.end(), p);
}

template<typename T, typename Allocator>
inline void fill_n(Vector<T, Allocator>& vec, size_t n, const T& val)
{
	std::fill_n(vec.begin(), n, val);
}

template<typename T, typename Allocator>
inline void fill(Vector<T, Allocator>& vec, const T& val)
{
	std::fill(vec.begin(), vec.end(), val);
}

template<typename T, typename Allocator>
inline void fillSequence(Vector<T, Allocator>& vec, int32 start, int32 end, const T& startVal)
{
	pFlow::algorithms::STD::fillSequence<T, false>(vec.data()+start, end-start, startVal);
}

template<typename T, typename Allocator>
inline void fillSequence(Vector<T, Allocator>& vec, const T& startVal)
{
	pFlow::fillSequence(vec, 0, vec.size(), startVal);
}

template<typename T, typename Allocator>
inline void sort(Vector<T, Allocator>& vec)
{
	std::sort(vec.begin(), vec.end());
}


template<typename T, typename Allocator>
inline int64 find(Vector<T, Allocator>& vec, const T& val)
{
	ForAll( i,vec)
	{
		if ( vec[i] == val) return static_cast<int64>(i);
	}
	return -1;
}


} // pFlow

