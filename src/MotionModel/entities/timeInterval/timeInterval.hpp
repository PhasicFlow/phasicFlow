

#ifndef __timeInterval_hpp__
#define __timeInterval_hpp__

#include "types.hpp"
#include "pFlowMacros.hpp"

namespace pFlow
{

// forward 
class dictionary;


class timeInterval
{
protected:
	real startTime_ = 0;

	real endTime_ = largeValue;

	real time_=0;

	bool isInInterval_ = true;
	
public:

	INLINE_FUNCTION_HD
	timeInterval(){}

	INLINE_FUNCTION_HD
	timeInterval(const timeInterval&) = default;

	INLINE_FUNCTION_HD
	timeInterval& operator=(const timeInterval&) = default;

	FUNCTION_H
	timeInterval(const dictionary& dict);

	INLINE_FUNCTION_HD
	~timeInterval() = default;

	INLINE_FUNCTION_HD
	auto startTime()const
	{
		return startTime_;
	}

	INLINE_FUNCTION_HD
	auto endTime()const
	{
		return endTime_;
	}

	INLINE_FUNCTION_HD
	auto time()const
	{
		return time_;
	}

	INLINE_FUNCTION_HD
	void setTime(real t)
	{
		isInInterval_ =  inTimeRange(t);
		time_ = t;
	}

	INLINE_FUNCTION_HD
	bool inTimeRange(real t)const
	{
		return t>= startTime_ && t<= endTime_;
	}

	INLINE_FUNCTION_HD
	bool inTimeRange()const
	{
		return isInInterval_;
	}

	// - IO operation
	FUNCTION_H 
	bool read(const dictionary& dict);

	FUNCTION_H
	bool write(dictionary& dict) const;

	FUNCTION_H
	bool read(iIstream& is);

	FUNCTION_H
	bool write(iOstream& os)const;
};


inline iOstream& operator <<(iOstream& os, const timeInterval& obj)
{
	if(!obj.write(os))
	{
		fatalExit;
	}
	return os;
}

inline iIstream& operator >>(iIstream& is, timeInterval& obj)
{
	if( !obj.read(is) )
	{
		fatalExit;
	}
	return is;
}

}

#endif  
