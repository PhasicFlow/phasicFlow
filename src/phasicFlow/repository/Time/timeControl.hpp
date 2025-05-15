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
public:
	using timeStridedRange = stridedRange<TimeValueType>;
private:


	//// - Data members

		// - time information 
		timeInfo 		ti_;
		
		// - start time of simulation
		TimeValueType startTime_;

		// - end time of simulation 
		TimeValueType endTime_;

		// - stopAt
		TimeValueType stopAt_;	

		// - time interval for time folder output 
		TimeValueType saveInterval_;

		// - the last time folder that was saved
		TimeValueType lastSaved_;

		bool  managedExternaly_ = false;

		bool outputToFile_ = false;

		Logical						performSorting_;

		static 
		inline int32StridedRagne 	screenReportInterval_ ={0,100};

		static
		inline timeStridedRange  	timersReportInterval_{0,100};

		static
		inline timeStridedRange 	sortingInterval_{0,100};
		
		static 
		inline word  timeName_ = "wrongSettings"; // for managedExternamly

		static
		inline TimeValueType 	writeTime_ = 0; // for managedExternamly

		void checkForOutputToFile();
		
		bool screenReport()const;
	
public:

	explicit timeControl(const dictionary& dict);

	timeControl(
		dictionary& dict,
		TimeValueType startTime,
		TimeValueType endTime,
		TimeValueType saveInterval,
		word startTimeName);
	
	virtual ~timeControl() = default;
	
	TimeValueType dt()const
	{
		return ti_.dt();
	}

	TimeValueType setTime(TimeValueType t);
	

	void setStopAt(TimeValueType sT)
	{
		if(managedExternaly_)
		{
			stopAt_ = sT;
		}
	}

	TimeValueType startTime()const
	{
		return startTime_;
	}

	TimeValueType endTime()const
	{
		return endTime_;
	}

	TimeValueType saveInterval()const
	{
		return saveInterval_;
	}

	word timeName()const;	

	TimeValueType currentTime() const 
	{
		return ti_.currentTime();
	}

	word currentTimeWord(bool forSave = true)const
	{
		return ti_.timeName();
		
	}

	uint32 currentIter()const
	{
		return ti_.currentIter();
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
	
	inline
	const timeInfo& TimeInfo()const
	{
		return ti_;
	}

	static
	uint32 timePrecision()
	{
		return timeInfo::precision();
	}

};


} // pFlow

#endif // __timeControl_hpp__
