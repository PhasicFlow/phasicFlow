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

#ifndef __stridedRange_hpp__
#define __stridedRange_hpp__ 

#include "types.hpp"
#include "typeInfo.hpp"
#include "dictionary.hpp"

#include "streams.hpp"

namespace pFlow
{

template<typename T>
class
stridedRange
{
protected:

	T 		begin_ = 0;

	T 		end_ = 1;

	T 		stride_ = 1;

	static inline const T maxVal = largestPositive<T>();
	static inline const T minVal = largestNegative<T>();

public:

	TypeInfoTemplateNV11("stridedRange",T);

	stridedRange()=default;

	stridedRange(T begin, T end, T stride)
	:
		begin_(begin),
		end_(end),
		stride_(stride)
	{}

	stridedRange(T begin, T stride)
	:
		begin_(begin),
		end_(maxVal),
		stride_(stride)
	{}


	// it should be in the form of begin:stride:end
	stridedRange(const word& rangeString)
	{
		if(!parseRange(rangeString,begin_,end_, stride_))
		{
			fatalErrorInFunction<<
			"bad input for the range. It should have the form of  begin:stride:end \n";
			fatalExit;
		}
	}

	stridedRange(const dictionary& dict)
	:
		begin_(dict.getVal<T>("begin")),
		end_(dict.getVal<T>("end")),
		stride_(dict.getVal<T>("stride"))
	{}
		
	inline 
	T begin()const
	{
		return begin_;
	}

	T end()const
	{
		return end_;
	}

	T stride()const
	{
		return stride_;
	}
	
	inline
	bool isInRange(T val)const
	{
		return val>= begin_ && val<= end_;
	}

	inline
	bool isMember(T val, T epsilon = 0)const
	{
		
		if(!isInRange(val)) return false;
		if(const T dist = val-begin_; abs(dist%stride_)<=epsilon) return true;
		if(equal(val,begin_))return true;
		if(equal(val,end_))return true;
		return false;
	}

	static
	bool parseRange(const word& rangeString, T& begin, T& end, T& stride)
	{
		if(std::count(
			rangeString.begin(),
			rangeString.end(), 
			':') != 2)
			return false;

		auto col1 = rangeString.find_first_of(":");

		if(col1 == 0 || col1==std::string::npos)return false;

		auto beginCh = rangeString.substr(0,col1);
    
	    auto col2 = rangeString.find_first_of(":",col1+1);

	    if(col1 == col2 || col2==std::string::npos)return false;

    	auto strideCh = rangeString.substr(col1+1,col2-col1-1);
    
    	auto endCh = rangeString.substr(col2+1);

    	if(!readValue(beginCh,begin))return false;
    	if(!readValue(strideCh, stride))return false;
    	if(!readValue(endCh, end))return false;
    
	return true;

	}
};

template<>
inline
bool stridedRange<float>::isMember(float val, float epsilon)const
	{

	/*if(!isInRange(val)) return false;
	real dist = val-begin_;
	if(abs(
		(dist-(static_cast<uint64>((dist+0.01*epsilon)/stride_)*stride_))
		)<= epsilon) return true;
	if(equal(val,begin_))return true;
	if(equal(val,end_))return true;
	return false;*/
	if(!isInRange(val)) return false;
	if(const float dist = val-begin_; abs(remainder(dist,stride_)<= epsilon)) return true;	
	if(equal(val,begin_))return true;
	if(equal(val,end_))return true;
	return false;
}

template<>
inline
bool stridedRange<double>::isMember(double val, double epsilon)const
{
	if(!isInRange(val)) return false;
	if(const double dist = val-begin_; abs(remainder(dist,stride_)<= epsilon)) return true;	
	if(equal(val,begin_))return true;
	if(equal(val,end_))return true;
	return false;
}

}

#endif  //__stridedRange_hpp__ 
