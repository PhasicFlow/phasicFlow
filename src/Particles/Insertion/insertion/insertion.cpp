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


#include "particles.hpp"
#include "dictionary.hpp"
#include "insertion.hpp"
#include "streams.hpp"

bool pFlow::insertion::readInsertionDict
(
	const dictionary& dict
)
{

	active_ = dict.getVal<Logical>("active");

	if(active_)
		REPORT(1)<< "Particle insertion mechanism is "<<
		yellowText("active")<<" in the simulation."<<endREPORT;
	else
		REPORT(1)<< "Particle insertion mechanism is "<<
		yellowText("not active")<<" in the simulation."<<endREPORT;

	
	return true;
}

bool pFlow::insertion::writeInsertionDict
(
	dictionary& dict
)const
{
	if(!dict.add("active", active_) )
	{
		fatalErrorInFunction<<
		"  error in writing active to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("checkForCollision", checkForCollision_) )
	{
		fatalErrorInFunction<<
		"  error in writing checkForCollision to dictionary "<<dict.globalName()<<endl;
		return false;
	}

	return true;
}

pFlow::insertion::insertion
(
	particles& prtcl
)
:
	particles_(prtcl)
{}
