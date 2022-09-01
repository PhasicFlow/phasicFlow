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


#ifndef __eventObserver_H__
#define __eventObserver_H__

#include "eventMessage.H"

namespace pFlow
{

class eventSubscriber;


class eventObserver
{
protected:
	
	const eventSubscriber* subscriber_ = nullptr;
	
	// if this object is linked to subscriber 
	bool subscribed_ = false;

public:

	eventObserver();

	eventObserver(const eventSubscriber& subscriber, bool subscribe = true );

	virtual ~eventObserver();

	inline bool subscribed()const {return subscribed_;}

	bool subscribe(const eventSubscriber& subscriber);
	
	inline void invalidateSubscriber()
	{
		subscribed_ = false;
	}

	virtual bool update(const eventMessage& msg)=0;
};

} // pFlow


#endif // __eventObserver_H__
