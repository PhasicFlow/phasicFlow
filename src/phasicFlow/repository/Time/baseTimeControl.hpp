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
    granular and multiphase flows. You can redistribute it and/or modify it
under the terms of GNU General Public License v3 or any other later versions.

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

class timeInfo;

class baseTimeControl
{
private:

    bool                     isTimeStep_;

    int32StridedRagne        iRange_;

    stridedRange<TimeValueType>  rRange_;

    word                      intervalPrefix_;

protected:

	void setTimeControl(
		TimeValueType startTime, 
		TimeValueType endTime, 
		TimeValueType interval, 
		const word& intervalPrefix);

public:

	baseTimeControl(
	  const dictionary& dict,
	  const word&       intervalPrefix = "",
	  TimeValueType         defStartTime   = 0.0
	);

	baseTimeControl(
		const dictionary&  dict,
		const TimeValueType	   defInterval,
		const word&        intervalPrefix="",
		const TimeValueType    defStartTime=0.0);

	baseTimeControl(
		int32 start,
		int32 end,
		int32 stride,
		const word& intervalPrefix = ""
	); 

	baseTimeControl(
		TimeValueType start,
		TimeValueType end,
		TimeValueType stride,
		const word& intervalPrefix = ""
	);

    baseTimeControl(
        const baseTimeControl& other
    ) = default;

    baseTimeControl(
        baseTimeControl&& other
    ) = default;

    baseTimeControl& operator=(
        const baseTimeControl& other
    ) = default;
    
    baseTimeControl& operator=(
        baseTimeControl&& other
    ) = default;
    
    ~baseTimeControl() = default;

	inline bool isTimeStep() const
	{
		return isTimeStep_;
	}

	bool eventTime(uint32 iter, TimeValueType t, TimeValueType dt) const;

	bool eventTime(const timeInfo& ti)const;

	bool  isInRange(uint32 iter, TimeValueType t, TimeValueType dt) const;

	TimeValueType  startTime() const;

	TimeValueType  endTime() const;

	TimeValueType  rInterval() const;

	int32 startIter() const;

	int32 endIter() const;

	int32 iInterval() const;

	bool  write(dictionary& dict) const;
};

}

#endif