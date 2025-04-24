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

#include "postprocessComponent.hpp"
#include "fieldsDataBase.hpp"
#include "Time.hpp"


pFlow::postprocessData::postprocessComponent::postprocessComponent
( 
    const dictionary &dict,
    fieldsDataBase &fieldsDB, 
    const baseTimeControl &defaultTimeControl
)
:
    timeControl_(
        dict,
        defaultTimeControl,
        baseTimeControl(
            fieldsDB.time().startTime(),
            fieldsDB.time().endTime(),
            fieldsDB.time().saveInterval())
    ),
    fieldsDataBase_(fieldsDB)
{
}


pFlow::uniquePtr<pFlow::postprocessData::postprocessComponent> pFlow::postprocessData::postprocessComponent::create
(
    const dictionary& dict,
    fieldsDataBase& fieldsDB,
    const baseTimeControl& defaultTimeControl
)
{
    word method = dict.getVal<word>("processMethod");
    word region = dict.getVal<word>("processRegion");
    
    auto compModel = angleBracketsNames2("PostprocessComponent", region, method);

    if( dictionaryvCtorSelector_.search(compModel) )
    {
        REPORT(2)<<"Creating postprocess component "<< Green_Text(compModel)<<" ..."<<endl;
        return dictionaryvCtorSelector_[compModel](dict, fieldsDB, defaultTimeControl);
    }
    else
    {
        printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(compModel) << " dose not exist. \n"
			<<"Avaiable ones are: \n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
        return nullptr;
    }
}