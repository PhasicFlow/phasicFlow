

#include "timeInterval.hpp"
#include "dictionary.hpp"

FUNCTION_H
pFlow::timeInterval::timeInterval(const dictionary& dict)
{
	if(!read(dict))
	{
		fatalExit;
	}
}


FUNCTION_H 
bool pFlow::timeInterval::read(const dictionary& dict)
{
	startTime_ = dict.getValOrSet<real>("startTime", 0);
	endTime_	= dict.getValOrSet<real>("endTime", largeValue);

	return true;
}

FUNCTION_H
bool pFlow::timeInterval::write(dictionary& dict) const
{
	if( !dict.add("startTime", startTime_) )
	{
		fatalErrorInFunction<<
		"  error in writing startTime to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if( !dict.add("endTime", endTime_) )
	{
		fatalErrorInFunction<<
		"  error in writing endTime to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

FUNCTION_H
bool pFlow::timeInterval::read(iIstream& is)
{
	word key;
	real val;

	is >> key >> val;
	if(key != "startTime")
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"  expected startTime but found "<< key <<endl;
		return false;
	}
	else
		startTime_ = val;

	is.readEndStatement(FUNCTION_NAME);

	is >> key >> val;
	if(key != "endTime")
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"  expected endTime but found "<< key <<endl;
		return false;
	}
	else
		endTime_ = val;

	is.readEndStatement(FUNCTION_NAME);

	return true;
}

FUNCTION_H
bool pFlow::timeInterval::write(iOstream& os)const
{
	os.writeWordEntry("startTime", startTime_);
	os.writeWordEntry("endTime", endTime_);
	return os.check(FUNCTION_NAME);	
}