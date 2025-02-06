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

#include "integration.hpp"
#include "pointStructure.hpp"
#include "repository.hpp"

bool pFlow::integration::insertValues
(
	const anyList& varList, 
	const deviceViewType1D<realx3>& srcVals,
	realx3PointField_D& dstFeild
)
{
	const word eventName = message::eventName(message::ITEMS_INSERT);

	if(!varList.checkObjectType<uint32IndexContainer>(eventName))
	{
		fatalErrorInFunction<<"Insertion failed in integration for "<< baseName_<<
		", variable "<< eventName <<
		" does not exist or the type is incorrect"<<endl;
		return false;
	}

	const auto& indices = varList.getObject<uint32IndexContainer>(
		eventName);
	
	dstFeild.field().insertSetElement(indices, srcVals);

    return true;
}

pFlow::integration::integration(
    const word &baseName,
    pointStructure &pStruct,
    const word &,
    const realx3Field_D &)
    : owner_(*pStruct.owner()),
      pStruct_(pStruct),
      baseName_(baseName)
{}


pFlow::uniquePtr<pFlow::integration> 
	pFlow::integration::create
(
		const word& baseName,
		pointStructure& pStruct,
		const word& method,
		const realx3Field_D& initialValField
)
{
	if( wordvCtorSelector_.search(method) )
	{
		return wordvCtorSelector_[method] (baseName, pStruct, method, initialValField);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< method << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
