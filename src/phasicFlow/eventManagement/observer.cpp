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

#include "observer.hpp"
#include "subscriber.hpp"

const pFlow::subscriber *pFlow::observer::changeSubscriber(const subscriber *newSub)
{
	const subscriber* old = subscriber_;
	subscriber_ = newSub;
    return old;
}

pFlow::observer::observer(message msg) : subscriber_(nullptr),
                                         message_(msg)
{}

pFlow::observer::observer
(
	const subscriber* subscrbr,
	message msg
)
{
	addToSubscriber(subscrbr, msg);
}

pFlow::observer::~observer()
{
	if(subscriber_)
		subscriber_->unsubscribe(this);
	invalidateSubscriber();
}

void pFlow::observer::addToSubscriber
(
	const subscriber* subscrbr, 
	message msg
)
{
	if(subscriber_)
		subscriber_->unsubscribe(this);
	invalidateSubscriber();

	subscriber_ = subscrbr;
	message_ = msg;

	if(subscriber_)
	{
		if(!subscriber_->subscribe(message_, this))
		{
			fatalErrorInFunction<<
			"error in subscribing an observer"<<endl;
			fatalExit;
		}
	}	
}

bool pFlow::observer::addToSubscriber(const subscriber& subscrbr)
{
	if(subscriber_)
		subscriber_->unsubscribe(this);
	invalidateSubscriber();

	subscriber_ = &subscrbr;
	return subscriber_->subscribe(message_, this);
}