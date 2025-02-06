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
class timeInfo;

class observer
{
private:
	

	/// pointer to subscriber 
	const subscriber* 	subscriber_ = nullptr;
	
	/// list of events in the message 
	message 			message_;

	friend subscriber;
	
	const subscriber* changeSubscriber(const subscriber* newSub);

	inline void invalidateSubscriber()
	{
		subscriber_ = nullptr;
	}

public:

	observer(message msg);

	observer(
		const subscriber* subscrbr, 
		message msg);

	virtual 
	~observer();

	void subscribe(
		const subscriber* subscrbr, 
		message msg);
	
	observer& addEvent(message::EVENT event);

	inline 
	bool subscribed()const 
	{
		return subscriber_!=nullptr;
	}

	void addToSubscriber(
		const subscriber* subscrbr, 
		message msg);

	bool addToSubscriber(const subscriber& subscriber);
	
	

	static
	constexpr auto numEvents()
	{
		return message::numEvents();
	} 

	/// TODO: remove this API from this class 
	virtual bool hearChanges(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
		const anyList& varList)
		{ 
			notImplementedFunction;
			return false;
		};

	virtual bool hearChanges(
		const timeInfo& ti,
		const message& msg, 
		const anyList& varList)=0;
};

} // pFlow


#endif // __observer_hpp__
