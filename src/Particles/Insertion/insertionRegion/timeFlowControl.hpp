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

#ifndef __timeFlowControl_hpp__ 
#define __timeFlowControl_hpp__ 

#include "types.hpp"
#include "streams.hpp"

namespace pFlow
{

class dictionary;

class timeFlowControl
{
protected:

	real startTime_;

	real endTime_;

	real interval_;

	real rate_;

	size_t numInserted_ = 0;

	bool readTimeFlowControl(const dictionary& dict);

	bool writeTimeFlowControl(dictionary& dict) const;

	size_t numberToBeInserted(real currentTime)
	{
		if(currentTime<startTime_)return 0;
		if(currentTime>endTime_) return 0;

		return static_cast<size_t>( (currentTime - startTime_ + interval_)*rate_ - numInserted_ );
	}

	size_t addToNumInserted(size_t newInserted)
	{
		return numInserted_ += newInserted;
	}

public:


	timeFlowControl(const dictionary& dict);


	bool insertionTime( real currentTime, real dt)
	{
		if(currentTime < startTime_) return false;

		if(currentTime > endTime_) return false;
		if( mod(abs(currentTime-startTime_),interval_)/dt < 1 ) return true;

		return false;
	}

	size_t totalInserted()const
	{
		return numInserted_;
	}

	bool read(const dictionary& dict)
	{
		return readTimeFlowControl(dict);
	}

	bool write(dictionary& dict)const
	{
		return writeTimeFlowControl(dict);
	}

};

}

#endif //__timeFlowControl_hpp__ 
