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
#ifndef __pointFlagKernels_hpp__
#define __pointFlagKernels_hpp__

template<typename ExecutionSpace>
pFlow::ViewType1D<pFlow::uint32, typename pFlow::pointFlag<ExecutionSpace>::memory_space>
	pFlow::pointFlag<ExecutionSpace>::getActivePoints()
{
	using rpAPoints = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	ViewType1D<pFlow::uint32,memory_space> 
		aPoints("activePoints", activeRange_.end()+1);
	

	Kokkos::parallel_for(
		"pFlow::pointFlag<ExecutionSpace>::getActivePoints",
		rpAPoints(0,activeRange_.end()),
		CLASS_LAMBDA_HD(uint32 i)
		{
			if( isActive(i))
			{
				aPoints[i] = 1;
			}
			else
			{
				aPoints[i] = 0;
			}
		}
	);
	Kokkos::fence();
	return aPoints;
}


template<typename ExecutionSpace>
pFlow::uint32 pFlow::pointFlag<ExecutionSpace>::markOutOfBoxDelete
(
	const box& validBox,
	ViewType1D<realx3, memory_space> points
)
{
	using rpScanFlag = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	uint32 numDeleted = 0;
	
	uint32 start = activeRange().start();
	uint32 end   = activeRange().end();

	uint32 minRange = end;
	uint32 maxRange = start;

	if(start<end)
	{
		Kokkos::parallel_reduce(
			"pointFlagKernels::markOutOfBox",
			rpScanFlag(start, end),
			CLASS_LAMBDA_HD(
				uint32 i, 
				uint32& minUpdate, 
				uint32& maxUpdate, 
				uint32& delToUpdate)
			{
				if(isActive(i))
				{
					if(validBox.isInside(points[i]))
					{
						
						minUpdate = min(minUpdate,i);
						maxUpdate = max(maxUpdate,i);
					}
					else
					{
						flags_[i] = DELETED;
						delToUpdate++;
					}

				}
				
			},
			Kokkos::Min<uint32>(minRange), 
			Kokkos::Max<uint32>(maxRange),
			numDeleted);	
	}
	
	if(numDeleted >= numActive_)
	{
		minRange = 0;
		maxRange = 0;
		numDeleted == numActive_;
	}
	else
	{
		// add one to maxRange to make it half-open
		maxRange ++;
	}

	activeRange_ = {minRange, maxRange};
	numActive_ 	= numActive_ - numDeleted;
	isAllActive_ = 
		(activeRange_.numElements() == numActive_)&& numActive_>0;
	
	return numDeleted;
}

template<typename ExecutionSpace>
pFlow::uint32 
pFlow::pointFlag<ExecutionSpace>::addInternalPoints
(
	const ViewType1D<uint32, memory_space>& points
)
{
	if(points.extent(0)==0u)return 0;

	uint32 minRange;
	uint32 maxRange;
	uint32 numAdded = 0;
	
	
	Kokkos::parallel_reduce(
		"pointFlagKernels::addInternalPoints",
		deviceRPolicyStatic(0, points.extent(0)),
		LAMBDA_HD(
			uint32 i, 
			uint32& minUpdate, 
			uint32& maxUpdate, 
			uint32& addToUpdate)
		{
			uint32 idx = points(i);
			if( flags_[idx] <= DELETED) addToUpdate ++;
			minUpdate = min(minUpdate,idx);
			maxUpdate = max(maxUpdate,idx);
			flags_[idx] = INTERNAL;	
		},
		Kokkos::Min<uint32>(minRange), 
		Kokkos::Max<uint32>(maxRange),
		numAdded);	

	// add one to max range to make range it half open 
	maxRange++;
	
	minRange = min(activeRange_.start(), minRange);
	maxRange = max(activeRange_.end(), maxRange);

	activeRange_ = {minRange, maxRange};
	numActive_ 	= numActive_ + numAdded;
	
	isAllActive_ = activeRange_.numElements() == numActive_;
	
	return numAdded;
}

