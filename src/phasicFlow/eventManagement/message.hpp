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
#ifndef __message_hpp__
#define __message_hpp__

#include <bitset>

#include "types.hpp"
#include "iOstream.hpp"


namespace pFlow
{

class message
{
public:
	enum EVENT : size_t
	{
		DEFAULT 		= 0,
		CAP_CHANGED 	= 1,
		SIZE_CHANGED 	= 2,
		ITEM_DELETE	 	= 3,
		ITEM_INSERT  	= 4,
		RANGE_CHANGED   = 5,
		ITEM_REARRANGE 	= 6,
		ITEM_TRANSFER   = 7,
		RESET_COUNTERS  = 8	
	};

protected:

	static constexpr size_t numberOfEvents_ = 9;

	std::bitset<numberOfEvents_> events_{0x0000};
	

public:

	message()=default;

	message(EVENT evnt)
	{
		add(evnt);
	}

	message(size_t i )
	{
		if(i<numberOfEvents_)
		{
			events_.set(i);
		}
	}

	message(const message&) = default;

	message(message&&) = default;

	message& operator=(const message&) = default;

	message& operator=(message&&) = default;

	~message()=default;

	inline message& add( EVENT evnt)
	{
		events_.set(static_cast<size_t>(evnt));
		return *this;
	}

	inline message& remove(EVENT evnt)
	{
		events_.set(static_cast<size_t>(evnt), false);
		return *this;
	}

	inline bool equivalentTo( EVENT evnt )const
	{
		return equivalentTo(static_cast<size_t>(evnt));
	}

	inline bool equivalentTo(size_t i)const
	{
		return events_.test(i);
	}

	inline bool isNull()const
	{
		return events_.none();
	}

	inline auto size()const
	{
		return events_.size();
	}

	inline
	message& operator+(EVENT evnt)
	{
		return add(evnt);
	}

	inline
	message& operator-(EVENT evnt)
	{
		return remove(evnt);
	}
	
	static
	auto constexpr numEvents()
	{
		return numberOfEvents_;
	}
	static 
	message Default()
	{
		message msg;
		return msg+DEFAULT;
	}

	static 
	message Empty()	
	{
		message msg;
		return msg;
	}
	
};

inline
iOstream& operator<<(iOstream& os, const message& msg)
{
	for(size_t i=msg.size(); i>0; i--)
	{
		os<< msg.equivalentTo(i-1)? '1':0;
	}
	os<<endl;

	return os;
}

inline
message operator + (message::EVENT evnt1, message::EVENT evnt2)
{
	message msg = message::Empty().add(evnt1).add(evnt2);
	return msg;
}

}

#endif // __eventMessage_hpp__
