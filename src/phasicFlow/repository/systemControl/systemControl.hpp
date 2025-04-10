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
#include "fileDictionary.hpp"
#include "Timers.hpp"
#include "dynamicLinkLibs.hpp"
#include "Set.hpp"
#include "auxFunctions.hpp"


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

    /// settingsDict fileDictionary
	uniquePtr<fileDictionary> 	settingsDict_;

    /// time repository
    Time 			            Time_;

	/// settings folder repository
	uniquePtr<repository> 		settings_;

	/// caseSetup folder repository
	uniquePtr<repository>       caseSetup_;

	/// extra libs to be loaded
	dynamicLinkLibs 	        libs_;
	
	/// precision for writing to file 
	size_t 			outFilePrecision_ = 6;


	/// if time control is managed externaly
	bool 			externalTimeControl_ = false;

	// all timers
	Timers 			timers_;

	Logical			timersReport_;

	Timer 			writeToFileTimer_;

	wordSet 		includeList_;

	wordSet 		excludeList_;
    
	uniquePtr<auxFunctions> 		auxFunctions_ = nullptr;
	
	bool readIncludeExclue(const dictionary& dict);

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
		return settings_();
	}

	repository& settings(){
		return settings_();
	}

	const repository& caseSetup()const{
		return caseSetup_();
	}

	repository& caseSetup(){
		return caseSetup_();
	}

	
	const Time& time() const
	{
		return Time_;
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

	const fileDictionary& settingsDict()const{
		return settingsDict_();
	}

	fileDictionary& settingsDict(){
		return settingsDict_();
	}

	//fileDictionary& domainDict();
	

	virtual word runName() const
	{
		return runName_;
	}

	inline const realx3 g()const
	{
		return settingsDict_().getVal<realx3>("g");
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

	
	bool isIncluded(const word& objName)const final
	{
		return includeList_.count(objName) == static_cast<size_t>(1);
	}

	
	bool isExcluded(const word& objName)const final
	{
		return excludeList_.count(objName) == static_cast<size_t>(1);
	}

	void clearIncludeExclude()
	{
		includeList_.clear();
		excludeList_.clear();
	}

	bool addInclude(const word& objName)
	{
		auto [iter, success] = includeList_.insert(objName);
		return success;
	}

	bool addExclude(const word& objName)
	{
		auto [ite, success] = excludeList_.insert(objName);
		return success;
	}

};



} // pFlow


#endif // __systemControl_hpp__
