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

#include "shape.hpp"

bool pFlow::shape::findPropertyIds()
{	
	shapePropertyIds_.resize(numShapes());

	ForAll( i, materialNames_)
	{	
		if(uint32 propId; property_.nameToIndex(materialNames_[i], propId) )
		{
			shapePropertyIds_[i] = propId;
		}
		else
		{
			fatalErrorInFunction<<"Material name "<< materialNames_[i]<< 
			" is not valid in dictionary "<<globalName()<<endl;
			return false;
		}
	}
    return true;
}

bool pFlow::shape::readFromDictionary2()
{

	materialNames_ = getVal<wordVector>("materials");

	if(materialNames_.size() != numShapes() )
	{
		fatalErrorInFunction<<
		"  number of elements in materials and names are not the same in "<< globalName()<<endl;
		return false;
	}

	return true;
}


pFlow::shape::shape
(
    const word &fileName, 
    repository *owner, 
    const property &prop
)
:
   baseShapeNames(fileName,owner),
    property_(prop)
{
	
	if( !readFromDictionary2() )
	{
		fatalErrorInFunction;
		fatalExit;
	}

	if(!findPropertyIds())
	{
		fatalErrorInFunction;
		fatalExit;
	}

}

pFlow::shape::shape
(
	const word &shapeType, 
	const word &fileName, 
	repository *owner, 
	const property &prop
)
:
	shape(fileName, owner, prop)
{
}

bool pFlow::shape::writeToDict(dictionary &dict)const
{
	if(!baseShapeNames::writeToDict(dict))return false;

	if( !dict.add("materials", materialNames_) )
	{
		fatalErrorInFunction<<
		"  Error in writing materials to dictionary "<< dict.globalName()<<endl;
		return false;
	}

	return true;
}

pFlow::uniquePtr<pFlow::shape> pFlow::shape::create
(
	const word &shapeType, 
	const word &fileName, 
	repository *owner, 
	const property &prop
)
{
	word type = angleBracketsNames("shape", shapeType);

	if( wordvCtorSelector_.search(type) )
	{
		auto objPtr = 
		wordvCtorSelector_[type]
			(shapeType, fileName, owner, prop);
		return objPtr;
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< 
			type << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			wordvCtorSelector_
		);
		fatalExit;
		return nullptr;
	}

}
