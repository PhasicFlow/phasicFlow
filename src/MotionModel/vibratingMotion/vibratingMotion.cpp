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

#include "vibratingMotion.hpp"
#include "dictionary.hpp"
#include "vocabs.hpp"


bool pFlow::vibratingMotion::readDictionary
(
	const dictionary& dict
)
{

	auto motionModel = dict.getVal<word>("motionModel");

	if(motionModel != "vibratingMotion")
	{
		fatalErrorInFunction<<
		"  motionModel should be vibratingMotion, but found "
		 << motionModel <<endl;
		return false;
	}

	auto& motionInfo = dict.subDict("vibratingMotionInfo");
	auto compNames = motionInfo.dictionaryKeywords();
	
	components_.reserve(compNames.size()+1);
	
	
	components_.clear();
	componentName_.clear();

		
	for(auto& cName: compNames)
	{
		auto& compDict = motionInfo.subDict(cName);
		
		if(auto compPtr = makeUnique<vibrating>(compDict); compPtr)
		{
			components_.push_back(compPtr());
			componentName_.push_back(cName);
		}
		else
		{
			fatalErrorInFunction<<
			"could not read vibrating motion from "<< compDict.globalName()<<endl;
			return false;
		}
	}



	if( !componentName_.search("none") )
	{
		components_.push_back
		(
			vibrating()
		);
		componentName_.push_back("none");	
	}

	components_.syncViews();
	numComponents_ = components_.size();
	

	return true;
}

bool pFlow::vibratingMotion::writeDictionary
(
	dictionary& dict
)const
{
	dict.add("motionModel", "vibratingMotion");

	auto& motionInfo = dict.subDictOrCreate("vibratingMotionInfo");
	
	ForAll(i, components_)
	{
		
		auto& compDict = motionInfo.subDictOrCreate(componentName_[i]);
		if( !components_.hostVectorAll()[i].write(compDict))
		{
			fatalErrorInFunction<<
			"  error in writing motion compoonent "<< componentName_[i] << " to dicrionary "
			<< motionInfo.globalName()<<endl;
			return false;
		}
	}

	return true;
}

pFlow::vibratingMotion::vibratingMotion()
{}

pFlow::vibratingMotion::vibratingMotion
(
	const dictionary& dict
)
{
	if(! readDictionary(dict) )
	{
		fatalExit;
	}
}

bool pFlow::vibratingMotion::read
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

bool pFlow::vibratingMotion::write
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