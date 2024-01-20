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

pFlow::observer::observer():
	subscriber_(nullptr)
{}

pFlow::observer::observer
(
	const subscriber* subscrbr,
	message msg
)
:
	subscriber_(subscrbr),
	message_(msg)
{
	if(subscriber_)
	{
		if(!subscriber_->subscribe(msg, this))
		{
			fatalErrorInFunction<<
			"error in subscribing an observer"<<endl;
			fatalExit;
		}
	}
}

pFlow::observer::~observer()
{
	if( subscriber_)
		subscriber_->unsubscribe(this);
	invalidateSubscriber();
}

bool pFlow::observer::addToSubscriber(const subscriber& subscrbr)
{
	subscriber_ = &subscrbr;
	return subscriber_->subscribe(message_, this);
}