template<typename ExecutionSpace>
bool pFlow::pointFlag<ExecutionSpace>::deletePoints
(
	scatteredFieldAccess<uint32, memory_space>  points
)
{
	if(points.empty())return true;
	
	
	using policy = Kokkos::RangePolicy<
		execution_space,  
		Kokkos::IndexType<uint32>>;

	uint32 numDeleted = 0;
	Kokkos::parallel_reduce
	(
		"pointFlagKernels::deletePoints",
		policy(0u, points.size()),
		CLASS_LAMBDA_HD(uint32 i, uint32& valDelUpdate)
		{
			uint32 n = points(i);
			if(isActive(n))
			{
				valDelUpdate++;
				flags_[n] = Flag::DELETED;
			}
		},
		numDeleted
	);	

	if(numDeleted >= numActive_)
	{
		activeRange_ = {0, 0};
		numDeleted == numActive_;
	}
		
	numActive_ 	= numActive_ - numDeleted;
	isAllActive_ = 
		(activeRange_.numElements() == numActive_)&& numActive_>0;
	
	return true;
}
template<typename ExecutionSpace>
bool pFlow::pointFlag<ExecutionSpace>::deletePoints
(
	ViewType1D<uint32, memory_space> points
)
{

	uint32 s = points.size();
	if(s==0u)return true;
	
	using policy = Kokkos::RangePolicy<
		execution_space,  
		Kokkos::IndexType<uint32>>;

	uint32 numDeleted = 0;
	Kokkos::parallel_reduce
	(
		"pointFlagKernels::deletePoints",
		policy(0u, points.size()),
		CLASS_LAMBDA_HD(uint32 i, uint32& valDelUpdate)
		{
			uint32 n = points(i);
			if(isActive(n))
			{
				valDelUpdate++;
				flags_[n] = Flag::DELETED;
			}
		},
		numDeleted
	);	

	if(numDeleted >= numActive_)
	{
		activeRange_ = {0, 0};
		numDeleted == numActive_;
	}
		
	numActive_ 	= numActive_ - numDeleted;
	isAllActive_ = 
		(activeRange_.numElements() == numActive_)&& numActive_>0;
	
	return true;
}


template<typename ExecutionSpace>
bool pFlow::pointFlag<ExecutionSpace>::changeFlags
(
	ViewType1D<uint32, memory_space> changePoints, 
	uint32 boundaryIndex
)
{
	auto flg = getBoundaryFlag(boundaryIndex);
	Kokkos::parallel_for
	(
		"pointFlag::changeFlags",
		rPolicy(0, changePoints.size()),
		LAMBDA_HD(uint32 i)
		{
			flags_[changePoints(i)] = flg;
		}
	);
	Kokkos::fence();
	return true;
}

