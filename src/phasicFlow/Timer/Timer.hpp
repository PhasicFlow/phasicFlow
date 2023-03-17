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


#ifndef __Timerr_hpp__
#define __Timerr_hpp__


#include <chrono>

#include "types.hpp"



namespace pFlow
{

// forward 
class Timers;


class Timer
{
protected:

	using timer = std::chrono::high_resolution_clock;

	// - name for the timer 
	word name_ = "noNameTimer";

	// start time
	timer::time_point   start_;

	// number of times start() and end() are called 
	int32 				numIteration_ = 0;

	// sum of time duratios (in seconds) between all start() and end() calls 
	real 				accTime_  = 0.0;

	// 
	real 				lastTime_ = 0.0;

	Timers* 			parrent_ = nullptr;

public:

	TypeInfo("Timer");

	Timer(){}

	Timer(const word name)
	:
		name_(name)
	{}

	
	Timer(const word name, Timers* parrent);
	

	const word& name()const
	{
		return name_;
	}

	virtual ~Timer();
	
	virtual void removeParrent()
	{
		parrent_ = nullptr;
	}

	virtual int32 level()const;
	

	virtual bool master()const
	{
		return false;
	}

	void start()
	{
		start_ = timer::now();
	}

	void end()
	{
		auto end = timer::now();
		lastTime_ =  std::chrono::duration_cast
			< std::chrono::duration<real> >(end - start_).count();
		
		numIteration_++;
		accTime_ += lastTime_;
	}

	inline
	bool timerActive()const
	{
		return numIteration_!=0;
	}

	inline
	real lastTime()const
	{
		return lastTime_;
	}

	inline
	real totalTime()const
	{
		return accTime_;
	}

	inline
	real averageTime()const
	{

		return accTime_/max(numIteration_, 1);
	}

	virtual
	real accTimersTotal()const
	{
		return totalTime();
	}

	//// - IO operations

	virtual bool write(iOstream& os, bool subTree)const;

	virtual bool read(iIstream& is)
	{
		return true;
	}
};


inline iOstream& operator<<(iOstream& os, const Timer& t)
{
	t.write(os, false);
	return os;
}

inline iIstream& operator>>(iIstream& is, Timer& t)
{
	return is;
}

}


#endif //__Timer_hpp__
