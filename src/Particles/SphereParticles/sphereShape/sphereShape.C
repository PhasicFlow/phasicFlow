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

#include "sphereShape.H"
#include "dictionary.H"
#include "vocabs.H"
#include "streams.H"


bool pFlow::sphereShape::insertNames
(
	const wordVector& names
)
{
	names_.clear();
	uint32 i=0;
	for(const auto& nm:names)
	{
		if(!names_.insertIf(nm, i))
		{
			fatalErrorInFunction<<
			"  repeated name in the list of sphere names: " << names;
			return false;
		}
		i++;
	}
	names_.rehash(0);
	
	numShapes_ = names_.size();
	
	return true;
}


bool pFlow::sphereShape::readDictionary
(
	const dictionary& dict
)
{
	diameters_ = dict.getVal<realVector>("diameters");
	materials_ = dict.getVal<wordVector>("materials");
	auto names = dict.getVal<wordVector>("names");

	if(diameters_.size() != materials_.size() )
	{
		fatalErrorInFunction<<
		"  number of elements in diameters and properties are not the same in "<< dict.globalName()<<endl;
		return false;
	}
	else if(diameters_.size() != names.size() )
	{
		fatalErrorInFunction<<
		"  number of elements in diameters and names are not the same in "<< dict.globalName()<<endl;
		return false;
	}

	if( !insertNames(names) )
	{
		fatalErrorInFunction<<
		"  error in reading dictionary "<< dict.globalName();
		return false;
	}

	return true;
}

bool pFlow::sphereShape::writeDictionary
(
	dictionary& dict
)const
{

	if( !dict.add("diamters", diameters_) )
	{
		fatalErrorInFunction<<  
		"  Error in writing diameters to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	if( !dict.add("properties", materials_) )
	{
		fatalErrorInFunction<<
		"  Error in writing properties to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	size_t n = names_.size();
	wordVector names(n);
	names.clear();
	word nm;
	
	for(label i=0; i<n; i++)
	{
		indexToName(i, nm);
		names.push_back(nm);
	}

	if( !dict.add("names", names) )
	{
		fatalErrorInFunction<<
		"  Error in writing names to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}



pFlow::sphereShape::sphereShape
(
	const realVector& diameter,
	const wordVector& property,
	const wordVector& name
)
:
	diameters_(diameter),
	materials_(property)
{
	if( !insertNames( name) )
	{
		fatalExit;
	}
}

bool pFlow::sphereShape::shapeToDiameter
(
	wordVector& names,
	realVector& diams
)const
{
	diams.clear();
	uint32 idx;
	for(const auto& nm:names)
	{
		if(!nameToIndex(nm, idx))
		{
			fatalErrorInFunction<<
			"  invalid shape name requested "<< nm <<endl;
			return false;
		}
		diams.push_back(diameters_[idx]);
	}

	return true;
}


bool pFlow::sphereShape::read(iIstream& is)
{

	dictionary sphereDict(sphereShapeFile__, true);

	if( !sphereDict.read(is) )
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"  error in reading dictionray " << sphereShapeFile__ <<" from file. \n";
		return false;
	}

	if( !readDictionary(sphereDict) )
	{
		return false;
	}

	return true;
}

bool pFlow::sphereShape::write(iOstream& os)const
{

	dictionary sphereDict(sphereShapeFile__, true);

	if( !writeDictionary(sphereDict))
	{
		return false;
	}

	if( !sphereDict.write(os) )
	{
		ioErrorInFile( os.name(), os.lineNumber() )<<
		"  error in writing dictionray to file. \n";
		return false;
	}

	return true;
}
