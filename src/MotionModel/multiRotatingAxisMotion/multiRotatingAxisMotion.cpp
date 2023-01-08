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

#include "multiRotatingAxisMotion.hpp"
#include "dictionary.hpp"
#include "vocabs.hpp"


bool pFlow::multiRotatingAxisMotion::readDictionary
(
	const dictionary& dict
)
{

	auto motionModel = dict.getVal<word>("motionModel");

	if(motionModel != "multiRotatingAxisMotion")
	{
		fatalErrorInFunction<<
		"  motionModel should be multiRotatingAxisMotion, but found "
		 << motionModel <<endl;
		return false;
	}

	auto& motionInfo = dict.subDict("multiRotatingAxisMotionInfo");
	auto axisNames = motionInfo.dictionaryKeywords();
	wordList rotationAxis;

	// first check if 
	
			
	for(auto& aName: axisNames)
	{
		auto& axDict = motionInfo.subDict(aName);
		
		if(auto axPtr = makeUnique<rotatingAxis>(axDict); axPtr)
		{
			rotationAxis.push_back(
				axDict.getValOrSet<word>("rotationAxis", "none"));
		}
		else
		{
			fatalErrorInFunction<<
			"could not read rotating axis from "<< axDict.globalName()<<endl;
			return false;
		}
	}

	if( !axisNames.search("none") )
	{
		axisNames.push_back("none");
		rotationAxis.push_back("none");
	}

	using intPair = std::pair<int32, int32>;

	std::vector<intPair> numRotAxis;

	for(size_t i=0; i< axisNames.size(); i++)
	{
		word rotAxis = rotationAxis[i];
		int32 n=0;
		while(rotAxis != "none")
		{
			n++;
			if(int32 iAxis = axisNames.findi(rotAxis) ; iAxis != -1)
			{
				rotAxis = rotationAxis[iAxis];	
			}else
			{
				fatalErrorInFunction<<
				"rotation axis name "<< rotAxis << "is does not exist!"<<endl;
				return false;
			}
			
		}

		numRotAxis.push_back({n,i});
	}

	auto compareFunc = [](const intPair& a, const intPair& b) 
		{ return a.first > b.first; };

	algorithms::STD::sort(numRotAxis.data(), numRotAxis.size(), compareFunc);

	sortedIndex_.clear();
	axisName_.clear();
	

	for(auto ax:numRotAxis)
	{
		axisName_.push_back(axisNames[ax.second]);
		sortedIndex_.push_back(ax.second);
	}

	numAxis_ = axisName_.size();
	axis_.clear();
	axis_.reserve(numAxis_);


	// create the actual axis vector 
	for(auto& aName: axisName_)
	{
		if(aName != "none")
		{
			auto& axDict = motionInfo.subDict(aName);
			axis_.push_back(
				multiRotatingAxis(this, axDict));	
		}
		else
		{
			axis_.push_back(
				multiRotatingAxis(this));
		}
		
	}

	return true;
}

bool pFlow::multiRotatingAxisMotion::writeDictionary
(
	dictionary& dict
)const
{
	dict.add("motionModel", "multiRotatingAxisMotion");

	auto& motionInfo = dict.subDictOrCreate("multiRotatingAxisMotionInfo");
	
	ForAll(i, axis_)
	{
		
		auto& axDict = motionInfo.subDictOrCreate(axisName_[i]);
		if( !axis_.hostVectorAll()[i].write(this,axDict))
		{
			fatalErrorInFunction<<
			"  error in writing axis "<< axisName_[i] << " to dicrionary "
			<< motionInfo.globalName()<<endl;
			return false;
		}
	}

	return true;
}

pFlow::multiRotatingAxisMotion::multiRotatingAxisMotion()
{}

pFlow::multiRotatingAxisMotion::multiRotatingAxisMotion
(
	const dictionary& dict
)
{
	if(! readDictionary(dict) )
	{
		fatalExit;
	}
}

FUNCTION_H
bool pFlow::multiRotatingAxisMotion::move(real t, real dt)
{

	// every thing is done on host 
	for(int32 i=0; i<numAxis_; i++)
	{	
		auto& ax = axis_[sortedIndex_[i]];
		ax.setTime(t);
		ax.setAxisList(getAxisListPtrHost());
		ax.move(dt);
	}

	// transfer to device
	axis_.modifyOnHost();
	axis_.syncViews();

	return true;
}

bool pFlow::multiRotatingAxisMotion::read
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

bool pFlow::multiRotatingAxisMotion::write
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