template<typename ExecutionSpace>
pFlow::uint32 pFlow::pointFlag<ExecutionSpace>::markPointRegions
(
	domain 								dm,
	ViewType1D<realx3, memory_space> 	points,
	real leftLength,
	real rightLength,
	real bottomLength,
	real topLength,
	real rearLength,
	real frontLength)
{
	
	using rpMark = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	uint32 start = activeRange().start();
	uint32 end   = activeRange().end();

	uint32 minRange = end;
	uint32 maxRange = start;

	uint32 numMarkedDelete = 0;
	uint32 nLeft = 0, nRight = 0;
	uint32 nBottom = 0, nTop = 0;
	uint32 nRear = 0, nFront = 0;

	if(start<end)
	{
		
		Kokkos::parallel_reduce(
			"pointFlagKernels::markDeleteInDomain",
			rpMark(start,end),
			CLASS_LAMBDA_HD(
				uint32 i, 
				uint32& minUpdate, 
				uint32& maxUpdate, 
				uint32& valDelete,
				uint32& valLeft,
				uint32& valRight,
				uint32& valBottom,
				uint32& valTop,
				uint32& valRear,
				uint32& valFront){
				if(this->isActive(i))
				{
					realx3 p = points[i];
					if( !dm.domainBox().isInside(p) )
					{
						flags_[i] = DELETED;
						valDelete++;
					}
					else
					{

						uint8 flg  = 0;
						minUpdate = min(minUpdate,i);
						maxUpdate = max(maxUpdate,i);

						if(dm.left().inPositiveDistance(p, leftLength))
						{
							flg += LEFT;
							valLeft++;	
						} 

						if(dm.right().inPositiveDistance(p, rightLength))
						{
							flg += RIGHT;
							valRight++;
						}
						
						if(dm.bottom().inPositiveDistance(p, bottomLength))
						{
							flg += BOTTOM;
							valBottom++;
						} 

						if(dm.top().inPositiveDistance(p, topLength)) 
						{
							flg += TOP;
							valTop++;
						}
						
						if(dm.rear().inPositiveDistance(p, rearLength)) 
						{
							flg += REAR;
							valRear++;
						}

						if(dm.front().inPositiveDistance(p, frontLength))
						{
							flg += FRONT;
							valFront++;
						} 

						flags_[i] = flg>static_cast<uint8>(0)? flg: INTERNAL;
					}
				}
			},
			Kokkos::Min<uint32>(minRange), 
			Kokkos::Max<uint32>(maxRange),
			numMarkedDelete,
			nLeft,
			nRight,
			nBottom,
			nTop,
			nRear,
			nFront);

	}
	
	// means either range was empty or all points have been deleted.
	if(numMarkedDelete>= numActive_)
	{
		minRange = 0;
		maxRange = 0;
		numMarkedDelete =  numActive_;
	}
	else
	{
		maxRange++; // add one to make it half
	}

	activeRange_ = {minRange, maxRange};
	numActive_ 	-= numMarkedDelete;
	isAllActive_ = (activeRange_.numElements() == numActive_)&& numActive_>0;

	nLeft_ 	= nLeft;
	nRight_	= nRight;
	nBottom_= nBottom;
	nTop_ 	= nTop;
	nRear_	= nRear;
	nFront_ = nFront;

	return numMarkedDelete;
}

template<typename ExecutionSpace>
void pFlow::pointFlag<ExecutionSpace>::fillNeighborsLists
(
	ViewType1D<uint32, memory_space> leftList,
	ViewType1D<uint32, memory_space> rightList,
	ViewType1D<uint32, memory_space> bottomList,
	ViewType1D<uint32, memory_space> topList,
	ViewType1D<uint32, memory_space> rearList,
	ViewType1D<uint32, memory_space> frontList
)
{
	using rpMark = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	uint32 start = activeRange().start();
	uint32 end   = activeRange().end();

	ViewType1D<uint32, memory_space> nElems("nElems",6);
	
	fill(nElems, 0, 6, static_cast<uint32>(0));
		
	if(start<end)
	{
		Kokkos::parallel_for(
			"pointFlagKernels::markDeleteInDomain",
			rpMark(start,end),
			CLASS_LAMBDA_HD(uint32 i){

				uint8 flg = flags_(i);
				if(this->isBoundary(flg))
				{
					if(this->isLeft(flg)) leftList[Kokkos::atomic_fetch_add(&nElems[0],1)] = i;
					if(this->isRight(flg))rightList[Kokkos::atomic_fetch_add(&nElems[1],1)] = i;
					if(this->isBottom(flg))bottomList[Kokkos::atomic_fetch_add(&nElems[2],1)] = i;
					if(this->isTop(flg))topList[Kokkos::atomic_fetch_add(&nElems[3],1)] = i;
					if(this->isRear(flg))rearList[Kokkos::atomic_fetch_add(&nElems[4],1)] = i;
					if(this->isFront(flg))frontList[Kokkos::atomic_fetch_add(&nElems[5],1)] = i;
				}
			});
		Kokkos::fence();	
	}
}

