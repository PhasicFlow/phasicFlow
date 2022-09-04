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


#include "property.H"
#include "dictionary.H"

bool pFlow::property::readDictionary
(
	const dictionary& dict
)
{
	
	materials_ = dict.getVal<wordVector>("materials");
	
	densities_ = dict.getVal<realVector>("densities");

	if(materials_.size() != densities_.size() )
	{
		fatalErrorInFunction<<
		"  number of elements in material ("<<materials_.size()<<
		") is not equal to number of elements in densities ("<<densities_.size()<<
		") in dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if(!makeNameIndex())
	{
		fatalErrorInFunction<<
		"  error in dictionary "<< dict.globalName()<<endl;
		return false;
	}
	return true;
}

bool pFlow::property::writeDictionary
(
	dictionary& dict
)const
{

	if(!dict.add("materials", materials_))
	{
		fatalErrorInFunction<<
		"  error in writing materials to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if(!dict.add("densities", densities_))
	{
		fatalErrorInFunction<<
		"  error in writing densities to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

bool pFlow::property::makeNameIndex()
{
	nameIndex_.clear();

	uint32 i=0;
	for(auto& nm:materials_)
	{
		if(!nameIndex_.insertIf(nm, i++))
		{
			fatalErrorInFunction<<
			"  repeated material name in the list of materials: " << materials_;
			return false;
		}
	}
	nameIndex_.rehash(0);
	numMaterials_ = materials_.size();
	return true;
}


pFlow::property::property
(
	const wordVector& materials,
	const realVector& densities
)
:
	materials_(materials),
	densities_(densities)
{
	if(!makeNameIndex())
	{
		fatalErrorInFunction<<
		"  error in the input parameters of constructor. \n";
		fatalExit;
	}
}

pFlow::property::property
(
const fileSystem& file
)
:
	dict_
	(
		makeUnique<dictionary>
		("property", true)
	)
{
	iFstream dictStream(file);

	if(!dict_().read(dictStream))
	{
		ioErrorInFile(dictStream.name(), dictStream.lineNumber());
		fatalExit;
	}

	if(!readDictionary(dict_()))
	{
		fatalExit;
	}

}

pFlow::property::property
(
	const dictionary& dict
)
:
	dict_
	(
		makeUnique<dictionary>(dict)
	)
{
	
	if(!readDictionary(dict_()))
	{
		fatalExit;
	}
}

