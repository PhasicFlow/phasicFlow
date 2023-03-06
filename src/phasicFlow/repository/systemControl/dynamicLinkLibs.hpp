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

#ifndef __dynamicLinkLibs_hpp__
#define __dynamicLinkLibs_hpp__


#include "hashMap.hpp"
#include "dictionary.hpp"

namespace pFlow
{


class dynamicLinkLibs
{
protected:

	wordHashMap<void*> libs_;

	void* open(word libName);

public:

	dynamicLinkLibs(const dictionary &dict, word libList = "libs");

	~dynamicLinkLibs();
	


};


} // pFlow


#endif // __dynamicLinkLibs_hpp__
