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

/**
 *  Time control for particle insertion
 */
class timeFlowControl
{
protected:

	/// start time of insertion
	real startTime_;

	/// end time of insertion 
	real endTime_;

	/// time interval between each insertion
	real interval_;

	/// rate of insertion 
	real rate_;

	/// number of inserted particles
	size_t numInserted_ = 0;	


	/// Read dictionary
	bool readTimeFlowControl(const dictionary& dict);

	/// Write to dictionary 
	bool writeTimeFlowControl(dictionary& dict) const;

	/// Return number of particles to be inserted at time currentTime
	size_t numberToBeInserted(real currentTime);
	
	/// Add to numInserted
	inline
	size_t addToNumInserted(size_t newInserted)
	{
		return numInserted_ += newInserted;
	}

public:

	/// Construct from dictionary 
	timeFlowControl(const dictionary& dict);

	/// Is currentTime the insertion moment?
	bool insertionTime( real currentTime, real dt);
	
	/// Total number inserted so far
	size_t totalInserted()const
	{
		return numInserted_;
	}

	/// Read from dictionary
	bool read(const dictionary& dict)
	{
		return readTimeFlowControl(dict);
	}

	/// Write to dictionary 
	bool write(dictionary& dict)const
	{
		return writeTimeFlowControl(dict);
	}

};

}

#endif //__timeFlowControl_hpp__ 
