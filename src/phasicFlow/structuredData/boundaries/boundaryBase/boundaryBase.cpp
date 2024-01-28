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

#include "boundaryBase.hpp"
#include "dictionary.hpp"
#include "internalPoints.hpp"

/*pFlow::boundaryBase::boundaryBase
(
	const plane& 		bplane,
	uint32 				mirrorProc, 
	const word& 		name,
	const word& 		type,
	internalPoints& 	internal
)
:
	subscriber(groupNames(name,type)),
	boundaryPlane_(bplane),
	name_(name),
	type_(type),
	mirrorProcessoNo_(mirrorProc),
	internal_(internal)
{
	
}*/

pFlow::boundaryBase::boundaryBase
(
	const dictionary& 	dict,
	const plane& 		bplane,
	internalPoints& 	internal
)
:
	subscriber(dict.name()),
	boundaryPlane_(bplane),
	indexList_(groupNames(dict.name(),"indexList")),
	neighborLength_(dict.getVal<real>("neighborLength")),
	internal_(internal),
	mirrorProcessoNo_(dict.getVal<uint32>("mirrorProcessorNo")),
	name_(dict.name()),
	type_(dict.getVal<word>("type"))
{

}

void pFlow::boundaryBase::setSize(uint32 newSize)
{
	indexList_.resize(newSize);

	if( indexList_.capacity() <= newSize+1 )
	{
		indexList_.reserve(newSize+1);
	}
}	

typename pFlow::boundaryBase::pointFieldAccessType
pFlow::boundaryBase::thisPoints()
{
    
    return pointFieldAccessType
    (
        indexList_.size(),
        indexList_.deviceVectorAll(),
        internal_.pointPosition().deviceVectorAll()
    );
   
}

typename pFlow::boundaryBase::pointFieldAccessType
    pFlow::boundaryBase::neighborPoints()
{
    notImplementedFunction;
    return pointFieldAccessType();
}

pFlow::uniquePtr<pFlow::boundaryBase> pFlow::boundaryBase::create(
    const dictionary &dict,
    const plane &bplane,
    internalPoints &internal)
{
	word type = dict.getVal<word>("type");
	word bType = angleBracketsNames("boundary", type);

	if( dictionaryvCtorSelector_.search(bType) )
	{
		return dictionaryvCtorSelector_[bType] (dict, bplane, internal);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< bType << " dose not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}