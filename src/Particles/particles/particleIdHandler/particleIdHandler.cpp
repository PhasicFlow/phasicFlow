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

pFlow::particleIdHandler::particleIdHandler(pointStructure& pStruct)
:
	uint32PointField_D
	(
		objectFile
		(
			"id",
			"",
			objectFile::READ_IF_PRESENT,
			objectFile::WRITE_ALWAYS
		),
		pStruct,
		static_cast<uint32>(-1),
		static_cast<uint32>(-1)
	)
{

}

bool
pFlow::particleIdHandler::hearChanges(
  const timeInfo& ti,
  const message& msg,
  const anyList& varList
)
{
	if(msg.equivalentTo(message::ITEMS_INSERT))
	{
		const word eventName = message::eventName(message::ITEMS_INSERT);

		const auto& indices = varList.getObject<uint32IndexContainer>(
			eventName);

		uint32 numNew = indices.size();
		if(numNew == 0u)return true;

		auto idRange = getIdRange(numNew);
		uint32Vector newId("newId",numNew,numNew,RESERVE());
		fillSequence(newId, idRange.first);
		
		return this->field().insertSetElement(indices, newId);
	}
	else
	{
		return uint32PointField_D::hearChanges(ti, msg, varList);
	}
}

pFlow::uniquePtr<pFlow::particleIdHandler>
pFlow::particleIdHandler::create(pointStructure& pStruct)
{
    word idHType = angleBracketsNames(
        "particleIdHandler", 
        pFlowProcessors().localRunTypeName());


	if( pointStructurevCtorSelector_.search(idHType) )
	{
		REPORT(1)<<"Creating particle id handler "<< 
			Green_Text(idHType)<<END_REPORT;
		return pointStructurevCtorSelector_[idHType] (pStruct);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< idHType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			pointStructurevCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}
