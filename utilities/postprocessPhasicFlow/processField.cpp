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

#include "processField.hpp"
#include "pointRectCell.hpp"
#include "repository.hpp"
#include "twoPartEntry.hpp"


pFlow::processField::processField(
	const dictionary& dict,
	pointRectCell& pToCell,
	repository& rep)
:
	dict_(dict),
	pointToCell_(pToCell),
	timeFolder_(rep),
	processedFieldName_(dict.name()),
	operation_(dict.getVal<word>("operation")),
	includeMaskType_(dict.getVal<word>("includeMask")),
	threshold_(dict.getValOrSetMax<int32>("threshold", 1))
{
	
	if(!processField::getFieldType(
		dict_, 
		timeFolder_,
		fieldName_,
		fieldType_) )
	{
		fatalExit;
	}
	
	auto& incDict = dict_.subDictOrCreate(includeMaskType_+"Info");
	
	includeMask_ = includeMask::create(incDict, includeMaskType_, timeFolder_);

}

bool pFlow::processField::getFieldType(
	const dictionary& dict,
	readFromTimeFolder& timeFolder,
	word& fieldName,
	word& fieldType)
{
	if(dict.containsDataEntry("field"))
	{
		const dataEntry& entry = dict.dataEntryRef("field");

		if( isTwoPartEntry(entry))
		{
			twoPartEntry tpEntry(entry);
			fieldName = "uniformField";
			fieldType = tpEntry.firstPart();
		}
		else
		{
			fieldName = dict.getVal<word>("field");
			if( !timeFolder.pointFieldFileGetType(fieldName, fieldType) )
			{
				fatalErrorInFunction<<"error in reading field type from file "<< fieldName<<
				"in folder "<< timeFolder.path()<<endl;
				return false;
			}
		}
	}
	else
	{
		fatalErrorInFunction<< "dictionary "<< dict.globalName()<<
		"does not contain field keyword"<<endl;
		return false;
	}

	return true;
}


pFlow::uniquePtr<pFlow::processField> 
pFlow::processField::create(
	const dictionary& dict,
	pointRectCell& pToCell,
	repository& rep)
{

	word  fName, fType;
	readFromTimeFolder timeFolder(rep);
	if(!getFieldType(dict, timeFolder, fName, fType))
	{
		fatalExit;
		return nullptr;
	}
	
	auto method = angleBracketsNames("ProcessField", fType);

	if( dictionaryvCtorSelector_.search(method) )
	{
		auto objPtr = 
			dictionaryvCtorSelector_[method]
			(dict, pToCell, rep);
		REPORT(2)<<"Processing/creating " << Yellow_Text(dict.name())<< " with model "<<Green_Text(method)<<"."<<END_REPORT;
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
}