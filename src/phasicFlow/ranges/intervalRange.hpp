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


#ifndef __intervalRange_hpp__
#define __intervalRange_hpp__ 

#include "types.hpp"
#include "typeInfo.hpp"


namespace pFlow
{

template<typename T>
class
intervalRange
{
protected:

	T 		begin_;

	T 		end_;

	static inline const T maxVal = largestPositive<T>();
	static inline const T minVal = largestNegative<T>();
public:

	TypeInfoTemplateNV11("intervalRange",T);

	intervalRange(T begin, T end)
	:
		begin_(begin),
		end_(end)
	{}

	intervalRange(T beginEnd, bool openEnd )
	:
		begin_(openEnd?beginEnd:minVal),
		end_(openEnd?maxVal:beginEnd)
	{}

	// it should be in the form of begin:end or :end or begin:
	intervalRange(const word& rangeString)
	{
		if(!parseRange(rangeString,begin_,end_))
		{
			fatalErrorInFunction<<
			"bad input for the range. It should have the form of  begin:end or :end or begin: \n";
			fatalExit;
		}
	}

	T begin()const
	{
		return begin_;
	}

	T end()const
	{
		return end_;
	}

	inline
	bool isMember(T val)const
	{
		if(val<begin_ || val>end_)return false;
		return true;
	}

	static
	bool parseRange(const word& rangeString, T& begin, T& end)
	{
		if(std::count(
			rangeString.begin(),
			rangeString.end(), 
			':') != 1)
			return false;

		auto col1 = rangeString.find_first_of(":");
		word beginCh;
		if(col1 == 0) 
		{
			begin = minVal;
		}
		else
		{
			auto beginCh = rangeString.substr(0,col1);
			if(!readValue(beginCh,begin))return false;
		}
    
    	if(col1 == rangeString.size())
    	{
    		end = maxVal;
    	}
    	else
    	{
    		auto endCh = rangeString.substr(col1+1);
    		if(!readValue(endCh, end))return false;
    	}
    	
   
		return true;

	}
};

}

#endif  //__stridedRange_hpp__ 
