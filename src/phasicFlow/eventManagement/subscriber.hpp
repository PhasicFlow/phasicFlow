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

namespace pFlow
{

class observer;
class anyList;

class subscriber
{
protected:

	// - list of subsribed objectd that recieve updage messages 
	mutable std::array<List<observer*>,message::numEvents()> observerList_;

	word subName_;

public:

	subscriber(const word& name)
	:
		subName_(name)
	{}

	subscriber(const subscriber&) = delete;

	subscriber(subscriber&&) = default;

	subscriber& operator = (const subscriber&) = delete;

	subscriber& operator = (subscriber&&) = default;

	virtual ~subscriber();

	virtual bool subscribe(message msg, observer* obsevr)const;

	virtual bool unsubscribe(observer* obsevr)const;

	//bool notify(const eventMessage& msg);

	//bool notify(const eventMessage& msg, const List<eventObserver*>& exclutionList );


	
	bool notify(
		real t,
		real dt,
		uint32 iter,
		const message msg, 
		const anyList& varList);
	

};

} // pFlow



#endif // __eventSubscriber_hpp__
