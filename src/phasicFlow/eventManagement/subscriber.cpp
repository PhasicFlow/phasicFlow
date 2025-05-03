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

pFlow::subscriber::subscriber(const word& name)
:
	observerList_(message::numEvents())
{}

pFlow::subscriber::subscriber(const subscriber & src)
{
}

pFlow::subscriber::subscriber(subscriber && src)
:
	observerList_(std::move(src.observerList_))
{
	
	for(size_t i=0; i<observerList_.size(); i++)
	{
		for( auto obsvr: observerList_[i] )
		{
			if(obsvr) obsvr->changeSubscriber(this);
		}
	}
}

pFlow::subscriber &pFlow::subscriber::operator=(const subscriber & rhs)
{
    //this->subName_ = rhs.subName_;
	return *this;
}

pFlow::subscriber &pFlow::subscriber::operator=(subscriber && rhs)
{
    //this->subName_ = std::move(rhs.subName_);
	this->observerList_ = std::move(rhs.observerList_);

	for(size_t i=0; i<observerList_.size(); i++)
	{
		for( auto obsvr: observerList_[i] )
		{
			if(obsvr) obsvr->changeSubscriber(this);
		}
	}

	return *this;
}	

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

/*bool pFlow::subscriber::notify
(
	uint32 iter,
	real t,
	real dt,
	const message msg, 
	const anyList& varList
)
{
	for(size_t i=0; i<msg.size(); i++)
	{
		if(msg.equivalentTo(i))
		{
			for( auto obsvr: observerList_[i] )
			{
				obsvr->hearChanges
				(
					t,
					dt,
					iter,
					message(i),
					varList
				);
			}
		}
	} 
	return true;
}*/

bool pFlow::subscriber::notify(const timeInfo &ti, const message msg, const anyList &varList)
{
	for(size_t i=0; i<msg.size(); i++)
	{
		if(msg.equivalentTo(i))
		{
			for( auto obsvr: observerList_[i] )
			{
				obsvr->hearChanges
				(
					ti,
					message(i),
					varList
				);
			}
		}
	}
	return true;
}
