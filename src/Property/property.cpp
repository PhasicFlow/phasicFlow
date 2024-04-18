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

#include "property.hpp"
#include "dictionary.hpp"

bool pFlow::property::readDictionary()
{
	
	materials_ = getVal<wordVector>("materials");
	
	densities_ = getVal<realVector>("densities");

	if(materials_.size() != densities_.size() )
	{
		fatalErrorInFunction<<
		"  number of elements in material ("<<materials_.size()<<
		") is not equal to number of elements in densities ("<<densities_.size()<<
		") in dictionary "<< globalName()<<endl;
		return false;
	}

	return true;
}

bool pFlow::property::writeDictionary()
{

	if(!add("materials", materials_))
	{
		fatalErrorInFunction<<
		"  error in writing materials to dictionary "<< globalName()<<endl;
		return false;
	}

	if(!add("densities", densities_))
	{
		fatalErrorInFunction<<
		"  error in writing densities to dictionary "<< globalName()<<endl;
		return false;
	}

	return true;
}

bool pFlow::property::makeNameIndex()
{
	nameIndex_.clear();


	uint32 i=0;
	for(auto const& nm:materials_)
	{
		if(!nameIndex_.insertIf(nm, i))
		{
			fatalErrorInFunction<<
			"  repeated material name in the list of materials: " << materials_;
			return false;
		}
		i++;
	}
	nameIndex_.rehash(0);
	numMaterials_ = static_cast<uint32>(materials_.size());
	return true;
}


pFlow::property::property
(
	const word& fileName, 
	const wordVector& materials,
	const realVector& densities,
	repository* owner
)
:
	fileDictionary
	(
		objectFile
		(
			fileName,
			"",
			objectFile::READ_NEVER,
			objectFile::WRITE_NEVER
		),
		owner
	),
	materials_(materials),
	densities_(densities)
{
	
	if(!writeDictionary())
	{
		fatalExit;
	}

	if(!makeNameIndex())
	{
		fatalErrorInFunction<<
		"error in the input parameters of constructor. \n";
		fatalExit;
	}
}

pFlow::property::property
(
	const word& fileName,
	repository* owner
)
:
	fileDictionary
	(
		objectFile
		(
			fileName,
			"",
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		owner
	)
{
	
	if(!readDictionary())
	{
		fatalExit;
	}

	if(!makeNameIndex())
	{
		fatalErrorInFunction<<
		"error in dictionary "<< globalName()<<endl;
		fatalExit;
	}

}

pFlow::property::property
(
	const word &fileName, 
	const fileSystem &dir
)
:
	fileDictionary
	(
		objectFile
		(
			fileName,
			dir,
			objectFile::READ_ALWAYS,
			objectFile::WRITE_NEVER
		),
		nullptr
	)
{
	if(!readDictionary())
	{
		fatalExit;
	}

	if(!makeNameIndex())
	{
		fatalErrorInFunction<<
		"error in dictionary "<< globalName()<<endl;
		fatalExit;
	}
}
