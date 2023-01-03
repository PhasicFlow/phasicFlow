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

#include "vibrating.hpp"
#include "dictionary.hpp"


FUNCTION_H
pFlow::vibrating::vibrating(const dictionary& dict)
{

	if(!read(dict))
	{
		fatalErrorInFunction<<
		"  error in reading vibrating from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}



FUNCTION_H
bool pFlow::vibrating::read(const dictionary& dict)
{
	
	if(!timeInterval::read(dict))return false;

	angularFreq_	= dict.getVal<realx3>("angularFreq");

	phaseAngle_ 	= dict.getValOrSet<realx3>("phaseAngle", realx3(0));

	amplitude_ 		= dict.getVal<realx3>("amplitude");
	
	return true;
}

FUNCTION_H
bool pFlow::vibrating::write(dictionary& dict) const
{
	
	if(!timeInterval::write(dict)) return false;

	if( !dict.add("angularFreq", angularFreq_) )
	{
		fatalErrorInFunction<<
		"  error in writing angularFreq to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if( !dict.add("phaseAngle", phaseAngle_) )
	{
		fatalErrorInFunction<<
		"  error in writing phaseAngle to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if( !dict.add("amplitude", amplitude_) )
	{
		fatalErrorInFunction<<
		"  error in writing amplitude to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

FUNCTION_H
bool pFlow::vibrating::read(iIstream& is)
{
	
	notImplementedFunction;

	return true;
}

FUNCTION_H
bool pFlow::vibrating::write(iOstream& os)const
{
	if(!timeInterval::write(os)) return false;
	
	os.writeWordEntry("angularFreq", angularFreq_);
	os.writeWordEntry("phaseAngle", phaseAngle_);
	os.writeWordEntry("amplitude", amplitude_);
	return os.check(FUNCTION_NAME);
}