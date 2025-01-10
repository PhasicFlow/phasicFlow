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
#ifndef __subscriber_hpp__
#define __subscriber_hpp__



#include <array>

#include "List.hpp"
#include "message.hpp"
#include "timeInfo.hpp"

namespace pFlow
{

class observer;
class anyList;

class subscriber
{
private:

	// - list of subsribed objectd that recieve updage messages 
	mutable std::array<List<observer*>,message::numEvents()> observerList_;

	word subName_;

public:

	subscriber(const word& name)
	:
		subName_(name)
	{}

	/// Copy constructor, only copies the name, not the list 
	subscriber(const subscriber&);

	/// @brief Move constructor, moves the name and the list,
	/// also change the subriber of the object in the list 
	subscriber(subscriber&&);

	/// Copy assignemnt, only assign the name, do not assign the list 
	subscriber& operator = (const subscriber&);

	/// @brief Move assignment, move assign name and list,
	/// also change the subcriber of the object in the list 
	subscriber& operator = (subscriber&&);

	virtual ~subscriber();

	virtual bool subscribe(message msg, observer* obsevr)const;

	virtual bool unsubscribe(observer* obsevr)const;

	bool notify(
		uint32 iter,
		real t,
		real dt,
		const message msg, 
		const anyList& varList);
	
	bool notify(
		const timeInfo& ti,
		const message msg, 
		const anyList& varList);
	
	const word& subscriberName()const
	{
		return subName_;
	}

};

} // pFlow



#endif // __eventSubscriber_hpp__
