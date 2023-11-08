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

#ifndef __IOPattern_hpp__
#define __IOPattern_hpp__



#include "types.hpp"

namespace pFlow
{


class IOPattern
{
public:

	/**
	 * Type of input/output
	 * 
	 * MasterProcessorOnly: Read or write is done on master processor
	 *    and the data on master processor is affected.
	 * 
	 * MasterProcessorDistribute: Read is done on master processor, but 
	 * the data should be distributed between processors based on an externally 
	 * specified pattern. Write is done on master processors and the data is 
	 * collected from all processors (collection is done based on the externally 
	 * specified pattern). 
	 * 
	 * AllProcessorsSimilar: Read is done on all processors and processors 
	 *    read the same data. Write is done on master processor, since 
	 *    all processors have the same copy of data. 
	 */
	enum IOType : int
	{
		MasterProcessorOnly			= 0,
		AllProcessorsSimilar 		= 1,
		MasterProcessorDistribute	= 4,
		AllProcessorsDifferent  = 8 // this is used for << and >> operators for 
									// standard input and output streams 
	};

protected:

	IOType 		ioType_;

	int  			globalSize_ = 1;

	int 			globalRank_ = 0;

	bool 			isMaster_ = true;

	bool 			isParallel_ = false;

public:

	IOPattern( IOType iotype);
	
	IOPattern(const IOPattern&)=default;

	IOPattern& operator=(const IOPattern&)=default;

	~IOPattern() = default;

	inline
	bool isMasterProcessorOnly()const
	{
		return ioType_ == MasterProcessorOnly;
	}

	inline
	bool isAllProcessorSimilar()const
	{
		return ioType_ == AllProcessorsSimilar;
	}

	inline
	bool isMasterProcessorDistribute()const
	{
		return ioType_ == MasterProcessorDistribute;
	}

	inline
	bool isAllProcessorsDifferent()const
	{
		return ioType_ == AllProcessorsDifferent;
	}

	inline
	bool isMaster()const
	{
		return isMaster_;
	}

	inline 
	bool isParallel()const
	{
		return isParallel_;
	}

	inline
	bool thisProcReadData()const
	{
		if(isMasterProcessorOnly() && !isMaster())return false;
		if(isMasterProcessorDistribute() && !isMaster()) return false;
		return true;
	}

	inline
	bool thisProcWriteData()const
	{
		if(isAllProcessorsDifferent()) return true;
		return isMaster();
	}

	inline 
	bool thisProcReadHeader()const
	{
		return thisProcReadData();
	}

	inline
	bool thisProcWriteHeader()const
	{
		return isMaster();
	}

	inline 
	auto globalSize()const
	{
		return globalSize_;
	}

	inline 
	auto globalRank()const
	{
		return globalRank_;
	}

	static 
	word exeMode();


};

}

#endif //__IOPattern_hpp__