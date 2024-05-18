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
#include "boundaryList.hpp"
#include "dictionary.hpp"
#include "internalPoints.hpp"
#include "anyList.hpp"
#include "Time.hpp"
#include "pointStructure.hpp"
#include "boundaryBaseKernels.hpp"

void pFlow::boundaryBase::setSize(uint32 newSize)
{
	indexList_.resize(newSize);
	unSyncLists();
}

void pFlow::boundaryBase::setNewIndices
(
	const uint32Vector_D& newIndices
)
{
	indexList_.assign(newIndices, false);
	unSyncLists();
}


bool pFlow::boundaryBase::appendNewIndices
(
	const uint32Vector_D& newIndices
)
{
	
	indexList_.append(newIndices);
	unSyncLists();

	message msg;

	msg.add(message::BNDR_APPEND);
	anyList varList;
	varList.emplaceBack( 
		message::eventName(message::BNDR_APPEND),
		newIndices);

	if(!notify(
		internal_.time().currentIter(),
		internal_.time().currentTime(),
		internal_.time().dt(),
		msg,
		varList))
	{
		fatalErrorInFunction;
		return false;
	}

	return true;
	
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
	
	if(!setRemoveKeepIndices(removeIndices, keepIndices))
	{
		return false;
	}

	anyList aList;
	
	aList.emplaceBack(
		message::eventName(message::BNDR_RESET), 
		std::move(keepIndices));
	
	message msgBndry 	= message::BNDR_RESET;

	uint32 iter = time().currentIter();
	real t = time().currentTime();
	real dt = time().dt();

	if( !this->notify(iter, t, dt, msgBndry, aList) )
	{
		fatalErrorInFunction<<"Error in notify operation in boundary "<< 
		name_ <<endl;
		return false;
	}
	
	return true;
}

bool pFlow::boundaryBase::setRemoveKeepIndices
(
	const uint32Vector_D &removeIndices, 
	const uint32Vector_D &keepIndices
)
{

	if(!internal_.deletePoints(removeIndices))
	{
		fatalErrorInFunction<<
		"error in deleting points from boundary "<< name()<<endl;
		return false;
	}

	setNewIndices(keepIndices);

    return true;
}

bool pFlow::boundaryBase::transferPointsToMirror
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

	// first, change the flags in the internalPoints
	if( !internal_.changePointsFlagPosition(
		transferIndices,
		transferVector, 
		thisBoundaryIndex(),
		transferBoundaryIndex) ) 
	{
		fatalErrorInFunction;
		return false;
	}

	// second, remove the indices from this list 
	setNewIndices(keepIndices);

	// third, add the indices to the mirror boundary 
	return mirrorBoundary().appendNewIndices(transferIndices);
	
}

pFlow::boundaryBase::boundaryBase
(
    const dictionary &dict,
    const plane 	&bplane,
    internalPoints 	&internal,
	boundaryList	&bndrs,
	uint32 			thisIndex
)
: 
	subscriber(dict.name()),
	boundaryPlane_(bplane),
	indexList_(groupNames("indexList", dict.name())),
	indexListHost_(groupNames("hostIndexList",dict.name())),
	neighborLength_(dict.getVal<real>("neighborLength")),
	updateInetrval_(dict.getVal<uint32>("updateInterval")),
	boundaryExtntionLengthRatio_(dict.getVal<real>("boundaryExtntionLengthRatio")),
	internal_(internal),
	boundaries_(bndrs),
	thisBoundaryIndex_(thisIndex),
	neighborProcessorNo_(dict.getVal<int32>("neighborProcessorNo")),
	isBoundaryMaster_(thisProcessorNo()>=neighborProcessorNo()),
	name_(dict.name()),
	type_(dict.getVal<word>("type"))
{
	unSyncLists();
}

const pFlow::pointStructure &pFlow::boundaryBase::pStruct() const
{
    return boundaries_.pStruct();
}

const pFlow::Time &pFlow::boundaryBase::time() const
{
    return boundaries_.pStruct().time();
}

pFlow::boundaryBase &pFlow::boundaryBase::mirrorBoundary()
{
    return boundaries_[mirrorBoundaryIndex()];
}

const pFlow::boundaryBase &pFlow::boundaryBase::mirrorBoundary() const
{
    return boundaries_[mirrorBoundaryIndex()];
}

typename pFlow::boundaryBase::pointFieldAccessType
pFlow::boundaryBase::thisPoints()const
{
    
    return pointFieldAccessType
    (
        indexList_.size(),
        indexList_.deviceViewAll(),
        internal_.pointPosition().deviceViewAll()
    );
   
}

pFlow::realx3Vector_D& pFlow::boundaryBase::neighborProcPoints()
{
	static realx3Vector_D dummyVector{"dummyVector"};
	notImplementedFunction;
	fatalExit;
	return dummyVector;
}

const pFlow::realx3Vector_D& 
pFlow::boundaryBase::neighborProcPoints()const
{
	static realx3Vector_D dummyVector{"dummyVector"};
	notImplementedFunction;
	fatalExit;
	return dummyVector;
}

pFlow::realx3 pFlow::boundaryBase::displacementVectroToMirror() const
{
	const plane& thisP = boundaryPlane();
	const plane& mirrorP = mirrorBoundary().boundaryPlane();
	return thisP.normal()*(thisP.d() + mirrorP.d());
}


pFlow::uniquePtr<pFlow::boundaryBase> pFlow::boundaryBase::create
(
    const dictionary &dict,
    const plane 	&bplane,
    internalPoints 	&internal,
	boundaryList	&bndrs,
	uint32 			thisIndex
)
{
	word type = dict.getVal<word>("type");
	word bType = angleBracketsNames("boundary", type);

	if( dictionaryvCtorSelector_.search(bType) )
	{
		return dictionaryvCtorSelector_[bType] (dict, bplane, internal, bndrs, thisIndex);
	}
	else
	{
		printKeys
		( 
			fatalError << "Ctor Selector "<< bType << " does not exist. \n"
			<<"Avaiable ones are: \n\n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
	}
	return nullptr;
}