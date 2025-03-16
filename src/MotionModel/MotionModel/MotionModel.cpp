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


template<typename Model, typename Component>
inline
bool pFlow::MotionModel<Model, Component>::impl_nameToIndex(const word& name, uint32& indx)const
{
	if( auto i = componentNames_.findi(name); i == -1)
	{
		return false;
	}
	else
	{
		indx = static_cast<uint32>(i);
		return true;
	}
}

template<typename Model, typename Component>
inline
bool pFlow::MotionModel<Model, Component>::impl_indexToName(uint32 i, word& name)const
{
	if(i < numComponents_ )
	{
		name = componentNames_[i];
		return true;
	}
	else
	{
		return false;
	}
}

template<typename Model, typename Component>
inline
bool pFlow::MotionModel<Model, Component>::impl_readDictionary
(
	const dictionary& dict
)
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
	
	Vector<ModelComponent> components(
		"Read::modelComponent", 
		compNames.size()+1, 
		0, 
		RESERVE());
	
	componentNames_.clear();

		
	for(auto& cName: compNames)
	{
		auto& compDict = motionInfo.subDict(cName);
		
		if(auto compPtr = makeUnique<ModelComponent>(compDict); compPtr)
		{
			components.push_back(compPtr());
			componentNames_.push_back(cName);
		}
	}

	if( !componentNames_.search("none") )
	{
		components.push_back
		(
			impl_noneComponent()
		);
		componentNames_.push_back("none");	
	}

	motionComponents_.assign(components);
	numComponents_ = motionComponents_.size();

	return true;
}


template<typename Model, typename Component>
inline
bool pFlow::MotionModel<Model, Component>::impl_writeDictionary
(
	dictionary& dict
)const
{
	word modelName = getTypeName<ModelComponent>();

	dict.add("motionModel", modelName );

	auto modelDictName = modelName+"Info";

	auto& motionInfo = dict.subDictOrCreate(modelDictName);
	auto hostComponents = motionComponents_.hostView();

	ForAll(i, motionComponents_)
	{
		
		auto& axDict = motionInfo.subDictOrCreate(componentNames_[i]);
		if( !hostComponents[i].write(axDict))
		{
			fatalErrorInFunction<<
			"  error in writing axis "<< componentNames_[i] << " to dicrionary "
			<< motionInfo.globalName()<<endl;
			return false;
		}
	}

	return true;
}


template<typename Model, typename Component>
pFlow::MotionModel<Model, Component>::MotionModel()
:
	motionComponents_("motionComponents")
{}