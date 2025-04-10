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

#include "auxFunctions.hpp"
#include "systemControl.hpp"


pFlow::uniquePtr<pFlow::auxFunctions> 
    pFlow::auxFunctions::create(const systemControl& control)
{
    const auto& setDict = control.settingsDict();

    if( !setDict.containsDataEntry("auxFunctions"))
    {
        return nullptr;
    }

    word func = setDict.getVal<word>("auxFunctions");

    if( systemControlvCtorSelector_.search(func) )
    {
        REPORT(1)<<"Creating auxiliary function "<< Green_Text(func)<< " ..."<<END_REPORT;
        return systemControlvCtorSelector_[func](control);
    }
    else
    {
        printKeys
		( 
			fatalError 
                << "Ctor Selector "<< Yellow_Text(func) << " dose not exist "
                << "for axuFunctions.\n"
			    <<"Avaiable ones are: \n"
			,
			systemControlvCtorSelector_
		);
		fatalExit;
        return nullptr;
    }
}
