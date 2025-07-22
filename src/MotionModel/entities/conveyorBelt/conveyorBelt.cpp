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
		"  error in reading from dictionary "<< dict.globalName()<<endl;
		fatalExit;
	}
}

FUNCTION_H
bool pFlow::conveyorBelt::read(const dictionary& dict)
{	

	linearVelocity_ 		= dict.getVal<real>("linearVelocity");
    normal_             = dict.getVal<realx3>("normal");

    if(normal_.length() > verySmallValue)
    {
        normal_.normalize();
    }
    else
    {
        fatalErrorInFunction<<
            "  normal vector in "<< 
            dict.globalName() << 
            " cannot be zero vector "<<endl;
        return false;
    }
	
	return true;
}

FUNCTION_H
bool pFlow::conveyorBelt::write(dictionary& dict) const
{
	if( !dict.add("linearVelocity", linearVelocity_) )
	{
		fatalErrorInFunction<<
		"  error in writing tangentVelocity to dictionary "<< dict.globalName()<<endl;
		return false;
	}

    if(!dict.add("normal", normal_))
    {
        fatalErrorInFunction<<
        "  error in writing normal to dictionary "<< dict.globalName()<<endl;
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
    os.writeWordEntry("linearVelocity", linearVelocity_);
    os.writeWordEntry("normal", normal_);
    return true;
}