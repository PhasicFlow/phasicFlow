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

#ifndef __symArray_H__
#define __symArray_H__

#include "KokkosTypes.H"


#include "types.H"
#include "typeInfo.H"
#include "Vector.H"


/*
stores the elemnt of a symetric array in the following order in a 1D vector

 0  1  2  3
    4  5  6
       7  8  
          9

*/

namespace pFlow
{


template<typename Type>
INLINE_FUNCTION_HD
void SWAP(Type &x, Type& y)
{
	auto temp = y;
	y = x;
	x = temp;
}

template<typename T, typename MemorySpace=void>
class symArray
{

	using SymArrayType	= symArray<T, MemorySpace>;

  	using iterator        = T*;

  	using constIterator   = const T*;
  	
	using reference       = T&;
  	
  	using constReference  = const T&;

	using valueType       = T;
  	
  	using pointer         = T*;
  	
  	using constPointer    = const T*;
  	
  	// type defs related to Kokkos 
  	using ViewType 			= ViewType1D<T,MemorySpace>;

  	using deviceType 		= typename ViewType::device_type;

  	using memory_space 		= typename ViewType::memory_space;

  	using execution_space 	= typename ViewType::execution_space;

  	
protected:

	uint32 			n_=0;

	ViewType 		view_;


	constexpr static inline const char* memoerySpaceName()
  	{
  		return memory_space::name();
  	}

public:

	// - type info
	TypeNameTemplateNV2("symArray", T, memoerySpaceName());

	//// constructors 
	INLINE_FUNCTION_H
	symArray();

	INLINE_FUNCTION_H
	symArray(uint32 n)
	:
		symArray("symArray",n)
	{}

	INLINE_FUNCTION_H
	symArray(word name, uint32 n)
	:
		n_(n),
		view_(name, numElem(n))
	{}

	INLINE_FUNCTION_H
	symArray(word name, uint32 n , const T& val)
	:
		symArray(name, n)
	{
		this->fill(val);
	}

	INLINE_FUNCTION_H
	symArray(word name, Vector<T> src)
	:
		view_(name)
	{
		if( !assign(src))
		{
			fatalErrorInFunction<<
			"error in creating symArray " << name << endl;
			fatalExit;
		}
	}

	INLINE_FUNCTION_H
	symArray(const symArray&) = default;

	INLINE_FUNCTION_H
	symArray& operator=(const symArray&) = default;

	INLINE_FUNCTION_H
	symArray(symArray&&) = delete;

	INLINE_FUNCTION_H
	symArray& operator=(symArray&&) = delete;

	INLINE_FUNCTION_H
	~symArray()=default;

	
	//// - Methods
	void fill(const T& val)
	{
		if(n_==0)return;
		Kokkos::deep_copy(view_, val);
	}

	INLINE_FUNCTION_HD
	T& operator()(uint32 i, uint32 j)
	{
		if(i>j) SWAP(i,j);
		return view_(j+i*n_-numElem(i));
	}

	INLINE_FUNCTION_HD
	const T& operator()(uint32 i, uint32 j)const
	{
		if(i>j) SWAP(i,j);
		return view_(j+i*n_-numElem(i));
	}

	bool assign(const Vector<T> src)
	{
		uint32 nElem = src.size();
		uint32 n;
		if( !getN(nElem,n))
		{
			fatalErrorInFunction<<
			"size of input vector do not match a symetric array "<< nElem<<endl;
			return false;
		}

		if(n == 0 )
		{
			n_ = 0;
			return true;
		}

		if( n > n_ )
			Kokkos::realloc(view_, nElem );

		n_ = n;
		hostViewType1D<const T> temp(src.data(), nElem );
		Kokkos::deep_copy(view_, temp);

		return true;
	}

	
	//// IO operations 

		FUNCTION_H
		bool read(iIstream& is)
		{
			Vector<T> vecFromFile;
			if( !vecFromFile.read(is) ) return false;

			this->assign(vecFromFile);

			return true;
		}

		FUNCTION_H
		bool write(iOstream& os)const
		{
			
			int32 s = numElem(n_);
			Vector<T, noConstructAllocator<T>> vecToFile(s);
			
			const auto dVec = Kokkos::subview(view_, kPair<int32,int32>(0, s));
			hostViewType1D<T> mirror(vecToFile.data(), vecToFile.size());
			Kokkos::deep_copy(mirror,dVec);

			return vecToFile.write(os);
		}


		INLINE_FUNCTION_HD
		static uint32 numElem(uint32 n)
		{
			if(n==0)return 0;
			return n*(n+1)/2;
		}
		
		static bool getN(uint32 nElem, uint32& n)
		{
			real nr = 0.5*(sqrt(8.0*nElem+1)-1);
			n = static_cast<uint32>(nr);
			if( equal(nr-static_cast<real>(n) , 0.0) ) return true;

			return false;
		}


};

template<typename T, typename MemorySpace>
inline iIstream& operator >> (iIstream & is, symArray<T, MemorySpace> & iArr )
{
	if( !iArr.read(is) )
	{
		ioErrorInFile (is.name(), is.lineNumber());
		fatalExit;
	}
	return is;
}

template<typename T, typename MemorySpace>
inline iOstream& operator << (iOstream& os, const symArray<T, MemorySpace>& oArr )
{
	
	if( !oArr.write(os) )
	{
		ioErrorInFile(os.name(), os.lineNumber());
		fatalExit;
	}

	return os; 
}

}

#endif //__symArray_H__
