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

#ifndef __ListPtr_hpp__
#define __ListPtr_hpp__


#include <list>


#include "types.hpp"
#include "uniquePtr.hpp"
#include "error.hpp"
#include "iOstream.hpp"


namespace pFlow
{

template<typename  T >
class ListPtr
{
public:

	using ListPtrType 		= ListPtr<T> ; 
	using listType 			= std::list<T*>;


	template<typename... Args>
    inline static uniquePtr<T> makeSafe(Args&&... args)
    {
    	return uniquePtr<T>(new T(std::forward<Args>(args)...));
    }

protected:

	//// - data members

		// - list of pointers 
		std::list<T*>  	list_;

	

	//// - Methods

		// - copy the content to this list
		bool copy(const ListPtrType& src);

		// - return ith pointer
		T* ptr(size_t i);

		// - return ith const poiter
		const T* ptr(size_t i)const;

		// - iterator position of ith element
		auto pos(size_t i);
		
		// - const iterator position of ith element
		auto pos(size_t i) const;

public:

	// - Type info
	TypeInfoTemplateNV("ListPtr", T);
	
	//// - Contructors 

		// - empty list
		ListPtr()
		:
			list_()
		{}

		// - a list with initial length of len
		ListPtr(size_t len)
		:
			list_(len)
		{}

		// - copy construct, create new objects out of the pointers in the src 
		ListPtr(const ListPtrType& src);
		

		//- copy assignment, create new objects out of he pointers in the src
	    ListPtrType&  operator=(const ListPtrType& rhs);
    

	    // - move construct
	    //   Simply move the pointers, so the new object takes the 
	    //   ownership of the pointers. 
	    ListPtr( ListPtrType&& src )
	    :
	    	list_(std::move(src))
	    {}	

	    // - move assignment
	    // 	 the lhs object takes the ownership of pointers and rhs loses the ownership
	    ListPtrType& operator=(ListPtrType&& rhs)
	    {
	    	// clears the content of this 
	    	clear();

	    	list_.operator=(std::move(rhs));
	    	return *this;
	    }

	    ListPtrType* clonePtr()const
		{
			auto ptr = makeUnique<ListPtrType>(*this);
			return ptr.release();
		}

		uniquePtr<ListPtrType> clone()const
		{
			return makeUnique<ListPtrType>(*this);
		}

	    // - remove /delete all the objectes associated with pointers
	    ~ListPtr()
		{
			clear();
		}

	//// - Methods 

		// - set the ith element 
		T* set(size_t i, T* ptr);
		
		// - set the ith element and take the ownership from uniquePtr
		uniquePtr<T> set(size_t i, uniquePtr<T>& ptr );
		
		// - create the object in-place and set the pointer in ith position
		//   if oject creation fails, uniquePtr deletes the memeory
		template<typename... Args>
	    uniquePtr<T> setSafe(size_t i, Args&&... args);
	    
	    // - put the pointer at the end
	    void push_back(T* ptr);
	    
	    // - put the pointer at the end
	    void push_back(uniquePtr<T>& ptr);
	    
	    // - safely create (in-place) and put the pointer at the end
	    template<typename... Args>
	    void push_backSafe(Args&&... args);
	    
		// - access to ith element
		//   fatalexit if out of range or nullptr    
		T& operator[](size_t i);
		
		// - const access to ith element
		//   fatalexit if out of range or nullptr    
		const T& operator[](size_t i) const;

		// size of container
	    size_t size()const;
	    
	    // check if the container empty
		auto empty() const;

		// release the ownership of ith pointer 
		uniquePtr<T> release(size_t i);
		
		// - clear the content of list and delete objects
		void clear();
		
		// - clear the ith element
		void clear(size_t i);	
		
		// - clone the object 
		
};


} // pFlow


#include "ListPtrI.hpp"

#endif
