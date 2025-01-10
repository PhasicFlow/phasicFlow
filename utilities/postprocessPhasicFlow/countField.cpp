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

#include "countField.hpp"
#include "repository.hpp"
#include "twoPartEntry.hpp"

bool pFlow::countField::getFieldType(
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


pFlow::countField::countField(const dictionary& dict, repository& rep)
:
	dict_(dict),
	timeFolder_(rep)
{

	word includeMaskType = dict_.getVal<word>("includeMask");

	auto& incDict = dict_.subDictOrCreate(includeMaskType+"Info");
	
	includeMask_ = includeMask::create(incDict, includeMaskType, timeFolder_);

}


bool pFlow::countField::process(uint32& countedValue)
{
	auto& incMask = includeMask_();

	countedValue = 0;
	uint32 n = incMask.size();

	for(uint32 i=0; i<n; i++)
	{
		if( incMask(i) )
		{
			countedValue++;
		}
	}

	return true;
}