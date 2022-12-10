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

#ifndef __MapPtr_hpp__
#define __MapPtr_hpp__


#include <typeinfo>
#include <map>
#include <unordered_map>

#include "types.hpp"
#include "uniquePtr.hpp"
#include "error.hpp"
#include "iOstream.hpp"


namespace pFlow
{

template< template<class, class> class Container, class Key, class T >
class MapPtr

{
public:

	using MapPtrType 		= MapPtr<Container, Key, T>; 
	
	using mapType 			= Container<Key, T*>;

	using keyType 			= typename mapType::key_type;
	
	using mappedType 		= typename mapType::mapped_type;
	
	using valueType 		= typename mapType::value_type;
	
	using reference 		= typename mapType::reference;
	
	using constReference 	= typename mapType::const_reference;
	
	using iterator 			= typename mapType::iterator;
	
	using constIterator 	= typename mapType::const_iterator;
	
	template<typename... Args>
    inline static uniquePtr<T> makeSafe(Args&&... args)
    {
    	return std::make_unique(std::forward<Args>(args)...);
    }

protected:

	// data members
	Container<Key, T*>  map_;

	

	// Methods
	bool copy(const MapPtrType& src);

	// - find a key and return the the pointer associated with the key
	//   nullptr if fails
	T* findPtr(const keyType& k);

	const T* findPtr(const keyType& k)const ;
	
public:

	// - type info
	TypeInfoTemplateNV("MapPtr", Key);

	//// Contructors 
	
		// - empty map
		MapPtr()
		:
			map_()
		{}

		// - copy construct, create new objects out of the objects in the src 
		MapPtr(const MapPtrType& src);
		

		// - copy assignment, create new objects out of he pointers in the src
	    MapPtrType&  operator=(const MapPtrType& rhs);
	    

	    // move construct
	    // it just simply move the pointers, so the new objects takes the 
	    // ownership of the pointers. 
	    MapPtr( MapPtrType&& src );
	    

	    // move assignment
	    // the lhs object takes the ownership of the new objects and rhs loses the ownership
	    MapPtrType& operator=(MapPtrType&& rhs);
	    
	    // removes /deletes all the objectes associated with pointers

	    uniquePtr<MapPtrType> clone()const
	    {
	    	return makeUnique<MapPtrType>(*this);
	    }

	    MapPtrType* clonePtr()const
	    {
	    	return new MapPtrType(*this);
	    }

	    ~MapPtr()
		{
			clear();
		}

	//// - Methods

		// - insert the object with key and replace if it already exists in the container
		//   return false if fails to insert, no action is taken for object deletion 
		bool insertReplace(const keyType& key, T* ptr);

		// - insert the object with key and replace if it already exists in the container
		//   return false if fail to insert, no action is taken for object deletion  
		bool insertReplace(const keyType& key, uniquePtr<T>& ptr);

		// - insert the object (construct in-place)
		//   return false if fail to insert, the object is deleted by default if fails
		template<typename... Args>
		bool insertReplaceSafe(const keyType& key, Args&&... args);

		// - insert the object if key is new and return nullptr
		// - if key exist, replace the new object and return the pointer to the old object 
		T* set(const keyType& key, T* ptr);

		// - insert the object if key is new and return nullptr
		// - if key exist, replace the new object and return the pointer to the old object 
		uniquePtr<T> set(const keyType& key, uniquePtr<T>& ptr );

		// - insert the object (construct in-place), if key is new and return nullptr
		// - if key exist, replace the new object and return the pointer to the old object 
		template<typename... Args>
		uniquePtr<T> setSafe(const keyType& key, Args&&... args);		

		// - reference to the object found by key
		//   fatalError if not found
		T& operator[] (const keyType& key);
		
		// - const reference to the object found by key
		//   fatalError if not found
		const T& operator[]	(const keyType& key)const;

		// - search if a key exists
	    bool search(const keyType k) const;
	

		// - find a key and return the const pointer if successful
		std::pair<const T*, bool> find(const keyType& k)const;
	
		// - find a key and return the pointer if successful
		std::pair<T*, bool> find(const keyType& k);
		

		// - find the key and release the ownership of the object associated with it
		//   delete the key after release
		uniquePtr<T> release(const keyType& k);

	    // - erase the element and delete the object if it is allocated
	    void erase( const keyType& key );
	    
	    // - clear the content of the map with object deletion 
		void clear();
	
		// - size of container
		size_t size() const
		{
			return map_.size();
		}

		// - check if the container empty
		auto empty() const
		{
			return map_.emtpy();
		}

		// - iterators 
		iterator begin()
		{
			return map_.begin();
		}

		constIterator begin() const
		{
			return map_.begin();
		}

		iterator end()
		{
			return map_.end();
		}

		constIterator end() const
		{
			return map_.end();
		}

};

// ordered (sorted) map 
template<typename key, typename T>
using orderedMapPtr = MapPtr<std::map, key, T>;

// unordered map (hash) map pointer
template<typename key, typename T>
using hashMapPtr = MapPtr<std::unordered_map , key, T>;

template<typename T>
using wordOrderedMapPtr = orderedMapPtr<word,T>;

template<typename T>
using wordHashMapPtr = hashMapPtr<word,T>;


template<typename T>
inline iOstream& printKeys(iOstream& os, const wordHashMapPtr<T> & m)
{
	if (m.empty())
		return os<<"wordHashMapPtr is empty"<<endl;

	for(const auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}

template<typename T>
inline iOstream& printKeys(iOstream& os, const wordOrderedMapPtr<T> & m)
{
	if (m.empty())
		return os<<"wordOrderedMapPtr is empty"<<endl;

	for(const auto iter : m )
	{
		os << iter->first<<endl;
	}

	return os;
}


} // pFlow


#include "MapPtrI.hpp"

#endif
