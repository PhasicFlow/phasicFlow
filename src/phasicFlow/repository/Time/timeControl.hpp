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


#ifndef __timeControl_hpp__
#define __timeControl_hpp__

#include "types.hpp"
#include "ranges.hpp"
#include "streams.hpp"
#include "timeInfo.hpp"

namespace pFlow
{


class dictionary;


class timeControl
{
private:


	//// - Data members

	// - integration time step
	real dt_;
	
	// - start time of simulation
	real startTime_;

	// - end time of simulation 
	real endTime_;

	// - stopAt
	real stopAt_;

	// - current time of simulation
	real currentTime_;	

	// - time interval for time folder output 
	real saveInterval_;

	// - the last time folder that was saved
	real lastSaved_;

	// - current iteration number (for this execution)
	int32 currentIter_;

	// - time precision for output time folders
	int32 timePrecision_;

	bool  managedExternaly_ = false;

	word  timeName_ = "wrongSettings"; // for managedExternamly

	real 	writeTime_ = 0; // for managedExternamly

	realStridedRange  	timersReportInterval_;

	Logical				performSorting_;

	realStridedRange 	sortingInterval_;

	int32StridedRagne 	screenReportInterval_ ={0,100};

	bool outputToFile_ = false;

	void checkForOutputToFile();
	
	bool screenReport()const;
	
public:

	explicit timeControl(const dictionary& dict);

	timeControl(
		dictionary& dict,
		real startTime,
		real endTime,
		real saveInterval,
		word startTimeName);
	
	virtual ~timeControl() = default;
	
	real dt()const
	{
		return dt_;
	}

	real setTime(real t);
	

	void setStopAt(real sT)
	{
		if(managedExternaly_)
		{
			stopAt_ = sT;
		}
	}

	real startTime()const
	{
		return startTime_;
	}

	word timeName()const;	

	real currentTime() const 
	{
		return currentTime_;
	}

	word currentTimeWord(bool forSave = true)const
	{
		return real2FixedStripZeros( currentTime(), timePrecision());
		/*if(forSave)
		{
			if(!managedExternaly_)
				
			else
				return timeName_;
		}
		else
		{
			return real2FixedStripZeros( currentTime(), timePrecision());
		}*/
	}

	int32 currentIter()const
	{
		return currentIter_;
	}

	bool finalTime()const;
	
	bool reachedStopAt()const;

	bool outputToFile()const
	{
		return outputToFile_;
	}

	bool timersReportTime()const;

	bool sortTime()const;
	
	bool setOutputToFile(real writeTime, const word& timeName)
	{
		if(managedExternaly_)
		{
			timeName_ = timeName;
			writeTime_ = writeTime;
		}
		return true;
	}

	bool operator ++(int);

	void setSaveTimeFolder(
		bool saveToFile, 
		const word& timeName = "wrongTimeFolder");
	
	int32 timePrecision()const
	{
		return timePrecision_;
	}

	inline
	timeInfo TimeInfo()const
	{
		return {static_cast<uint32>(currentIter_), currentTime_, dt_};
	}


};


} // pFlow

#endif // __timeControl_hpp__
