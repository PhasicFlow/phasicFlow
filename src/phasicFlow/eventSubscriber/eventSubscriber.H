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


#ifndef __eventSubscriber_H__
#define __eventSubscriber_H__

#include "List.H"
#include "eventObserver.H"
#include "eventMessage.H"

namespace pFlow
{



class eventSubscriber
{
protected:

	// - list of subsribed objectd that recieve updage messages 
	mutable List<eventObserver*> observerList_;

public:

	eventSubscriber()
	{}

	virtual ~eventSubscriber();

	virtual bool subscribe(eventObserver* observer)const;

	virtual bool unsubscribe(eventObserver* observer)const;

	bool notify(const eventMessage& msg);

	bool notify(const eventMessage& msg, const List<eventObserver*>& exclutionList );



};

} // pFlow



#endif // __eventSubscriber_H__
