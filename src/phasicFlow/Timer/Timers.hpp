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

#ifndef __Timerss_hpp__ 
#define __Timerss_hpp__


#include "List.hpp"
#include "Timer.hpp"


namespace pFlow
{

class Timers
:
	public pFlow::Timer
{
protected:

	List<pFlow::Timer*> 	timers_;

	int32 			level_ = 0;

public:

	TypeInfo("Timers");


	Timers(const word& name)
	:
		Timer(name)
	{}

	Timers(const word& name, Timers* parrent)
	:
		Timer(name, parrent)
	{
		if(parrent)
		{
			level_ = parrent->level()+1;
		}
	}

	virtual ~Timers()
	{
		for(auto& timer:timers_)
		{
			timer->removeParrent();
		}
	}

	// add a timer to this node 
	uniquePtr<Timer> addTimer(const word& name)
	{
		return makeUnique<Timer>(name, this);
	}
	

	void addToList(Timer* timer)
	{
		timers_.push_back(timer);
	}

	void removeFromList(Timer* timer)
	{
		timers_.erase(timers_.find(timer));
	}

	virtual int32 level()const
	{
		return level_;
	}

	virtual bool master()const
	{
		return true;
	}

	real accTimersTotal()const override;

	virtual bool write(iOstream& os, bool subTree = true)const;
	

	virtual bool read(iIstream& is)
	{
		return true;
	}

};

inline iOstream& operator<<(iOstream& os, const Timers& t)
{
	t.write(os, true);
	return os;
}

inline iIstream& operator>>(iIstream& is, Timers& t)
{
	return is;
}

}


#endif //__Timers_hpp__
