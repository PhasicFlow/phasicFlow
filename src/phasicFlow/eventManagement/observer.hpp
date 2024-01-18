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


#ifndef __observer_hpp__
#define __observer_hpp__

#include "message.hpp"

namespace pFlow
{

class subscriber;
class anyList;

class observer
{
protected:
	

	/// pointer to subscriber 
	const subscriber* 	subscriber_ = nullptr;
	
	/// list of events in the message 
	const message 		message_;


public:

	observer();

	observer(
		const subscriber* subscrbr, 
		message msg);

	virtual 
	~observer();

	inline 
	bool subscribed()const 
	{
		return subscriber_!=nullptr;
	}

	bool addToSubscriber(const subscriber& subscriber);
	
	inline void invalidateSubscriber()
	{
		subscriber_ = nullptr;
	}

	static
	constexpr auto numEvents()
	{
		return message::numEvents();
	} 

	virtual bool hearChanges(const message& msg, const anyList& varList)=0;
};

} // pFlow


#endif // __observer_hpp__
