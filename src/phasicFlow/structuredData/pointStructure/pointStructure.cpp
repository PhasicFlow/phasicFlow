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


#include "pointStructure.hpp"
#include "pointStructureKernels.hpp"
#include "setFieldList.hpp"
#include "error.hpp"
#include "iOstream.hpp"
//#include "Time.hpp"
#include "mortonIndexing.hpp"

FUNCTION_H
bool pFlow::pointStructure::evaluatePointStructure()
{
	if(pointFlag_.size() != pointPosition_.size())
	{
		fatalErrorInFunction<<
		"number of elements in pointFlag and pointPosition is not equal \n";
		return false;
	}

	setNumMaxPoints();

	int32 minActive, maxActive;
	numActivePoints_ = pFlow::pointStructureKernels::scanPointFlag(
		0,
		numPoints_,
		static_cast<int8>(pointStructure::ACTIVE),
		pointFlag_.deviceVectorAll(),
		minActive,
		maxActive
		);
	
	activeRange_ = {minActive, maxActive};	
	
	return true;
}

FUNCTION_H
void pFlow::pointStructure::setNumMaxPoints()
{
	maxPoints_ = pointFlag_.capacity();
	numPoints_ = pointFlag_.size();
}

FUNCTION_H
pFlow::realx3Field_D& pFlow::pointStructure::pointPosition()
{
	return pointPosition_;
}

FUNCTION_H
pFlow::int8Field_HD& pFlow::pointStructure::pointFlag()
{
	return pointFlag_;
}
		
pFlow::uniquePtr<pFlow::int32IndexContainer> 
	pFlow::pointStructure::getNewPointsIndices(int32 numNewPoints)const
{


	if( capacity() - activeRange_.second >=  numNewPoints )
	{
		// fill the sequence starting from activeRange_.second-1
		return makeUnique<int32IndexContainer>(
			activeRange_.second,
			activeRange_.second+numNewPoints);

	}

	// second, check if there is space at the beginning
	if( activeRange_.first >= numNewPoints)
	{
		return makeUnique<int32IndexContainer>(
			0,
			numNewPoints);	
	}

	// otherwise scan the points from first to the end to find empty spaces 
	int32Vector newPoints(
		numNewPoints,
		RESERVE());

	newPoints.clear();
	int32 numAdded = 0;
	ForAll(i, pointFlag_)
	{
		if(!isActive(i))
		{
			newPoints.push_back(static_cast<int32>(i));
			numAdded++;
		}

		if(numAdded == numNewPoints)
		{
			return makeUnique<int32IndexContainer>(
				newPoints.data(),
				numNewPoints);
		}
	}

	// check if there is space at the end for the remaining of points
	if( numAdded <numNewPoints && capacity() - size() >= numNewPoints - numAdded )
	{
		int32 ind = size();
		for(int32 i=numAdded; i<numNewPoints; i++)
		{
			newPoints.push_back(ind);
			ind++;
		}
		
		return makeUnique<int32IndexContainer>(
			newPoints.data(),
			numNewPoints);
	}
	else
	{
		fatalErrorInFunction<<"not enough capacity for inserting particles into the point structure\n";
		return nullptr;
	}

	return nullptr;
}

pFlow::pointStructure::pointStructure()
:
	eventSubscriber(),	
	pointFlag_("pointFlag", "pointFlag", maxPoints_, 0 , RESERVE()),
	pointPosition_("pointPosition", "pointPosition", maxPoints_, 0, RESERVE()),
	activeRange_(0,0)
{}

pFlow::pointStructure::pointStructure
(
	const int8Vector& flgVec,
	const realx3Vector& posVec
)
:
	eventSubscriber(),	
	maxPoints_(posVec.capacity()),
	pointFlag_("pointFlag", "pointFlag", maxPoints_, 0 , RESERVE()),
	pointPosition_("pointPosition", "pointPosition", maxPoints_, 0, RESERVE()),
	activeRange_(0,0)
{
	
	pointFlag_.assign(flgVec);

	pointPosition_.assign(posVec);

	if( !evaluatePointStructure() )
	{
		fatalExit;
	}
}

