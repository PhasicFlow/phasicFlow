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
#ifndef __baseTimeControl_hpp__
#define __baseTimeControl_hpp__

#include "dictionary.hpp"
#include "ranges.hpp"


namespace pFlow
{

class baseTimeControl
{
private:
	 
	bool 	isTimeStep_;

	int32StridedRagne iRange_;

	realStridedRange  rRange_;
	

public:

	baseTimeControl(
		const dictionary& dict, 
		const word& intervalPrefix = "",
		real defStartTime = 0.0);
	
	inline
	bool isTimeStep()const
	{
		return isTimeStep_;
	}

	bool timeEvent(uint32 iter, real t, real dt)const;

	bool isInRange(uint32 iter, real t, real dt)const;

	real startTime()const;

	real endTime()const;

	real rInterval()const;

	int32 startIter()const;

	int32 endIter()const;

	int32 iInterval()const;

};

}

#endif