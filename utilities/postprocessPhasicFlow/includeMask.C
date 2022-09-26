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


#include "includeMask.H"

pFlow::includeMask::includeMask(
	const dictionary& dict,
	const word& opType,
	readFromTimeFolder& timeFolder)
:
	operatorType_(opType),
	timeFolder_(timeFolder)
{
	if(!getFieldType(dict, timeFolder, fieldName_, fieldType_))
	{
		fatalExit;
	}

}



bool pFlow::includeMask::getFieldType(
	const dictionary& dict,
	readFromTimeFolder& timeFolder,
	word& fName,
	word& fType)
{
	
	fName = dict.getValOrSet<word>("field", "none");

	if(fName == "none")
	{
		fType = "int8";	
	}
	else
	{
		if( !timeFolder.pointFieldFileGetType(fName, fType) )
		{
			fatalErrorInFunction<<"error in reading field type from file "<< fName<<
			"in folder "<< timeFolder.path()<<endl;
			return false;
		}
	}
	
	return true;
}


pFlow::uniquePtr<pFlow::includeMask> pFlow::includeMask::create(
	const dictionary& dict,
	const word& opType,
	readFromTimeFolder& timeFolder)
{

	word fType, fName;
	if(!getFieldType(dict, timeFolder, fName, fType))
	{
		fatalExit;
		return nullptr;
	}

	word method = angleBracketsNames2("IncludeMask", fType, opType);
	
	if( dictionaryvCtorSelector_.search(method) )
	{
		auto objPtr = 
			dictionaryvCtorSelector_[method]
			(dict, opType, timeFolder);
		Report(2)<< dict.name()<< " with model "<<greenText(method)<<" is processed."<<endReport;
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


	
