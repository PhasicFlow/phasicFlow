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
	using timeStridedRange = stridedRange<timeValue>;
private:


	//// - Data members

		// - time information 
		timeInfo 		ti_;
		
		// - start time of simulation
		timeValue startTime_;

		// - end time of simulation 
		timeValue endTime_;

		// - stopAt
		timeValue stopAt_;	

		// - time interval for time folder output 
		timeValue saveInterval_;

		// - the last time folder that was saved
		timeValue lastSaved_;

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
		inline timeValue 	writeTime_ = 0; // for managedExternamly

		void checkForOutputToFile();
		
		bool screenReport()const;
	
public:

	explicit timeControl(const dictionary& dict);

	timeControl(
		dictionary& dict,
		timeValue startTime,
		timeValue endTime,
		timeValue saveInterval,
		word startTimeName);
	
	virtual ~timeControl() = default;
	
	timeValue dt()const
	{
		return ti_.dt();
	}

	timeValue setTime(timeValue t);
	

	void setStopAt(timeValue sT)
	{
		if(managedExternaly_)
		{
			stopAt_ = sT;
		}
	}

	timeValue startTime()const
	{
		return startTime_;
	}

	word timeName()const;	

	timeValue currentTime() const 
	{
		return ti_.currentTime();
	}

	word currentTimeWord(bool forSave = true)const
	{
		return ti_.timeName();
		
	}

	int32 currentIter()const
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
	int32 timePrecision()
	{
		return timeInfo::precision();
	}

};


} // pFlow

#endif // __timeControl_hpp__
