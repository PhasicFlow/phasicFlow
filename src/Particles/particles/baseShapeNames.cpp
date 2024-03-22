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

#include "baseShapeNames.hpp"

bool pFlow::baseShapeNames::createHashNames()
{
	hashNames_.clear();
	hashes_.clear();

	std::hash<word> hasher;

	uint32 i=0;
	for(const auto& nm:shapeNames_)
	{
		if(!hashNames_.insertIf(nm, i))
		{
			fatalErrorInFunction<<
			"  repeated name in the list of shape names: " << shapeNames_;
			return false;
		}
		hashes_.push_back(hasher(nm));
		i++;
	}
	hashNames_.rehash(0);
	
	return true;
}


bool pFlow::baseShapeNames::readFromDictionary1()
{
	
	shapeNames_ = getVal<wordVector>("names");
	numShapes_ = shapeNames_.size();

	return true;
}


pFlow::baseShapeNames::baseShapeNames
(
    const word &fileName, 
    repository *owner
)
:
    fileDictionary
    (
        objectFile
        (
            fileName,
            "",
            objectFile::READ_ALWAYS,
            objectFile::WRITE_ALWAYS
        ),
        owner
    )
{
	
	if( !readFromDictionary1() )
	{
		fatalErrorInFunction;
		fatalExit;
	}

	if( !createHashNames())
	{
		fatalErrorInFunction;
		fatalExit;
	}

}


bool pFlow::baseShapeNames::writeToDict(dictionary &dict)const
{
	
	if( !dict.add("names", shapeNames_) )
	{
		fatalErrorInFunction<<
		"  Error in writing names to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

bool pFlow::baseShapeNames::write(iOstream &os) const
{
	dictionary newDict(fileDictionary::dictionary::name(), true);

	if( !writeToDict(newDict) )
	{
		return false;
	}

	if( !newDict.write(os) )
	{
		fatalErrorInFunction<<"Error in writing dictionary "<< globalName()<<
		"to stream "<< os.name()<<endl; 
		return false;
	}

	return true;
}
