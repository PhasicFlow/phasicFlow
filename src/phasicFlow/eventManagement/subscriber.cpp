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


#include "subscriber.hpp"
#include "anyList.hpp"
#include "observer.hpp"
#include "message.hpp"


pFlow::subscriber::~subscriber()
{
	for(size_t i=0; i<observerList_.size(); i++)
	{
		for( auto obsvr: observerList_[i] )
		{
			if(obsvr)
			{
				obsvr->invalidateSubscriber();
			}
		}
	}
}

bool pFlow::subscriber::subscribe
(
	message 	msg,
	observer* 	obsevr
)const
{
	if( msg.size()>16 )
	{
		fatalErrorInFunction<<
		"message size is greater than 16!"<<endl;
		return false;
	}
	// do not subscribe nullptr 
	if(!obsevr)return false;

	for(size_t i=0; i<msg.size(); i++)
	{
		if(msg.equivalentTo(i))
		{
			if(!observerList_[i].search(obsevr))
			{
				observerList_[i].push_back(obsevr);
			}
		}
	}
	return true;
}

bool pFlow::subscriber::unsubscribe
(
	observer* obsrvr
)const
{
	if(obsrvr)
	{
		for(size_t i=0; i<observerList_.size(); i++)
		{
			if( auto iter = observerList_[i].find(obsrvr); iter!= observerList_[i].end())
			{
				(*iter)->invalidateSubscriber();
				observerList_[i].erase(iter);
			}
		}
	}
	return true;
}

bool pFlow::subscriber::notify
(
	const message msg, 
	const anyList& varList
)
{
	if( msg.size()>16 )
	{
		fatalErrorInFunction<<
		"message size is greater than 16!"<<endl;
		return false;
	}

	for(size_t i=0; i<msg.size(); i++)
	{
		if(msg.equivalentTo(i))
		{
			for( auto obsvr: observerList_[i] )
			{
				obsvr->update(message(i), varList);
			}
		}
	}

	return true;
}

/*bool pFlow::subscriber::notify
(
	const eventMessage &msg
)
{
	for ( auto& observer:observerList_ )
	{
		if(observer)
			if( !observer->update(msg) ) return false;
	}

	return true;
}

bool pFlow::eventSubscriber::notify
(
	const eventMessage& msg,
	const List<eventObserver*>& exclutionList
)
{
	Set<eventObserver*> sortedExcList(exclutionList.begin(),exclutionList.end());

	for(auto& observer:observerList_)
	{
		if( observer && sortedExcList.count(observer) == 0 )
		{
			if(!observer->update(msg)) return false;
		}
	}

	return true;
}*/