pFlow::pointStructure::pointStructure
(
	const realx3Vector& posVec
)
:
	eventSubscriber(),	
	maxPoints_(posVec.capacity()),
	pointFlag_("pointFlag", "pointFlag", maxPoints_, 0 , RESERVE()),
	pointPosition_("pointPosition", "pointPosition", maxPoints_, 0, RESERVE()),
	activeRange_(0,0)
{

	pointPosition_.assign(posVec);

	pointFlag_.resize(pointPosition_.size(), static_cast<int8>(pointStructure::ACTIVE) );
	//pointFlag_.syncViews();

	if( !evaluatePointStructure() )
	{
		fatalExit;
	}
}

FUNCTION_H
const pFlow::realx3Field_D& pFlow::pointStructure::pointPosition()const
{
	return pointPosition_;
}

FUNCTION_H
const pFlow::int8Field_HD& pFlow::pointStructure::pointFlag()const
{
	return pointFlag_;
}

// - size of data structure
FUNCTION_H
pFlow::label pFlow::pointStructure::size()const
{
	return numPoints_;
}

FUNCTION_H
pFlow::label pFlow::pointStructure::capacity()const
{
	return maxPoints_;
}

FUNCTION_H
pFlow::label pFlow::pointStructure::numActive() const
{
	return numActivePoints_;
}

FUNCTION_H
bool pFlow::pointStructure::allActive()const
{
	return numActivePoints_ == numPoints_;
}

FUNCTION_H
bool pFlow::pointStructure::mortonSortPoints(const box& domain, real dx)
{
	if( !getSortedIndex(
		domain, 
		dx, 
		activeRange_,
		pointPosition_.deviceVectorAll(),
		pointFlag_.deviceVectorAll(),
		mortonSortedIndex_) )
	{
		fatalErrorInFunction<<"failed to perform morton sorting!"<<endl;
		return false;
	}

	pointPosition_.sortItems(mortonSortedIndex_);
	pointFlag_.sortItems(mortonSortedIndex_);

	auto oldSize 			= size();
	auto oldCapacity 	= capacity();
	auto oldRange 		= activeRange();

	// update size, range, capacity
	setNumMaxPoints();
	activeRange_ = {0, static_cast<int>(mortonSortedIndex_.size())};
	numActivePoints_ = mortonSortedIndex_.size();

	eventMessage msg(eventMessage::REARRANGE);

	if(oldSize != size() )
	{
		msg.add(eventMessage::SIZE_CHANGED);
	}

	if(oldCapacity != capacity())
	{
		msg.add(eventMessage::CAP_CHANGED);
	}

	if( oldRange != activeRange_)
	{
		msg.add(eventMessage::RANGE_CHANGED);
	}

	// notify all the registered objects except the exclusionList 
	if( !this->notify(msg) ) return false;

	return true;
}

FUNCTION_H
size_t pFlow::pointStructure::markDeleteOutOfBox(const box& domain)
{
	if(numPoints_==0) return 0;

	int32 minRange, maxRange;
	int32 numMarked = 
	pFlow::pointStructureKernels::markDeleteOutOfBox(
		domain,
		activeRange_.first,
		activeRange_.second,
		pointStructure::DELETED,
		pointPosition_.deviceVectorAll(),
		pointFlag_.deviceVectorAll(),
		activePointsMaskD(),
		minRange, maxRange );

	if(numMarked)
	{
		pointFlag_.modifyOnDevice();
		pointFlag_.syncViews();	
	}

	if( numMarked<=numActivePoints_)
	{
		numActivePoints_ -= numMarked;
	}
	else
	{
		fatalErrorInFunction<<
		"number of points marked as delete ("<<numMarked<<
		") is greater than the activePoints ("<<numActivePoints_<<
		").\n";
		fatalExit;
	}

	range newRange = {minRange, maxRange};
	
	if( activeRange_ != newRange )
	{
		activeRange_ = newRange;
		eventMessage msg(eventMessage::RANGE_CHANGED);
		
		// notify all the registered objects about active range change
		if( !this->notify(msg) ) 
		{
			fatalExit<<
			"something went wrong in notifying registered object about range change. \n";
			fatalExit;
		}
	}	

	return numMarked;
	
}

FUNCTION_H
bool pFlow::pointStructure::updateForDelete()
{
	notImplementedFunction;

	return true;
}

