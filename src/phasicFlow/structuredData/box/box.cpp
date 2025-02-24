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


#include "box.hpp"


FUNCTION_H
pFlow::box::box
(
	const dictionary & dict
)
:
	min_
	(
		dict.getVal<realx3>("min")
	),
	max_
	(
		dict.getVal<realx3>("max")
	)
{
	if( !(min_ < max_))
	{
		fatalErrorInFunction<<
		"the corenter points of box are not set correctly"<<
		" (min point is greater than max point). In dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}

FUNCTION_H
pFlow::box::box
(
	iIstream& is
)
{
	if( !read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading box from file. \n";
		fatalExit;
	}
}


FUNCTION_H
bool pFlow::box::read(iIstream & is)
{
	if(!is.nextData<realx3>("min", min_)) return false;
	if(!is.nextData<realx3>("max", max_)) return false;
	if(!(min_ < max_))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"the corenter points of box are not set correctly"<<
		" (min point is greater than max point). In dictionary "<<endl;
		return false;
	}
	return true;
}

FUNCTION_H
bool pFlow::box::write(iOstream& os)const
{
	os.writeWordEntry("min", min_);
	os.writeWordEntry("max", max_);
	return os.check(FUNCTION_NAME);
}

FUNCTION_H
bool pFlow::box::read
(
	const dictionary& dict
)
{
	min_ = dict.getVal<realx3>("min");
	max_ = dict.getVal<realx3>("max");
	return true;
}

FUNCTION_H
bool pFlow::box::write
(
	dictionary& dict
)const
{
	if(!dict.add("min", min_))
	{
		fatalErrorInFunction<<
		"  error in writing min to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("max", max_))
	{
		fatalErrorInFunction<<
		"  error in writing max to dictionary "<<dict.globalName()<<endl;
		return false;
	}
	
	return true;
}

FUNCTION_H
pFlow::iIstream& pFlow::operator >>(iIstream& is, box& b)
{
	if(! b.read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading box. \n";
		fatalExit;
	}
	return is;
}

FUNCTION_H
pFlow::iOstream& pFlow::operator <<(iOstream& os, const box& b)
{
	
	if(! b.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing box. \n";
		fatalExit;
	}
	return os;	
}