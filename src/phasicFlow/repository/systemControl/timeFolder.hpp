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

#ifndef __timeFolder_hpp__
#define __timeFolder_hpp__


#include "systemControl.hpp"

namespace pFlow
{

Map<timeValue, fileSystem> getTimeFolders(const fileSystem& path);

class timeFolder
{
	using timeList = Map<timeValue, fileSystem>;

protected:
	
	timeList 			folders_;
	
	timeList::iterator currentFolder_;

	bool validateForPointStructure();

	bool validFieldFile(const fileSystem& filePath)const;

	bool validFieldFile(
		const fileSystem& filePath, 
		word& fieldName, 
		word& objectType)const;

public:

	timeFolder(const systemControl& control, bool pointStructureOnly = false );
	
	timeFolder(const fileSystem& path, bool pointStructureOnly = false);
	
	inline
	bool empty()const
	{
		return folders_.empty();
	}

	inline
	timeValue currentTime()const
	{
		if(folders_.empty()) return -1;
		return currentFolder_->first;
	}

	inline
	timeValue nextTime()const
	{
		auto next = currentFolder_;
		next++;
		if(next == folders_.end()) return -1;
		return next->first;
	}

	fileSystem folder()const
	{
		return currentFolder_->second;
	}

	word timeName()const
	{
		auto tName = tailName(folder().wordPath(), '/');
		return tName;
	}

	fileSystem localFolder()const
	{
		return fileSystem(timeName());
	}

	bool operator ++(int)
	{
		if(!finished()) currentFolder_++;
		return !finished();
	}

	bool setTime(timeValue upto)
	{
		timeList::iterator orgFolder = currentFolder_;
		
		rewind();

		while(!finished())
		{
			auto t = currentTime();
			if( equal(upto, t) || t>upto)
			{
				return true;
			}
			(*this)++;
		}
		
		currentFolder_ = orgFolder;
		
		return false;
	}

	explicit operator bool()const
	{
		return !finished();
	}

	bool operator !()const
	{
		return finished();
	}

	void rewind()
	{
		currentFolder_ = folders_.begin();
	}

	bool finished()const
	{
		if(currentFolder_ == folders_.end()) return true;
		return false;
	}

	timeValue startTime()const
	{
		if(folders_.empty()) return -1;
		auto [t,f] = *folders_.begin();
		return t;
	}

	timeValue endTime()const
	{
		if(folders_.empty()) return -1;
		auto [t,f] = *(--folders_.end());
		return t;
	}

	bool containsPointStructure(const fileSystem& dirPath)const;

	/// Get the list of files in the current folder
	/// the first element is file name and the second is the objectType 
	Map<word, word> currentFolderFiles()const;
};

inline
Map<timeValue, fileSystem> getTimeFolders(const fileSystem& path)
{
	Map<timeValue, fileSystem> tFolders;

	auto subDirs = subDirectories(path);

	for(auto& subD: subDirs)
	{
		auto timeName = tailName(subD.wordPath(), '/');
		timeValue TIME;
		if( auto success = readReal(timeName, TIME); success)
		{
			if(!tFolders.insertIf(TIME, subD))
			{
				fatalErrorInFunction<<
				" duplicate time folder! time = " << TIME <<endl;
				fatalExit;
			}
		}
	}

	return tFolders;
}



} // pFlow


#endif // __Control_hpp__
