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

#include "rotatingAxis.hpp"
#include "dictionary.hpp"


FUNCTION_H
pFlow::rotatingAxis::rotatingAxis
(
	const dictionary& dict
)
{
	if(!read(dict))
	{
		fatalErrorInFunction<<
		"  error in reading rotatingAxis from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}

FUNCTION_HD
pFlow::rotatingAxis::rotatingAxis
(
	const realx3& p1,
	const realx3& p2,
	real omega
)
	:
	timeInterval(),
	line(p1, p2),
	omega_(omega),
	rotating_(!equal(omega,0.0))
{

}


FUNCTION_HD
pFlow::real pFlow::rotatingAxis::setOmega(real omega)
{
	auto tmp = omega_;
	omega_ = omega;
	rotating_ = !equal(omega, 0.0);
	return tmp;
}

FUNCTION_H
bool pFlow::rotatingAxis::read
(
	const dictionary& dict
)
{

	if(!timeInterval::read(dict))return false;
	if(!line::read(dict)) return false;
		
	real omega = dict.getValOrSet("omega", 0.0);
	
	setOmega(omega);
	return true;
}

FUNCTION_H
bool pFlow::rotatingAxis::write
(
	dictionary& dict
) const
{
	if( !timeInterval::write(dict) ) return false;
	if( !line::write(dict) ) return false;
	
	if( !dict.add("omega", omega_) )
	{
		fatalErrorInFunction<<
		"  error in writing omega to dictionary "<< dict.globalName()<<endl;
		return false;
	}
	return true;
}

FUNCTION_H
bool pFlow::rotatingAxis::read
(
	iIstream& is
)
{
	if( !rotatingAxis::timeInterval::read(is)) return false;
	if( !rotatingAxis::line::read(is)) return false;

	word omegaw;
	real omega;

	is >> omegaw >> omega;
	if( !is.check(FUNCTION_NAME))
	{
		ioErrorInFile(is.name(), is.lineNumber());
		return false;
	}
	if(omegaw != "omega")
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"  expected omega but found "<< omegaw <<endl;
		return false;
	}
	is.readEndStatement(FUNCTION_NAME);

	setOmega(omega);

	return true;
}

FUNCTION_H
bool pFlow::rotatingAxis::write
(
	iOstream& os
)const
{
	if( !rotatingAxis::timeInterval::write(os)) return false;
	if( !rotatingAxis::line::write(os) ) return false;
	
	os.writeWordEntry("omega", omega());
	return os.check(FUNCTION_NAME);
}