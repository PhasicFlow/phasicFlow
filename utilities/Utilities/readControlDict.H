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

#ifndef __readControlDict_H__
#define __readControlDict_H__

#include "fileSystem.H"


namespace pFlow
{

class
readControlDict
{
protected:
	
	fileSystem rootPath_;

	fileSystem cdPath_;

	real startTime_;
	
	real endTime_;
	
	real saveInterval_;

	word formatType_;

	int32 precision_;

	inline static fileSystem defaultRootPath = CWD();
	inline static fileSystem defaultCDPath = CWD()/"system"+"controlDict";
	
	word convertTimeToName(const real t, const int32 precision)const;

	bool read();

public:

	readControlDict(
		const fileSystem& rootPath = defaultRootPath, 
		const fileSystem& cdPath = defaultCDPath);

	auto startTime()const
	{
		return startTime_;
	}

	auto endTime()const
	{
		return endTime_;
	}

	auto saveInterval()const
	{
		return saveInterval_;
	}

	auto startTimeName()const
	{
		return convertTimeToName(startTime_, precision_);
	}

};

}

#endif //__readControlDict_H__

