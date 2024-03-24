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


#ifndef __hashMap_hpp__
#define __hashMap_hpp__

#include <unordered_map>

#include "types.hpp"
#include "typeInfo.hpp"
#include "iOstream.hpp"


namespace pFlow
{

template<class Key, class T,  class Hash = std::hash<Key> >
class hashMap
:
	public std::unordered_map<Key, T, Hash>
{
public:

	using hashMapType 	= hashMap<Key, T, Hash>;
	
	using hashmapType 	= std::unordered_map<Key, T, Hash>;

	using hasher 		= typename hashmapType::hasher;

	using iterator 		= typename hashmapType::iterator;
	
	using constIterator = typename hashmapType::const_iterator;
	
	using reference 	= typename hashmapType::reference;
	
	using constReference= typename hashmapType::const_reference ;
	
	using initList 		= typename std::initializer_list<T>;
	
	using keyType 		= typename hashmapType::key_type;
	
	using mappedType 	= typename hashmapType::mapped_type;
	
	using valueType 	= typename hashmapType::value_type;

	
	TypeInfoTemplateNV11("hashMap", Key);

	//// - Constructors

		// Empty hashMap
		hashMap()
		{}

		// - with initList
		hashMap(initList lst)
		:
			hashmapType(lst)
		{}

		// - Copy construct
		hashMap(const hashMapType & src) = default;

		// - Move construct 
		hashMap( hashMapType&& src) = default;

		// - Copy assignment
		hashMapType& operator=(const hashMapType& rhs) = default;
		
		// - Move assignment
		hashMapType& operator=(hashMapType&& rhs) = default;
		
		uniquePtr<hashMapType> clone()const
		{
			return makeUnique<hashMapType>(*this);
		}

		~hashMap() = default;
	

	//// - Methods	

		// Insert an item with copy operation
		bool insertIf(const keyType& k, const mappedType & v);
		
		// insert an item with move operation 
		bool insertIf( keyType&& k, mappedType && v);

		// search for a key
		bool search(const keyType k) const;
		

		std::pair<iterator, bool> findIf(const keyType& k);
		

		const std::pair<constIterator, bool> findIf(const keyType& k) const;
	
};


template<typename T>
using wordHashMap = hashMap<word,T>;

template<typename T>
using uint64HashMap = hashMap<uint64,T>;

template<typename T>
using uint32HashMap = hashMap<uint32,T>;

template<typename T>
using int64HashMap = hashMap<int64,T>;

template<typename T>
using int32HashMap = hashMap<int32,T>;

template<typename T>
inline iOstream& printKeys(iOstream& os, const wordHashMap<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const uint64HashMap<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const uint32HashMap<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const int64HashMap<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const int32HashMap<T> & m);


#include "hashMapI.hpp"

} // pFlow

#endif
