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

#include "stationary.hpp"



FUNCTION_H
pFlow::stationary::stationary(const dictionary& dict)
{
}



FUNCTION_H
bool pFlow::stationary::read(const dictionary& dict)
{	
	return true;
}

FUNCTION_H
bool pFlow::stationary::write(dictionary& dict) const
{
	
	return true;
}

FUNCTION_H
bool pFlow::stationary::read(iIstream& is)
{
	
	notImplementedFunction;
	return true;
}

FUNCTION_H
bool pFlow::stationary::write(iOstream& os)const
{
  return true;
}