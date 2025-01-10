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


#ifndef __Map_hpp__
#define __Map_hpp__


#include <map>

#include "types.hpp"
#include "iOstream.hpp"


namespace pFlow
{

template<class Key, class T, class Compare = std::less<Key> >
class Map
:
	public std::map<Key, T, Compare>
{
public:

	using MapType 		= Map<Key, T, Compare>;
	
	using mapType 		= std::map<Key, T, Compare>;

	using iterator 		= typename mapType::iterator;
	
	using constIterator = typename mapType::const_iterator;
	
	using reference 	= typename mapType::reference;
	
	using constReference= typename mapType::const_reference;
	
	using initList 		= typename std::initializer_list<T>;
	
	using keyType 		= typename mapType::key_type;
	
	using mappedType 	= typename mapType::mapped_type;
	
	using valueType 	= typename mapType::value_type;

	// - type info 
	TypeInfoTemplateNV11("Map", Key);

	//// - Constructors

		// Empty Map
		Map()
		{}

		// - with initList
		Map(initList lst)
		:
			mapType(lst)
		{}

		// - Copy construct
		Map(const MapType & src)
		:
			mapType(src)
		{}

		// - Move construct 
		Map( MapType&& src)
		:
			mapType(std::move(src))
		{}

		// - Copy assignment
		MapType& operator=(const MapType& rhs)
		{
			mapType::operator=(rhs);
			return *this;
		}

		// - Move assignment
		MapType& operator=(MapType&& rhs)
		{
			mapType::operator=( std::move(rhs));
			return *this;
		}	

		uniquePtr<MapType> clone()const
		{
			return makeUnique<MapType>(*this);
		}

		MapType* clonePtr()const
		{
			return new MapType(*this);
		}

		~Map()
		{
			this->clear();
		}


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
using wordMap = Map<word,T>;

template<typename T>
using uint64Map = Map<uint64,T>;

template<typename T>
using uint32Map = Map<uint32,T>;

template<typename T>
using int32Map = Map<int32,T>;

template<typename T>
using int64Map = Map<int64,T>;



template<typename T>
inline iOstream& printKeys(iOstream& os, const wordMap<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const uint64Map<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const uint32Map<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const int32Map<T> & m);

template<typename T>
inline iOstream& printKeys(iOstream& os, const int64Map<T> & m);


#include "MapI.hpp"

} // pFlow

#endif
