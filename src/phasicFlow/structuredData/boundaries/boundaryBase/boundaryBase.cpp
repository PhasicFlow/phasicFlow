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

#include "boundaryBaseKernels.hpp"

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

void pFlow::boundaryBase::setNewIndices
(
	deviceViewType1D<uint32> newIndices
)
{
	auto newSize = newIndices.size();
	setSize(static_cast<uint32>(newSize));
	if(newSize>0u)
	{
		copy(indexList_.deviceView(), newIndices);
	}
}

void pFlow::boundaryBase::appendNewIndices
(
	deviceViewType1D<uint32> newIndices
)
{
	auto s = static_cast<uint32>(newIndices.size());
	if(s == 0) return;

	uint32 oldS = size();
	uint32 newSize = oldS + s; 

	setSize(newSize);
	auto appendView = Kokkos::subview(
		indexList_.deviceViewAll(),
		Kokkos::make_pair<uint32>(oldS, newSize));
	copy(appendView, newIndices);
	
	// TODO: notify observers about this change 

	// the index list should be sorted  
	//sort(indexList_.deviceViewAll(), 0, newSize);
}



bool pFlow::boundaryBase::removeIndices
(
	uint32 numRemove,
	deviceViewType1D<uint32> removeMask
)
{
    if(removeMask.size() != size()+1 )
	{
		fatalErrorInFunction<<"size mismatch"<<endl;
		return false;
	}

	deviceViewType1D<uint32> removeIndices("removeIndices", 1);
	deviceViewType1D<uint32> keepIndices("keepIndices",1);

	pFlow::boundaryBaseKernels::createRemoveKeepIndices
	(
		indexList_.deviceView(),
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

	//TODO: notify observers about changes 

	return false;
}

bool pFlow::boundaryBase::transferPoints
(
	uint32 numTransfer, 
	deviceViewType1D<uint32> transferMask, 
	uint32 transferBoundaryIndex, 
	realx3 transferVector
)
{
	if(transferMask.size() != size()+1 )
	{
		fatalErrorInFunction<<"size mismatch"<<endl;
		return false;
	}

	deviceViewType1D<uint32> transferIndices("transferIndices",1);
	deviceViewType1D<uint32> keepIndices("keepIndices",1);

	pFlow::boundaryBaseKernels::createRemoveKeepIndices
	(
		indexList_.deviceView(),
		numTransfer,
		transferMask,
		transferIndices,
		keepIndices
	);

	// third, remove the indices from this list 
	setNewIndices(keepIndices);

	// first, change the flags in the internalPoints
	if( !internal_.changePointsFlag(
		transferIndices, 
		transferBoundaryIndex) ) 
	{
		return false;
	}

	// second, change the position of points 
	if(!internal_.changePointsPoisition(
		transferIndices,
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