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

#ifndef __combinedRange_hpp__
#define __combinedRange_hpp__

#include <vector>

#include "stridedRange.hpp"
#include "intervalRange.hpp"
#include "Lists.hpp"

#include "Set.hpp"

namespace pFlow
{

template<typename T>
class combinedRange
{
public:

	using StridedRangeType = stridedRange<T>;

	using IntervalRangeType = intervalRange<T>;

protected:
	
	List<StridedRangeType> 	sRanges_;

	List<IntervalRangeType> iRanges_;

	Set<T> 					individuals_;

public:
	
	combinedRange(){}

	combinedRange(const std::vector<word>& strRanges)
	{
		if(!addRanges(strRanges))
		{
			fatalExit;
		}
	}

	bool addRanges(const std::vector<word>& strRanges)
	{
		for(auto& sR: strRanges)
		{
			if( !(addStridedRange(sR) || 
				addIntervalRange(sR)||
				addIndividual(sR)) )
			{
				fatalErrorInFunction<<
				"bad systax for range defintion "<< sR<<endl;
				return false;
			}
		}
		return true;
	}

	bool addStridedRange(const word& strRange)
	{
		T begin, end, stride;
		if(StridedRangeType::parseRange(strRange, begin, end, stride))
		{
			sRanges_.emplace_back(begin, end, stride);
			return true;
		}
		return false;
	}

	bool addStridedRange(T begin, T end, T stride)
	{
		sRanges_.emplace_back(begin, end, stride);
		return true;
	}

	bool addIntervalRange(const word& strRange)
	{
		T begin, end;
		if( IntervalRangeType::parseRange(strRange, begin, end)	)
		{
			iRanges_.emplace_back(begin,end);
			return true;
		}
		
		return false;
	}

	bool addIntervalRange(T begin, T end)
	{
		iRanges_.emplace_back(begin,end);
		return true;
	}

	bool addIndividual(const T& val)
	{
		individuals_.insert(val);
		return true;
	}

	bool addIndividual(const word& strVal)
	{
		T val;
		if(readValue(strVal , val))
		{
			return addIndividual(val);
		}
		return false;
	}

	T minVal()const
	{
		T m = largestPositive<T>();
		for(const auto& sr:sRanges_)
		{
			m = min(m, sr.begin());
		}
		for(const auto& iR:iRanges_)
		{
			m = min(m, iR.begin());
		}
		for(const auto& i:individuals_)
		{
			m = min(m, i);
		}
		return m;
	}

	T maxVal()const
	{
		T m = largestNegative<T>();
		for(const auto& sr:sRanges_)
		{
			m = max(m, sr.begin());
		}
		for(const auto& iR:iRanges_)
		{
			m = max(m, iR.begin());
		}
		for(const auto& i:individuals_)
		{
			m = max(m, i);
		}
		return m;
	}

	bool isMember(T val)const
	{
		for(auto& sR:sRanges_)
		{
			if(sR.isMember(val))return true;
		}

		for(auto& iR:iRanges_)
		{
			if(iR.isMember(val)) return true;
		}

		if( individuals_.count(val) ) return true;

		return false;
	}
};

}


#endif //__combinedRange_hpp__
