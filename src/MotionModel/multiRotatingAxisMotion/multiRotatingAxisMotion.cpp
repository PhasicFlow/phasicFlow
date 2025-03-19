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

void pFlow::multiRotatingAxisMotion::impl_setTime
(
	uint32 iter, 
	real t, 
	real dt
)const
{
	auto motion = motionComponents_.deviceViewAll();
	Kokkos::parallel_for(
		"multiRotatingAxisMotion::impl_setTime",
		deviceRPolicyStatic(0, numComponents_),
		LAMBDA_D(uint32 i){
			motion[i].setTime(t);
		});
	Kokkos::fence();
}

bool pFlow::multiRotatingAxisMotion::impl_move(uint32 iter, real t , real dt ) const
{
	
	auto motion = motionComponents_.deviceViewAll();
	Kokkos::parallel_for(
		"multiRotatingAxisMotion::impl_move",
		deviceRPolicyStatic(0, numComponents_),
		LAMBDA_D(uint32 i){
			motion[i].move(dt);
		});
	Kokkos::fence();
	return true;
}

bool pFlow::multiRotatingAxisMotion::impl_readDictionary(const dictionary &dict)
{
	auto modelName = dict.getVal<word>("motionModel");

	if(modelName != getTypeName<ModelComponent>())
	{
		fatalErrorInFunction<<
		"  motionModel should be "<< Yellow_Text(getTypeName<ModelComponent>())<<
		", but found "<< Yellow_Text(modelName)<<endl;
		return false;
	}

	auto& motionInfo = dict.subDict(modelName+"Info");
	auto compNames = motionInfo.dictionaryKeywords();
	
	wordList rotationAxisNames;

	
	// in the first round read all dictionaries 			
	for(auto& compName: compNames)
	{
		auto& axDict = motionInfo.subDict(compName);
		
		if(auto axPtr = makeUnique<rotatingAxis>(axDict); axPtr)
		{
			rotationAxisNames.push_back(
				axDict.getValOrSet<word>("rotationAxis", "none"));
		}
		else
		{
			fatalErrorInFunction<<
			"could not read rotating axis from "<< axDict.globalName()<<endl;
			return false;
		}
	}

	if( !compNames.search("none") )
	{
		compNames.push_back("none");
		rotationAxisNames.push_back("none");
	}

	using intPair = std::pair<int32, int32>;

	std::vector<intPair> numRotAxis;

	for(size_t i=0; i< compNames.size(); i++)
	{
		word rotAxis = rotationAxisNames[i];
		int32 n=0;
		while(rotAxis != "none")
		{
			n++;
			if(int32 iAxis = compNames.findi(rotAxis) ; iAxis != -1)
			{
				rotAxis = rotationAxisNames[iAxis];	
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

	std::sort(numRotAxis.begin(), numRotAxis.end(), compareFunc);
	Vector<int> sortedIndex; 
	componentNames_.clear();

	output<<compNames<<endl;
	
	for(auto ax:numRotAxis)
	{
		componentNames_.push_back(compNames[ax.second]);
		sortedIndex.push_back(ax.second);
	}

	numComponents_ = componentNames_.size();
	motionComponents_.reserve(numComponents_);
	sortedIndex_.assign(sortedIndex);

	Vector<ModelComponent> components("Read::modelComponent", 
		compNames.size()+1, 
		0, 
		RESERVE());
	
		
	for(auto& compName: componentNames_)
	{
		
		if(compName != "none")
		{
			auto& compDict = motionInfo.subDict(compName);
			components.emplace_back(
				motionComponents_.data(), 
				componentNames_, 
				compDict);
		}
		else
		{
			components.emplace_back(impl_noneComponent());
		}
		
	}

	motionComponents_.assign(components);
    return true;
}


bool pFlow::multiRotatingAxisMotion::impl_writeDictionary
(
	dictionary& dict
)const
{
	word modelName = "multiRotatingAxis";

	dict.add("motionModel", modelName );

	auto modelDictName = modelName+"Info";

	auto& motionInfo = dict.subDictOrCreate(modelDictName);
	auto hostComponents = motionComponents_.hostView();

	ForAll(i, motionComponents_)
	{
		
		auto& axDict = motionInfo.subDictOrCreate(componentNames_[i]);
		if( !hostComponents[i].write(axDict, componentNames_))
		{
			fatalErrorInFunction<<
			"  error in writing axis "<< componentNames_[i] << " to dicrionary "
			<< motionInfo.globalName()<<endl;
			return false;
		}
	}

	return true;
}

pFlow::multiRotatingAxisMotion::multiRotatingAxisMotion(
    const objectFile &objf,
    repository *owner)
    : fileDictionary(objf, owner)
{

	if(! getModel().impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

pFlow::multiRotatingAxisMotion::multiRotatingAxisMotion
(
	const objectFile &objf, 
	const dictionary &dict, 
	repository *owner
)
:
fileDictionary(objf, dict, owner)
{
	if(!getModel().impl_readDictionary(*this) )
	{
		fatalErrorInFunction;
		fatalExit;
	}
}

bool pFlow::multiRotatingAxisMotion::write
(
	iOstream &os, 
	const IOPattern &iop
) const
{
	// a global dictionary
	dictionary newDict(fileDictionary::dictionary::name(), true);
	if( iop.thisProcWriteData() )
	{
		if( !getModel().impl_writeDictionary(newDict) || 
			!newDict.write(os))
		{
			fatalErrorInFunction<<
			" error in writing to dictionary "<< newDict.globalName()<<endl;
			return false;
		}
	}
    return true;
}