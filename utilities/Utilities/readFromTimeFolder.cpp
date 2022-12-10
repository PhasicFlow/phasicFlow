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

#include "readFromTimeFolder.hpp"
 
pFlow::readFromTimeFolder::readFromTimeFolder(repository& rep)
:
	repository_(rep)
{}


std::pair<bool, pFlow::IOfileHeader> 
	pFlow::readFromTimeFolder::fieldExists(word fieldName)const
{
	IOfileHeader fieldHeader(
		objectFile(
			fieldName,
			repository_.path(),
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER)
		);
	
	return std::make_pair( fieldHeader.headerOk(true), fieldHeader);
}

bool pFlow::readFromTimeFolder::pointFieldFileGetType(
	word fieldName,
	word& typeName) const
{
	
	word fileTypeName, space;
	if( auto [exist, fieldHeader]= fieldExists(fieldName); !exist )
	{
		fatalErrorInFunction<< "Folder "<< repository_.path() <<
		" does not contain " << fieldName << " field."<<endl;
		fatalExit;
		return false;
	}
	else
	{
		fileTypeName = fieldHeader.objectType();
	}

	typeName.clear();		
	word flType{};
	
	if( !pFlow::utilities::pointFieldGetType(
		fileTypeName, flType, space) )
	{
		fatalErrorInFunction<<
		"error in extracting type from "<<fileTypeName<<endl;
		fatalExit;
		return false;
	}

	typeName = flType;
	return true;
}