template<typename ExecutionSpace>
pFlow::uint32 pFlow::pointFlag<ExecutionSpace>::markDelete
(
	const plane& 		pln,
	ViewType1D<realx3, memory_space> 	points,
	ViewType1D<uint32, memory_space> 	indices,
	ViewType1D<uint32, memory_space> 	onOff
)
{
	// if it is empty, do nothing
	if(indices.size() == 0 )return 0;

	using rpMark = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	uint32 start = activeRange().start();
	uint32 end   = activeRange().end();

	uint32 minRange = end;
	uint32 maxRange = start;

	uint32 numMarked = 0;

	if(start<end)
	{
		Kokkos::parallel_reduce(
			"pointFlagKernels::markDelete",
			rpScanFlag(0, indices.size()),
			CLASS_LAMBDA_HD(
				uint32 i, 
				uint32& minUpdate, 
				uint32& maxUpdate, 
				uint32& sumToUpdate)
			{
				auto indx = indices(i);
				
				if( pln.pointInNegativeSide(points(indx)))
				{
					flags_[indx] = DELETED;
					sumToUpdate++;
					onOff[i] = 0;
				}
				else
				{
					minUpdate = min(minUpdate,i);
					maxUpdate = max(maxUpdate,i);
					onOff[i] = 1;
				}
			},
			Kokkos::Min<uint32>(minRange), 
			Kokkos::Max<uint32>(maxRange),
			numMarked);	
	}

	// means either range was empty or all points have been deleted.
	if(minRange<start || maxRange>end)
	{
		minRange = 0;
		maxRange = 0;
	}
	else
	{
		maxRange++; // add one to make it half
	}

	activeRange_ = {minRange, maxRange};
	isAllActive_ = isAllActive_ && numMarked == 0; 
	numActive_ 	-= numMarked;

	return numMarked;
}

template<typename ExecutionSpace>
pFlow::uint32 pFlow::pointFlag<ExecutionSpace>::changeCapacity
(
	uint32 reqEmptySpots
)
{
	
	uint32 oldCap = capacity();
	uint32 newCap = oldCap* pFlow::gSettings::vectorGrowthFactor__+1;
	uint32 emptySpots = newCap - numActive_;

	while( emptySpots < reqEmptySpots )
	{
		newCap = newCap*pFlow::gSettings::vectorGrowthFactor__+1;
		emptySpots = newCap - numActive_;
	}
	
	viewType newFlags(flags_.label(), newCap);
	// copy contnent 
	copy(newFlags, 0u, flags_, 0u, oldCap);
	fill(newFlags, oldCap, newCap, static_cast<uint8>(DELETED));

	// reference replacement 
	flags_ = newFlags;

	return newCap;
}

#endif // __pointFlagKernels_hpp__




/*template<typename ExecutionSpace>
pFlow::uint32 pFlow::pointFlag<ExecutionSpace>::scanPointFlag()
{
		
	using rpScanFlag = Kokkos::RangePolicy<execution_space,  
			Kokkos::IndexType<uint32>>;

	uint32 numActive = 0;
	
	
	uint32 start = activeRange().start();
	uint32 end   = activeRange().end();

	uint32 minRange = end;
	uint32 maxRange = start;

	if(start<end)
	{
		Kokkos::parallel_reduce(
			"pointFlagKernels::scanPointFlag",
			rpScanFlag(start, end),
			CLASS_LAMBDA_HD(
				uint32 i, 
				uint32& minUpdate, 
				uint32& maxUpdate, 
				uint32& sumToUpdate)
			{
				if(this->isActive(i))
				{
					sumToUpdate++;
					minUpdate = min(minUpdate,i);
					maxUpdate = max(maxUpdate,i);
				}
			},
			Kokkos::Min<uint32>(minRange), 
			Kokkos::Max<uint32>(maxRange),
			numActive);	
	}
	
	if(numActive==0)
	{
		minRange = 0;
		maxRange = 0;
	}
	else
	{
		// add one to maxRange to make it half-open
		maxRange ++;
	}

	activeRange_ = {minRange, maxRange};
	numActive_ 	= numActive;
	isAllActive_ = activeRange_.numElements() == numActive_;

	return numActive;
}*/