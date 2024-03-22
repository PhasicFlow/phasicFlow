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

#include "particleIdHandler.hpp"

pFlow::particleIdHandler::particleIdHandler(uint32PointField_D &id)
:
	id_(id)
{
}

pFlow::uniquePtr<pFlow::particleIdHandler> 
	pFlow::particleIdHandler::create(uint32PointField_D &id)
{
    word idHType = angleBracketsNames(
        "particleIdHandler", 
        pFlowProcessors().localRunTypeName());


	if( pointFieldvCtorSelector_.search(idHType) )
	{
		REPORT(1)<<"Creating particle id handler "<< 
			Green_Text(idHType)<<END_REPORT;
		return pointFieldvCtorSelector_[idHType] (id);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< idHType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			pointFieldvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
