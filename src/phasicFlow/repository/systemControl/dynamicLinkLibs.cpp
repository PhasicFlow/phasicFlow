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

#include <dlfcn.h>

#include "dynamicLinkLibs.hpp"
#include "List.hpp"
#include "streams.hpp"


pFlow::dynamicLinkLibs::dynamicLinkLibs(
	const dictionary &dict, 
	word libList)
{

	wordList libNames;
	if(dict.containsDataEntry(libList))
	{
		libNames = dict.getVal<wordList>(libList);	
	}
	

	REPORT(1)<< "libs are "<< greenText(libNames)<<endREPORT;

	for(auto libName:libNames)
	{
		auto* hndl = open(libName);

		if(hndl)
		{
			libs_.insertIf(libName, hndl);
		}
		else
		{
			fatalExit;
		}
	}
}


pFlow::dynamicLinkLibs::~dynamicLinkLibs()
{
	for(auto &lib:libs_)
	{
		if( lib.second && dlclose(lib.second) != 0)
		{
			warningInFunction<< "could not close lib "<<lib.first<<endl;
		}
	}
}

void* pFlow::dynamicLinkLibs::open(word libName)
{
	REPORT(2)<<"Loading "<< greenText(libName)<<endREPORT;

	void* handle = dlopen(libName.c_str(), RTLD_LAZY|RTLD_GLOBAL);
	
	if(!handle)
	{
		warningInFunction<< "could not open lib "<< libName<<endl;
		return nullptr;
	}

	return handle;
}