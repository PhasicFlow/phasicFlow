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

#ifndef __timeFolder_H__
#define __timeFolder_H__


#include "systemControl.H"

namespace pFlow
{


class timeFolder
{
	using timeList = Map<real, fileSystem>;
protected:
	const systemControl& control_;

	timeList 			folders_;
	
	timeList::iterator currentFolder_;

	
public:

	timeFolder(const systemControl& control ):
		control_(control),
		folders_(control.getTimeFolders()),
		currentFolder_(folders_.begin())
	{}

	real time()const
	{
		return currentFolder_->first;
	}

	fileSystem folder()const
	{
		return currentFolder_->second;
	}

	bool operator ++(int)
	{
		if(!finished()) currentFolder_++;
		return !finished();
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

	real startTime()const
	{
		auto [t,f] = *folders_.begin();
		return t;
	}

	real endTime()const
	{
		auto [t,f] = *(--folders_.end());
		return t;
	}
};


} // pFlow


#endif // __Control_H__
