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

#include "math.hpp"
#include "cylinder.hpp"
#include "zAxis.hpp"
#include "streams.hpp"

FUNCTION_H
bool pFlow::cylinder::calculateParams()
{

	auto p1p2 = p2_ - p1_;

	if( p1p2.length() > smallValue )
	{
		axisVector2_ 		= dot(p1p2,p1p2);
		axisVector_ = p1p2;

	}else
	{
		return false;
	}

	zAxis zA(p1_,p2_);

	realx3 minPinZ(-sqrt(radius2_), -sqrt(radius2_), 0.0);
	realx3 maxPinZ( sqrt(radius2_),  sqrt(radius2_), sqrt(axisVector2_));

	auto minp = zA.transferBackZ(minPinZ);
	auto maxp = zA.transferBackZ(maxPinZ);

	minPoint_ = min(minp, maxp);
	maxPoint_ = max(minp, maxp);
	
	return true;
}

FUNCTION_H
pFlow::cylinder::cylinder(
	const realx3& p1,
	const realx3& p2,
	const real radius)
:
	p1_(p1),
	p2_(p2),
	radius2_(radius*radius)
{
	if(!calculateParams())
	{
		fatalErrorInFunction<<
		"error in the input parameters for cylinder"<<endl;
		fatalExit;
	}
}

FUNCTION_H
pFlow::cylinder::cylinder
(
	const dictionary & dict
)
:
	p1_
	(
		dict.getVal<realx3>("p1")
	),
	p2_
	(
		dict.getVal<realx3>("p2")
	)
{
	auto rad = dict.getVal<real>("radius");
	radius2_= rad*rad;

	if(!calculateParams())
	{
		fatalErrorInFunction<<
		"error in the input parameters for cylinder in dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}

}

FUNCTION_H
pFlow::cylinder::cylinder
(
	iIstream& is
)
{
	if( !read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading cylinder from file. \n";
		fatalExit;
	}
}


FUNCTION_H
bool pFlow::cylinder::read(iIstream & is)
{
	if(!is.nextData<realx3>("p1", p1_)) return false;
	if(!is.nextData<realx3>("p2", p2_)) return false;
	real rad;
	if(!is.nextData<real>("radius", rad)) return false;
	radius2_ =rad*rad;
	return true;
}

FUNCTION_H
bool pFlow::cylinder::write(iOstream& os)const
{
	os.writeWordEntry("p1", p1_);
	os.writeWordEntry("p2", p2_);
	os.writeWordEntry("radius", sqrt(radius2_));
	return os.check(FUNCTION_NAME);
}

FUNCTION_H
bool pFlow::cylinder::read
(
	const dictionary& dict
)
{
	p1_ = dict.getVal<realx3>("p1");
	p2_ = dict.getVal<realx3>("p2");
	auto rad = dict.getVal<real>("radius");
	radius2_ = rad*rad;
	return true;
}

FUNCTION_H
bool pFlow::cylinder::write
(
	dictionary& dict
)const
{
	if(!dict.add("p1", p1_))
	{
		fatalErrorInFunction<<
		"  error in writing p1 to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("p2", p2_))
	{
		fatalErrorInFunction<<
		"  error in writing p2 to dictionary "<<dict.globalName()<<endl;
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
pFlow::iIstream& pFlow::operator >>(iIstream& is, cylinder& b)
{
	if(! b.read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"error in reading cylinder. \n";
		fatalExit;
	}
	return is;
}

FUNCTION_H
pFlow::iOstream& pFlow::operator << (iOstream& os, const cylinder& b)
{
	
	if(! b.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing cylinder. \n";
		fatalExit;
	}
	return os;	
}