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


template< template<class, class> class Container, class Key, class T >
bool pFlow::MapPtr<Container, Key, T>::copy
(
	const MapPtrType& src
)
{
	for(constIterator iter= src.begin(); iter != src.end(); ++iter)
	{

		// first make sure that the new key can be inserted
		auto [nIter, suc] = map_.insert(valueType(iter->first, nullptr));

		// then insert the new item
		if( suc ) 
		{
			if(iter->second != nullptr)
				map_[iter->first] = iter->second->clonePtr();

		}
		else
		{
			return false;
		}

	}

	return true;
}

template< template<class, class> class Container, class Key, class T >
T* pFlow::MapPtr<Container, Key, T>::findPtr
(
	const keyType& k
)
{	
	if( auto iter = map_.find(k); iter!= map_.end() )
		return iter->second;
	else
		return nullptr;
}

template< template<class, class> class Container, class Key, class T >
const T* pFlow::MapPtr<Container, Key, T>::findPtr
(
	const keyType& k
)const 
{	
	if( auto iter = map_.find(k); iter!= map_.cend() )
		return iter->second;
	else
		return nullptr;
}

template< template<class, class> class Container, class Key, class T >
pFlow::MapPtr<Container, Key, T>::MapPtr
(
	const MapPtrType& src
)
{
	
	if( !copy(src) )
	{
		fatalErrorInFunction<<
			"cannot copy new item into MapPtr \n" <<
			"MapPtr type is "<< typeid(T).name() <<endl;
			fatalExit;
	}		
}

template< template<class, class> class Container, class Key, class T >
pFlow::MapPtr<Container, Key, T>&  pFlow::MapPtr<Container, Key, T>::operator=
(
	const MapPtrType& rhs
)
{
	if (this == &rhs)
	{
    	return *this;  // Self-assignment 
	}

	// clears the content of this 
	clear();

	if( !copy(rhs) )
	{
		fatalErrorInFunction<<
			"cannot perform assignment from rhs into MapPtr \n" <<
			"MapPtr type is "<< typeid(T).name() <<endl;
			fatalExit;
	}

	return *this;

}

template< template<class, class> class Container, class Key, class T >
pFlow::MapPtr<Container, Key, T>::MapPtr
(
	MapPtrType&& src
)
:
	map_(std::move(src))
{}	

template< template<class, class> class Container, class Key, class T >
pFlow::MapPtr<Container, Key, T>&  pFlow::MapPtr<Container, Key, T>::operator=
(
	MapPtrType&& rhs
)
{
	// clears the content of this 
	clear();

	map_.operator=(std::move(rhs));
	return *this;
}


template< template<class, class> class Container, class Key, class T >
bool pFlow::MapPtr<Container, Key, T>::insertReplace
(
	const keyType& key,
	T* ptr
)
{
	// delete the current key if it exists
    erase(key);
    
    auto [iter, success] = map_.insert( valueType(key,ptr));
     
	return success;
}

template< template<class, class> class Container, class Key, class T >
bool pFlow::MapPtr<Container, Key, T>::insertReplace
(
	const keyType& key,
	uniquePtr<T>& ptr
)
{
	erase(key);
	if(auto [iter, success] = map_.insert(valueType(key, nullptr)); success)
	{
		map_[key] = ptr.release();
		return true;
	}

	return false;
}
	
template< template<class, class> class Container, class Key, class T >
template<typename... Args>
bool pFlow::MapPtr<Container, Key, T>::insertReplaceSafe
(
	const keyType& key,
	Args&&... args
)
{
	auto ptr = makeUnique<T>(std::forward<Args>(args)...);
	return insertReplace(key, ptr);
}


template< template<class, class> class Container, class Key, class T >
T* pFlow::MapPtr<Container, Key, T>::set
(
	const keyType& key,
	T* ptr
)
{

	auto [optr, exist] = find(key); 
	map_[key] = ptr;
	return optr;
}

template< template<class, class> class Container, class Key, class T >		
pFlow::uniquePtr<T> pFlow::MapPtr<Container, Key, T>::set
(
	const keyType& key,
	uniquePtr<T>& ptr)
{

	auto[optr,exist] = find(key);
	map_[key] = ptr.release();
	return optr;
	
}

template< template<class, class> class Container, class Key, class T >
template<typename... Args>
pFlow::uniquePtr<T> pFlow::MapPtr<Container, Key, T>::setSafe
(
	const keyType& key, Args&&... args
)
{
	auto ptr = makeUnique<T>(std::forward<Args>(args)...);
	return set(key, ptr);
}


template< template<class, class> class Container, class Key, class T >
T& pFlow::MapPtr<Container, Key, T>::operator[]
(
	const keyType& key
)
{
	T* p = findPtr(key);
	if( !p )
	{
		fatalErrorInFunction<<
		"trying to reach the reference of a nullptr or out of range access the element with key "
		<<key<<endl;
		fatalExit;
	}
	return *p;
}

template< template<class, class> class Container, class Key, class T >
const T& pFlow::MapPtr<Container, Key, T>::operator[]
(
	const keyType& key
)const
{
	const T* p = findPtr(key);
	if( !p )
	{
		fatalErrorInFunction<<
		"trying to reach the reference of a nullptr or out of range access the element with key "
		<<key<<endl;
		fatalExit;
	}

	return *p;
}

template< template<class, class> class Container, class Key, class T >
bool pFlow::MapPtr<Container, Key, T>::search
(
	const keyType k
) const
{
	auto [iter, found] = find(k);
	return found;
}


template< template<class, class> class Container, class Key, class T >
std::pair<const T*, bool> pFlow::MapPtr<Container, Key, T>::find
(
	const keyType& k
)const
{	
	if( auto iter = map_.find(k); iter!= map_.end() )
		return {iter->second,true};
	else
		return {nullptr,false};
}


template< template<class, class> class Container, class Key, class T >
std::pair<T*, bool> pFlow::MapPtr<Container, Key, T>::find(const keyType& k)
{	
	if( auto iter = map_.find(k); iter!= map_.end() )
		return {iter->second,true};
	else
		return {nullptr,false};
}


template< template<class, class> class Container, class Key, class T >
pFlow::uniquePtr<T> pFlow::MapPtr<Container, Key, T>::release
(
	const keyType& key
)
{
	auto p = findPtr(key);
	map_.erase(key);
	return p;
}


template< template<class, class> class Container, class Key, class T >
void pFlow::MapPtr<Container, Key, T>::erase
(
	const keyType& key
)
{
	if( auto ptr = findPtr(key); ptr )
	{
		delete ptr;
		ptr = nullptr;	
	}

	map_.erase(key);
}

template< template<class, class> class Container, class Key, class T >
void pFlow::MapPtr<Container, Key, T>::clear()
{
	for( auto iter = map_.begin(); iter != map_.end(); ++iter )
	{
		if(iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	map_.clear();
}

	