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

#ifndef __timeInfo__hpp_
#define __timeInfo__hpp_

#include "types.hpp"
#include "dictionary.hpp"

namespace pFlow
{

class timeInfo
{
private:
	
	friend class timeControl;

	// - current iteration number (for this execution)
	uint32 		currentIter_;

	// - current time of simulation
	TimeValueType   currentTime_;

	// - integration time step
	TimeValueType   dt_;

	inline static uint32 presicion_ = 5;

public:

	timeInfo(uint32 cIter, TimeValueType cTime, TimeValueType dt)
	  : currentIter_(cIter),
	    currentTime_(cTime),
	    dt_(dt)
	{
	}

	timeInfo(const dictionary& dict)
	:
		currentIter_(0),
		currentTime_(dict.getVal<TimeValueType>("startTime")),
		dt_( dict.getVal<TimeValueType>("dt"))
	{
		presicion_ = dict.getValOrSet<uint32>("timePrecision",5);
	}

	timeInfo(TimeValueType currentTime, const dictionary& dict)
	:
		currentIter_(0),
		currentTime_(currentTime),
		dt_( dict.getVal<TimeValueType>("dt"))
	{
		presicion_ = dict.getValOrSet<int32>("timePrecision",5);
	}

	inline const TimeValueType& currentTime()const
	{
		return currentTime_;
	}

	inline const TimeValueType& t() const
	{
		return currentTime_;
	}
	inline const TimeValueType& dt() const
	{
		return dt_;
	}
	inline const uint32& iter() const
	{
		return currentIter_;
	}

	inline const uint32& currentIter() const
	{
		return currentIter_;
	}

	inline
	void martchDT()
	{
		currentIter_++;
		currentTime_ += dt_;
	}
	
	inline
	word timeName()const
	{
		return real2FixedStripZeros(currentTime_, presicion_);
	}

	inline 
	word prevTimeName()const
	{
		return real2FixedStripZeros( max(currentTime_-dt_, TimeValueType(0)), presicion_);
	}

	static
	uint32 precision()
	{
		return presicion_;
	}
};

} // namespace pFlow

#endif //__timeInfo__hpp_
