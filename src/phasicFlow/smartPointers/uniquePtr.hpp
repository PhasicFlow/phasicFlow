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

#ifndef __uniquePtr_hpp__
#define __uniquePtr_hpp__


#include <memory>
#include <typeinfo>

#include "pFlowMacros.hpp"
#include "error.hpp"

// just for preventing the use of std namespace and adding some minor functionalities


namespace pFlow
{



template<
	typename T
>
class uniquePtr
:
	public std::unique_ptr<T>
{
public:

	using uniquePtrType = std::unique_ptr<T>;

	// using base constructors 
	using uniquePtrType::unique_ptr;


	template<typename... Args>
	inline static uniquePtr<T> makeUnique(Args&&... args)
	{
		return uniquePtr<T>(new T(std::forward<Args>(args)...));
	}

	void clear()
	{
		this->reset(nullptr);
	}

	// ref to the object of type T
	T& operator ()()
	{
		if(!*this)
		{
			fatalErrorInFunction <<
			"uniquePtr is empty, and you are trying to get its reference. \n" <<
			"Type name is "<< typeid(T).name()<<"\n";
			fatalExit;
		}
		return *this->get();
	}

	// const ref to the object of type T
	const T& operator() () const
	{
		if(!*this)
		{
			fatalErrorInFunction <<
			"uniquePtr is empty, and you are trying to get its reference. \n" <<
			"Type name is "<< typeid(T).name()<<"\n";
			fatalExit;
		}
		return static_cast<const T&>(*this->get());
	}

	explicit operator bool() const
	{
		return this->get()!= nullptr;
	}

};

template<class T, class... Args>
inline uniquePtr<T> makeUnique(Args&&... args)
{
    return uniquePtr<T>(new T(std::forward<Args>(args)...));
}

}

#endif
