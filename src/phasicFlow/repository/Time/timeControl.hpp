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


namespace pFlow
{


class dictionary;


class timeControl
{
protected:


	//// - Data members

	// - integration time step
	real dt_;
	
	// - start time of simulation
	real startTime_;

	// - end time of simulation 
	real endTime_;

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

	word  timeName_ = "wrongSettings";

	realStridedRange  timersReportInterval_;

	int32StridedRagne screenReportInterval_ ={0,100};

	bool outputToFile_ = false;

	void checkForOutputToFile();
	
	bool screenReport()const;
	
public:

	timeControl(const dictionary& dict);

	timeControl(
		dictionary& dict,
		real startTime,
		real endTime,
		real saveInterval,
		word startTimeName);
	
	virtual ~timeControl()
	{}


	real dt()const
	{
		return dt_;
	}

	real setTime(real t)
	{
		real tmp = currentTime_;
		currentTime_ = t;
		lastSaved_ = t;
		checkForOutputToFile();
		return tmp;
	}

	real startTime()const
	{
		return startTime_;
	}

	real currentTime() const 
	{
		return currentTime_;
	}

	word currentTimeWord(bool forSave = true)const
	{
		if(forSave)
		{
			if(!managedExternaly_)
				return real2FixedStripZeros( currentTime(), timePrecision());
			else
				return timeName_;
		}
		else
		{
			return real2FixedStripZeros( currentTime(), timePrecision());
		}
	}

	int32 currentIter()const
	{
		return currentIter_;
	}

	bool finished()const
	{
		if( currentTime_ >= endTime_ ) return true;
		return false;
	}

	bool outputToFile()const
	{
		return outputToFile_;
	}

	bool timersReportTime()const;
	

	bool operator ++(int)
	{

		if( finished() ) return false;
		// increament iteration number 
		currentIter_++;

		currentTime_ += dt_;
		if(screenReport())
		{
			REPORT(0)<<"Time (s): "<<cyanText( currentTimeWord() )<<endREPORT;
		}
		// switch outputToFile_ on/off
		checkForOutputToFile();

		return true;
	}

	void setSaveTimeFolder(
		bool saveToFile, 
		const word& timeName = "wrongTimeFolder");
	
	int32 timePrecision()const
	{
		return timePrecision_;
	}




};


} // pFlow

#endif // __timeControl_hpp__
