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


#include "sphere.hpp"


FUNCTION_H
pFlow::sphere::sphere(
	const realx3& center,
	const real radius)
:
	center_(center),
	radius2_(radius*radius)
{
	
}

FUNCTION_H
pFlow::sphere::sphere
(
	const dictionary & dict
)
:
	center_
	(
		dict.getVal<realx3>("center")
	)
{
	auto rad = dict.getVal<real>("radius");
	radius2_= rad*rad;
}

FUNCTION_H
pFlow::sphere::sphere
(
	iIstream& is
)
{
	if( !read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading sphere from file. \n";
		fatalExit;
	}
}


FUNCTION_H
bool pFlow::sphere::read(iIstream & is)
{
	if(!is.nextData<realx3>("center", center_)) return false;
	real rad;
	if(!is.nextData<real>("radius", rad)) return false;
	radius2_ =rad*rad;
	return true;
}

FUNCTION_H
bool pFlow::sphere::write(iOstream& os)const
{
	os.writeWordEntry("center", center_);
	os.writeWordEntry("radius", sqrt(radius2_));
	return os.check(FUNCTION_NAME);
}

FUNCTION_H
bool pFlow::sphere::read
(
	const dictionary& dict
)
{
	center_ = dict.getVal<realx3>("center");
	auto rad = dict.getVal<real>("radius");
	radius2_ = rad*rad;
	return true;
}

FUNCTION_H
bool pFlow::sphere::write
(
	dictionary& dict
)const
{
	if(!dict.add("center", center_))
	{
		fatalErrorInFunction<<
		"  error in writing center to dictionary "<<dict.globalName()<<endl;
		return false;
	}


	if(!dict.add("radius", sqrt(radius2_)) )
	{
		fatalErrorInFunction<<
		"  error in writing radius to dictionary "<<dict.globalName()<<endl;
		return false;
	}
	
	return true;
}

FUNCTION_H
pFlow::iIstream& pFlow::operator >>(iIstream& is, sphere& b)
{
	if(! b.read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading sphere. \n";
		fatalExit;
	}
	return is;
}

FUNCTION_H
pFlow::iOstream& pFlow::operator << (iOstream& os, const sphere& b)
{
	
	if(! b.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing sphere. \n";
		fatalExit;
	}
	return os;	
}