FUNCTION_H
pFlow::uniquePtr<pFlow::int32IndexContainer> pFlow::pointStructure::insertPoints
(
	const realx3Vector& pos,
	const setFieldList& setField,
	repository& owner,
	const List<eventObserver*>& exclusionList
)
{

	auto numNew = pos.size();
	if( numNew==0)
	{
		return makeUnique<int32IndexContainer>();
	}
		
	auto newPointsPtr = getNewPointsIndices( numNew );

	if(!newPointsPtr)return nullptr;

	auto oldSize 			= size();
	auto oldCapacity 	= capacity();
	auto oldRange 		= activeRange();

	tobeInsertedIndex_ = newPointsPtr();

	// set the position of new points
	
	if(!pointPosition_.insertSetElement(
		newPointsPtr(),
		pos)
		)return nullptr;

	
	if(!pointFlag_.insertSetElement(
		newPointsPtr(),
		static_cast<int8>(PointFlag::ACTIVE))
		)return nullptr;

	
	setNumMaxPoints();
	auto minInd = newPointsPtr().min();
	auto maxInd = newPointsPtr().max();


	List<eventObserver*> localExlusion(exclusionList);

	for(auto sfEntry:setField)
	{

		if(void* fieldPtr = 
			sfEntry.setPointFieldSelectedAll( 
				owner, 
				newPointsPtr(), 
				false );
			fieldPtr)

			localExlusion.push_back( 
				static_cast<eventObserver*>(fieldPtr)
				);
		else
			return nullptr;
	}

	// changes the active rage based on the new inserted points 
	activeRange_ = { static_cast<int>(min(activeRange_.first, minInd )),
					 static_cast<int>(max(activeRange_.second, maxInd+1))};

	numActivePoints_ += numNew;

	eventMessage msg(eventMessage::INSERT);

	if( oldRange != activeRange_ )
		msg.add(eventMessage::RANGE_CHANGED);

	if( oldSize != size() )
		msg.add(eventMessage::SIZE_CHANGED);

	if( oldCapacity != capacity() )
		msg.add(eventMessage::CAP_CHANGED);
	
	// notify all the registered objects except the exclusionList 
	if( !this->notify(msg, localExlusion) ) return nullptr;

	return newPointsPtr;
}



FUNCTION_H
bool pFlow::pointStructure::readPointStructure
(
	iIstream& is
)
{
	auto psCapacity = is.lookupDataOrSet("pointStructureCapacity", maxSizeDefault_);
	pointPosition_.reallocate(psCapacity);
	pointFlag_.reallocate(psCapacity);

	if( !pointPosition_.read(is))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"Error in reading pointPosition in pointStructure \n";
		return false;	
	}

	if(! pointFlag_.read(is, true))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		"Error in reading pointFlag in pointStructure \n";
		return false;
	}

	
	return evaluatePointStructure();
}

FUNCTION_H
bool pFlow::pointStructure::writePointStructure
(
	iOstream& os
)const
{
	os.writeWordEntry("pointStructureCapacity", maxPoints_);

	if(!pointPosition_.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing pointPosition to file \n";
		return false;
	}

	if(!pointFlag_.write(os))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"error in writing pointFlag to file \n";
		return false;
	}
	return true;
}


/*pFlow::uniquePtr<pFlow::int32Vector> 
pFlow::pointStructure::newPointsIndices(
	int32 numNewPoints
)const
{
	
	auto newPointsPtr = makeUnique<int32Vector>(
		"pointStructure::newPointsPtr",
		numNewPoints);

	auto& newPoints = newPointsPtr();
	

	// first, check if there is space at the end
	if( capacity() - activeRange_.second >=  numNewPoints )
	{
		// fill the sequence starting from activeRange_.second-1
		fillSequence(newPoints, activeRange_.second-1);
		return newPointsPtr;
	}

	// second, check if there is space at the beggining
	if( activeRange_.first >= numNewPoints)
	{
		// fill the sequence starting from 0
		fillSequence(newPoints, 0);
		return newPointsPtr;
	}

	// otherwise scan the points from first to the end to find empty spaces 
	newPoints.clear();
	int32 numAdded = 0;
	ForAll(i, pointFlag_)
	{
		if(!isActive(i))
		{
			newPoints.push_back(static_cast<int32>(i));
			numAdded++;
		}

		if(numAdded == numNewPoints)
		{
			
			return newPointsPtr;
		}
	}

	// check if there is space at the end for the remaining of points
	if( capacity() - size() >= numNewPoints - numAdded )
	{
		int32 ind = size();
		for(int32 i=numAdded; i<numNewPoints; i++)
		{
			newPoints.push_back(ind);
			ind++;
		}
		
		return newPointsPtr;
	}
	else
	{
		fatalErrorInFunction<<"not enough capacity for inserting particles into the point structure\n";
		return nullptr;
	}

	return nullptr;
}*/