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


#include "line.H"
#include "dictionary.H"


FUNCTION_HD
pFlow::line::line
(
	const realx3 &lp1,
	const realx3 &lp2
)
{
	set(lp1, lp2);
}

FUNCTION_H
pFlow::line::line
(
	const dictionary& dict
)
{
	if(!read(dict))
	{
		fatalExit;
	}
}

FUNCTION_H
bool pFlow::line::read
(
	const dictionary& dict
)
{
	realx3 p1 = dict.getVal<realx3>("p1");
	realx3 p2 = dict.getVal<realx3>("p2");
	set(p1, p2);
	return true;
}

FUNCTION_H
bool pFlow::line::write(dictionary& dict) const
{
	if(!dict.add("p1", point1()))
	{
		fatalErrorInFunction <<
		" error in writing p1 to dictionary " << dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("p2", point2()))
	{
		fatalErrorInFunction <<
		" error in writing p2 to dictionary " << dict.globalName()<<endl;
		return false;
	}

	return true;
}

FUNCTION_H
bool pFlow::line::read
(
	iIstream& is
)
{
	word p1w;
	realx3 p1;
	is >> p1w >> p1;
	if( !is.check(FUNCTION_NAME))
	{
		ioErrorInFile(is.name(), is.lineNumber());
		return false;
	}
	if(p1w != "p1")
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"  expected p1 but found "<< p1w<<endl;
		return false;
	}
	is.readEndStatement(FUNCTION_NAME);

	word p2w;
	realx3 p2;
	is >> p2w >> p2;

	if( !is.check(FUNCTION_NAME))
	{
		ioErrorInFile(is.name(), is.lineNumber());
		return false;
	}
	if(p2w != "p2")
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"  expected p2 but found "<< p2w<<endl;
		return false;
	}
	is.readEndStatement(FUNCTION_NAME);
	set(p1,p2);
	return true;
}

FUNCTION_H
bool pFlow::line::write
(
	iOstream& os
)const
{
	os.writeWordEntry("p1", point1());
	os.writeWordEntry("p2", point2());
	return os.check(FUNCTION_NAME);
}