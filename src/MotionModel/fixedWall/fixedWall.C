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

#include "fixedWall.H"
#include "dictionary.H"
#include "vocabs.H"

bool pFlow::fixedWall::readDictionary
(
	const dictionary& dict
)
{

	auto motionModel = dict.getVal<word>("motionModel");

	if(motionModel != "fixedWall")
	{
		fatalErrorInFunction<<
		"  motionModel should be fixedWall, but found " << motionModel <<endl;
		return false;
	}
		
	return true;
}

bool pFlow::fixedWall::writeDictionary
(
	dictionary& dict
)const
{
	dict.add("motionModel", "fixedWall");

	auto& motionInfo = dict.subDictOrCreate("fixedWallInfo");

	return true;
}

pFlow::fixedWall::fixedWall()
{}

pFlow::fixedWall::fixedWall
(
	const dictionary& dict
)
{
	if(! readDictionary(dict) )
	{
		fatalExit;
	}
}

bool pFlow::fixedWall::read
(
	iIstream& is
)
{
	// create an empty file dictionary
	dictionary motionInfo(motionModelFile__, true);

	// read dictionary from stream
	if( !motionInfo.read(is) )
	{
		ioErrorInFile(is.name(), is.lineNumber()) <<
		"  error in reading dictionray " << motionModelFile__ <<" from file. \n";
		return false;
	}

	if( !readDictionary(motionInfo) ) return false;
	
	return true;
}

bool pFlow::fixedWall::write
(
	iOstream& os
)const
{
	// create an empty file dictionary
	dictionary motionInfo(motionModelFile__, true);

	if( !writeDictionary(motionInfo))
	{
		return false;
	}

	if( !motionInfo.write(os) )
	{
		ioErrorInFile( os.name(), os.lineNumber() )<<
		"  error in writing dictionray to file. \n";
		return false;
	}
	return true;
}