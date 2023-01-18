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


#ifndef __systemControl_hpp__
#define __systemControl_hpp__


// top-level entity repository for the whole application 
// Each application that is executed in pFlow, should have 
// settings/systemControl file in it. 


#include "types.hpp"
#include "Time.hpp"
#include "dictionary.hpp"
#include "box.hpp"
#include "Timers.hpp"

namespace pFlow
{

class systemControl
:
	public repository
{
protected:

	// run name 
	const word 	   		runName_;

	// - path to top-level folder 
	const fileSystem 	topLevelFolder_;

	// - settings folder repository
	repository 			settings_;

	// - caseSetup folder repository
	repository 			caseSetup_;

	// - settingsDict fileDictionary
	dictionary& 		settingsDict_;

	// - precision for writing to file 
	size_t 			outFilePrecision_ = 6;

	// - time repository
	Time 			Time_;

	// - if time control is managed externaly

	bool 				externalTimeControl_ = false;

	// - acceleration 
	realx3 			g_;

	// - domain for dem world
	box  			domain_;

	// all timers
	Timers 			timers_;

	Logical			timersReport_;

	Timer 			writeToFileTimer_;

	static word getRunName( const fileSystem& path);
	
	static word getTopFolder(const fileSystem& path);


public:

	systemControl(const fileSystem path = CWD());
		
	systemControl(
		const real startTime, 
		const real endTime,
		const real saveInterval,
		const word startTimeName,
		const fileSystem path = CWD() );

	const repository& settings() const{
		return settings_;
	}

	repository& settings(){
		return settings_;
	}

	const repository& caseSetup()const{
		return caseSetup_;
	}

	repository& caseSetup(){
		return caseSetup_;
	}

	
	const Time& time() const
	{
		return const_cast<Time&>(Time_);
	}

	Time& time()
	{
		return Time_;
	}

	const repository& geometry()const
	{
		return Time_.geometry();
	}

	repository& geometry()
	{
		return Time_.geometry();
	}

	const Timers& timers()const
	{
		return timers_;
	}

	Timers& timers()
	{
		return timers_;
	}

	inline bool timersReport()const
	{
		return timersReport_();
	}

	const dictionary& settingsDict()const{
		return settingsDict_;
	}

	dictionary& settingsDict(){
		return settingsDict_;
	}

	virtual word runName() const
	{
		return runName_;
	}

	inline const realx3& g()const
	{
		return g_;
	}

	inline const box& domain()const
	{
		return domain_;
	}

	bool operator ++(int);

	void setSaveTimeFolder(
		bool saveToFile, 
		const word& timeName = "wrongTimeFolder")
	{
		Time_.setSaveTimeFolder(saveToFile, timeName);
	}

	size_t outFilePrecision() const override
	{
		return outFilePrecision_;
	}

};



} // pFlow


#endif // __systemControl_hpp__
