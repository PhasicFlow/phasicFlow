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

template<typename T>
inline bool pFlow::ListPtr<T>::copy(const ListPtrType& src)
{
	
	for( auto& iter : src.list_)
	{		

		if( iter != nullptr)
		{
			push_back( iter->clone().release());
		}
		else
		{
			push_back(nullptr);
		}
	}

	return true;
}

template<typename T>
inline
T* pFlow::ListPtr<T>::ptr(size_t i) 
{

	if(i >= size() ) return nullptr;	

	auto iter = list_.begin();
	std::advance(iter, i);

	return *iter;
}

template<typename T>
inline
const T* pFlow::ListPtr<T>::ptr
(
	size_t i
) const
{

	if(i >= size() ) return nullptr;	

	auto iter = list_.cbegin();
	std::advance(iter, i);

	return *iter;
}

template<typename T>
inline
auto pFlow::ListPtr<T>::pos
(
	size_t i
)
{
	if(i >= size() ) 
	{
		fatalErrorInFunction<< 
		"out of range access to container element. \n";
		fatalExit;
	}

	auto iter = list_.begin();
	std::advance(iter, i);
	return iter;
}

template<typename T>
inline
auto pFlow::ListPtr<T>::pos
(
	size_t i
)const 
{
	if(i >= size() ) 
	{
		fatalErrorInFunction<< 
		"out of range access to container element. \n";
		fatalExit;
	}

	auto iter = list_.cbegin();
	std::advance(iter, i);
	return iter;
}

template<typename T>
inline
pFlow::ListPtr<T>::ListPtr
(
	const ListPtrType& src
)
{
	
	if( !copy(src) )
	{
		fatalErrorInFunction<<
			"cannot copy new item into ListPtr \n" <<
			"ListPtr type is "<< typeid(T).name() <<endl;
			fatalExit;
	}		
}


template<typename T>
inline
pFlow::ListPtr<T>& pFlow::ListPtr<T>::operator=
(
	const ListPtrType& rhs
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

template<typename T>
inline
pFlow::uniquePtr<T> pFlow::ListPtr<T>::set
(
	size_t i, T* ptr
)
{
	uniquePtr<T> uptr(ptr);
	return set(i, uptr);
}


template<typename T>
inline
pFlow::uniquePtr<T> pFlow::ListPtr<T>::set
(
	size_t i,
	uniquePtr<T>&& ptr
)
{
	if( i > size() )
	{
		fatalErrorInFunction<<
		"Out of range access of list members. PtrList size is "<<
		size() << "and you are accessing "<< i << "\n";
		fatalExit;
	}

	auto iter = list_.begin();
	std::advance(iter, i);
	T* oldPtr = *iter;
	*iter = ptr.release();
	return uniquePtr<T>(oldPtr);
}


template<typename T>
template<typename... Args>
inline
pFlow::uniquePtr<T> pFlow::ListPtr<T>::setSafe
(
	size_t i,
	Args&&... args
)
{
	auto ptr(uniquePtr<T>::makeUnique(std::forward<Args>(args)...) );
	return set(i,ptr);	
}

template<typename T>
inline
void pFlow::ListPtr<T>::push_back
(
	T* ptr
)
{
	list_.push_back(ptr);
}

template<typename T>
inline
void pFlow::ListPtr<T>::push_back(uniquePtr<T>&& ptr)
{
	list_.push_back( ptr.release() );
}

template<typename T>
template<typename... Args>
inline
void pFlow::ListPtr<T>::push_backSafe(Args&&... args)
{
	uniquePtr<T> ptr = makeUnique<T>(std::forward<Args>(args)...) ;
	push_back(ptr);
}

template<typename T>
inline
T& pFlow::ListPtr<T>::operator[]
(
	size_t i
)
{
	T* p = ptr(i);

	if( !p )
	{
		fatalErrorInFunction<<
		"trying to reach the reference of a nullptr or out of range access. \n";
		fatalExit;
	}

	return *p;
}

template<typename T>
inline
const T& pFlow::ListPtr<T>::operator[]
(
	size_t i
) const
{
	const T* p = ptr(i);

	if(!p)
	{
		fatalErrorInFunction<<
		"trying to reach the reference of a nullptr or out of range access. \n";
		fatalExit;
	}
	return *p;
}

template<typename T>
inline
size_t pFlow::ListPtr<T>::size()const
{
	return list_.size();
}

template<typename T>
inline
auto pFlow::ListPtr<T>::empty() const
{
	return list_.emtpy();
}

template<typename T>
inline
pFlow::uniquePtr<T> pFlow::ListPtr<T>::release
(
	size_t i
)
{
	auto p = ptr(i);
	list_.erase(pos(i));
	return p;
}



template<typename T>
inline
void pFlow::ListPtr<T>::clear()
{
	
	for( auto iter = list_.begin(); iter != list_.end(); ++iter )
	{
		if(*iter != nullptr)
		{	
			delete *iter;
			*iter = nullptr;
		}	
	}

	list_.clear();
}

template<typename T>
inline
void pFlow::ListPtr<T>::clear
(
	size_t i
)
{
	T* p = ptr(i);

	if( p )
	{
		delete p;
		list_.erase(pos(i));
	}
}