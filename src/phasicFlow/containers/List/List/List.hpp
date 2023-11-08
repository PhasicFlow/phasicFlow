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


#ifndef __List_hpp__
#define __List_hpp__

#include <list>

#include "types.hpp"
#include "typeInfo.hpp"
#include "uniquePtr.hpp"
#include "iOstream.hpp"
#include "iIstream.hpp"

namespace pFlow
{


template<
    typename T
> class List 
:
	public std::list<T, std::allocator<T> >
{

public:

	using ListType 		= List<T>;

	using listType 		= std::list<T,std::allocator<T>>;

	using iterator 		= typename listType::iterator;
	
	using const_iterator = typename listType::const_iterator;
	
	using reference 	= typename listType::reference;
	
	using const_reference= typename listType::const_reference;
	
	using initList 		= typename std::initializer_list<T>;
	
	using valueType 	= T; 


protected:

	
	static inline size_t getListStride(const size_t& len)
	{
		size_t stride = 1;
		if( len < 6 )
			stride = len;
		else if( len <16 )
			stride = 3;
		else if( len < 31)
			stride = 2;
		else
			stride = 1;

		return stride;
	}

public:

	// - Type info
	TypeInfoTemplateNV("List", T);

	//// - Constructors

		// - empty list 
		List()
		{}

		// - list with length len
		List(size_t len)
		:
			listType(len)
		{}

		// - list with length len and value
		List(size_t len, const T& value)
		:
			listType(len, value)
		{}

		// - construct from initList
		List(initList lst)
		:
			listType(lst)
		{}


		// - copy construct
		List(const List& src) = default;
			

		// - move construct
		List( List && mv) = default;
		
		// - copy assignment
		ListType& operator=(const ListType& rhs) = default;
		

		// - move assignment
		ListType& operator=(ListType&& rhs) = default;
		

		uniquePtr<ListType> clone()const{
			return makeUnique<ListType>(*this);
		}

		ListType* clonePtr()const{
			return new ListType(*this);
		}


		// - destructor 
		~List()
		{
			listType::clear();
		}

	//// -  Methods

		// - counts elements based on a given value
		int32 countElement(const T& elm) const;
		
		// - size of container
		size_t size()const;
		
		// position of ith element
		auto pos(size_t i, bool noError = false);

		// position of ith element 
		const auto pos(size_t i, bool noError = false)const;

		// - access to ith element
		//   fatal exit if out of range
		T& operator[](size_t i);

		// - const access to ith element
		//   fatal exit if our of range
		const T& operator[](size_t i)const;
		
		// - find the position of the first element with value val
		//   cend() if not found
		const_iterator find(const T& val) const;
		
		// - find the position of the first element with value val
		//    end() if not found
		iterator find(const T& val);

		// - find the index of the first element with value val
		//   -1 if not found
		int32 findi(const T& val) const;
		

		// - search for element, if finds it, 
		//   return true, otherwise false
		bool search(const T& val) const;

		// - set ith element with copy 
		//   fatal error if out of range
		void set(size_t i, const T& val);
		
		// - set ith element with move operation 
		//   fatal error if out of range 
		void set(size_t i, T&& val);

	//// - IO operations

		// - write to oStream
		bool writeList(iOstream& os) const;
		
		// - read from iStream
		bool readList(iIstream& is);

		bool read(iIstream& is)
		{
			return readList(is);
		}

		bool write(iOstream& os)const
		{
			return writeList(os);
		}
	
};


template<typename T>
iOstream& operator << (iOstream& os, const List<T>& lst );


template<typename T>
iIstream& operator >>(iIstream& is, List<T>& lst);



using int64List 	= List<int64>;
using int32List 	= List<int32>;
using int8List 		= List<int8>;
using uint64List 	= List<uint64>;
using uint32List 	= List<uint32>;

using realList 		= List<real>;
using realx3List 	= List<realx3>;
using realx3x3List  = List<realx3x3>;


using boolList 		= List<bool>;
using wordList 		= List<word>;



} // pFlow


#include "ListI.hpp"

#endif //__List_hpp__
