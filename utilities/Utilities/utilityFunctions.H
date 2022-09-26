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

#ifndef __utilityFunctions_H__
#define __utilityFunctions_H__

#include <regex>


namespace pFlow::utilities
{

bool inline pointFieldGetType(std::string TYPENAME, std::string& fieldType, std::string& fieldSpace)
{
	std::regex match("pointField\\<([A-Za-z1-9_]*)\\,([A-Za-z1-9_]*)\\>");
    std::smatch search;
	if(!std::regex_match(TYPENAME, search, match)) return false;
    if(search.size()!= 3) return false;
    fieldType  = search[1];
    fieldSpace = search[2];
	return true;
}


}


#endif //__utilityFunctions_H__