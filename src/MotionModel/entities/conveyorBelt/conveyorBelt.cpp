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

#include "conveyorBelt.hpp"
#include "dictionary.hpp"


FUNCTION_H
pFlow::conveyorBelt::conveyorBelt(const dictionary& dict)
{
  if(!read(dict))
	{
		fatalErrorInFunction<<
		"  error in reading conveyorBelt from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}



FUNCTION_H
bool pFlow::conveyorBelt::read(const dictionary& dict)
{	

	velocity_ 		= dict.getVal<realx3>("velocity");
	
	return true;
}

FUNCTION_H
bool pFlow::conveyorBelt::write(dictionary& dict) const
{
	if( !dict.add("velocity", velocity_) )
	{
		fatalErrorInFunction<<
		"  error in writing velocity to dictionary "<< dict.globalName()<<endl;
		return false;
	}
	return true;
}

FUNCTION_H
bool pFlow::conveyorBelt::read(iIstream& is)
{
	
	notImplementedFunction;
	return true;
}

FUNCTION_H
bool pFlow::conveyorBelt::write(iOstream& os)const
{
  os.writeWordEntry("velocity", velocity_);
  return true;
}