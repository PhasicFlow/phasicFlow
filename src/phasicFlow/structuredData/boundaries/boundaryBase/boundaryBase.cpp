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
#include "anyList.hpp"
#include "Time.hpp"
#include "boundaryBaseKernels.hpp"


void pFlow::boundaryBase::setNewIndices
(
	const uint32Vector_D& newIndices
)
{
	indexList_.assign(newIndices, false);
}

void pFlow::boundaryBase::appendNewIndices
(
	const uint32Vector_D& newIndices
)
{
	indexList_.append(newIndices);
	
	// TODO: notify observers about this change 

	// the index list should be sorted  
	//sort(indexList_.deviceViewAll(), 0, newSize);
}

bool pFlow::boundaryBase::removeIndices
(
	uint32 numRemove,
	const uint32Vector_D& removeMask
)
{
    if(removeMask.size() != size()+1 )
	{
		fatalErrorInFunction<<"size mismatch"<<endl;
		return false;
	}

	uint32Vector_D removeIndices("removeIndices");
	uint32Vector_D keepIndices("keepIndices");

	pFlow::boundaryBaseKernels::createRemoveKeepIndices
	(
		indexList_,
		numRemove,
		removeMask,
		removeIndices,
		keepIndices
	);
	
	if(!internal_.deletePoints(removeIndices))
	{
		fatalErrorInFunction<<
		"error in deleting points from boundary "<< name()<<endl;
		return false;
	}

	setNewIndices(keepIndices);

	anyList aList;
	
	aList.emplaceBack(
		message::eventName(message::BNDR_RESET), 
		std::move(keepIndices));
	
	message msgBndry 	= message::BNDR_RESET;

	uint32 iter = internal_.time().currentIter();
	real t = internal_.time().currentTime();
	real dt = internal_.time().dt();

	if( !this->notify(iter, t, dt, msgBndry, aList) )
	{
		fatalErrorInFunction<<"Error in notify operation in boundary "<< 
		name_ <<endl;
		return false;
	}
	
	return true;
}

bool pFlow::boundaryBase::transferPoints
(
	uint32 numTransfer, 
	const uint32Vector_D& transferMask, 
	uint32 transferBoundaryIndex, 
	realx3 transferVector
)
{
	if(transferMask.size() != size()+1 )
	{
		fatalErrorInFunction<<"size mismatch"<<endl;
		return false;
	}

	uint32Vector_D transferIndices("transferIndices");
	uint32Vector_D keepIndices("keepIndices");

	pFlow::boundaryBaseKernels::createRemoveKeepIndices
	(
		indexList_,
		numTransfer,
		transferMask,
		transferIndices,
		keepIndices
	);

	// third, remove the indices from this list 
	setNewIndices(keepIndices);

	// first, change the flags in the internalPoints
	if( !internal_.changePointsFlag(
		transferIndices.deviceView(), 
		transferBoundaryIndex) ) 
	{
		return false;
	}

	// second, change the position of points 
	if(!internal_.changePointsPoisition(
		transferIndices.deviceView(),
		transferVector))
	{
		return false;
	}

	// fourth, add the indices to the mirror boundary 
	internal_.boundary(transferBoundaryIndex).
		appendNewIndices(transferIndices);
	
    return true;
}

pFlow::boundaryBase::boundaryBase
(
    const dictionary &dict,
    const plane &bplane,
    internalPoints &internal
)
: 
	subscriber(dict.name()),
	boundaryPlane_(bplane),
	indexList_(groupNames(dict.name(), "indexList")),
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
	/*if( indexList_.capacity() <= newSize+1 )
	{
		indexList_.reserve(newSize+1);
	}*/
	//INFORMATION<<"new size of boundary "<< name_<<" "<< indexList_.size()<<END_INFO;
}	

typename pFlow::boundaryBase::pointFieldAccessType
pFlow::boundaryBase::thisPoints()
{
    
    return pointFieldAccessType
    (
        indexList_.size(),
        indexList_.deviceViewAll(),
        internal_.pointPosition().deviceViewAll()
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