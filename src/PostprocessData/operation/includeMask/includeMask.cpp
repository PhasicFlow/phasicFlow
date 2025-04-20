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


#include "includeMask.hpp"
#include "dictionary.hpp"
#include "fieldsDataBase.hpp"


pFlow::includeMask::includeMask
(
	const dictionary& dict, 
	fieldsDataBase& fieldDB
)
:
	database_(fieldDB)
{}

pFlow::includeMask::includeMask
(
	const word &type, 
	const dictionary &opDict, 
	fieldsDataBase &fieldsDB
)
:
	database_(fieldsDB)
{
}

pFlow::uniquePtr<pFlow::includeMask> pFlow::includeMask::create
(
    const dictionary& opDict, 
    fieldsDataBase& fieldsDB
)
{
	word mask = opDict.getValOrSet<word>("includeMask", "all");
	word fieldType;
	if( mask != "all")
	{
		auto& maskDict = opDict.subDict(mask+"Info");
		word maskField = maskDict.getVal<word>("field");
		
		if( !fieldsDB.getFieldType(maskField, fieldType) )
		{
			fatalErrorInFunction<<"Error in retriving the type of field"
				<< maskField <<" from dictionary "
				<< maskDict.globalName()
				<< endl;
			fatalExit;
			return nullptr;
		}
	}
	else
	{
		fieldType = getTypeName<real>();
	}
	
	word method = angleBracketsNames2("IncludeMask", fieldType, mask);
	
	if( dictionaryvCtorSelector_.search(method) )
	{
		auto objPtr = 
			dictionaryvCtorSelector_[method]
			(opDict, fieldsDB);
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< 
			method << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
		return nullptr;
	}
	return nullptr;
}

pFlow::uniquePtr<pFlow::includeMask> 
	pFlow::includeMask::create
(
	const word &type, 
	const dictionary &opDict, 
	fieldsDataBase &fieldsDB
)
{
	word fieldType;
	if( type != "all")
	{
		auto& maskDict = opDict.subDict(type+"Info");
		word maskField = maskDict.getVal<word>("field");
		
		if( !fieldsDB.getFieldType(maskField, fieldType) )
		{
			fatalErrorInFunction<<"Error in retriving the type of field"
				<< maskField <<" from dictionary "
				<< maskDict.globalName()
				<< endl;
			fatalExit;
			return nullptr;
		}
	}
	else
	{
		fieldType = getTypeName<real>();
	}
	
	word method = angleBracketsNames2("IncludeMask", fieldType, type);
	
	if( wordvCtorSelector_.search(method) )
	{
		auto objPtr = 
			wordvCtorSelector_[method]
			(type, opDict, fieldsDB);
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< 
			method << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit;
		return nullptr;
	}
	return nullptr;
}
