#ifndef __NBSLevels_hpp__
#define __NBSLevels_hpp__

#include "NBSLevel.hpp"
#include "NBSLevel0.hpp"
#include "KokkosTypes.hpp"

namespace pFlow
{

template<typename executionSpace>
class NBSLevels
{

public:

	using NBSLevelType 	= NBSLevel<executionSpace>;

	using cellIterator 	= typename NBSLevelType::cellIterator;

	using IdType 		= typename NBSLevelType::IdType;
	
	using IndexType 	= typename NBSLevelType::IndexType;

	using Cells 		= typename NBSLevelType::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space 	= typename NBSLevelType::execution_space;

	using memory_space 		= typename NBSLevelType::memory_space;

	using realRange 	= kPair<real,real>;

protected:
	
	real 		minSize_;

	real 		maxSize_;
	
	int32 		numLevels_=1;

	Vector<NBSLevelType> 					nbsLevels_;

	ViewType1D<realRange, memory_space> 	sizeRangeLevels_;

	ViewType1D<realRange, HostSpace> 		sizeRangeLevelsHost_;

	ViewType1D<real, memory_space> 			maxSizeLevels_;

	ViewType1D<real, HostSpace> 			maxSizeLevelsHost_;

	ViewType1D<int8, memory_space> 			particleLevel_;

	range 									activeRange_{0,0};

	using rangePolicyType	=
		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Static>,
			execution_space>;

	int32 setNumLevels()
	{

		int32 maxOvermin = static_cast<int32>(maxSize_/minSize_);

		if (maxOvermin <=1)
			return 1;
		else if(maxOvermin<=3)
			return 2;
		else if(maxOvermin<=7)
			return 3;
		else if(maxOvermin<15)
			return 4;
		else if(maxOvermin<31)
			return 5;
		else if(maxOvermin<63)
			return 6;
		else if(maxOvermin <127)
			return 7;
		else
		{
			fatalErrorInFunction<<
			"size ratio is not valid for multi-grid NBS "<< maxOvermin<<endl;
			fatalExit;
		}

		return -1;
	}

	bool setDiameterRange(real sizeRatio)
	{
		real lvl_maxD  = sizeRatio* maxSize_;
		real lvl_minD  = lvl_maxD/2.0;		

		for(int32 lvl=numLevels_-1; lvl>=0; lvl--)
		{

			if(lvl == 0 ) lvl_minD = 0.01*minSize_;
			
			sizeRangeLevelsHost_[lvl] = {lvl_minD, lvl_maxD};
			maxSizeLevelsHost_[lvl] = lvl_maxD;
			lvl_maxD = lvl_minD;
			lvl_minD /= 2.0;
		}
		
		copy(sizeRangeLevels_, sizeRangeLevelsHost_);
		copy(maxSizeLevels_, maxSizeLevelsHost_);

		REPORT(2)<<"Grids with "<< yellowText(numLevels_)<< " levels have been created."<<endREPORT;
		for(int32 lvl=0; lvl<numLevels_; lvl++)
		{
			REPORT(3)<<"Cell gird No "<< yellowText(lvl)<<" with size range ("
				<<sizeRangeLevelsHost_[lvl].first<<","<<sizeRangeLevelsHost_[lvl].second<<"]."<<endREPORT;
		}
		
		return true;
	}

	bool initLevels(
		const 	box& domain,
		real 	sizeRatio,
		const 	ViewType1D<realx3, memory_space>& position,
		const 	ViewType1D<real, memory_space>& diam)
	{
		

		for(int32 lvl = 0; lvl<numLevels_; lvl++)
		{
			nbsLevels_[lvl] = NBSLevelType(
				lvl, 
				domain, 
				maxSizeLevelsHost_[lvl]*sizeRatio,
				sizeRatio,
				position,
				diam );
		}

		auto next0 = nbsLevels_[0].next();
		for(int32 lvl=1; lvl<numLevels_; lvl++)
		{
			nbsLevels_[lvl].setNext(next0);
		}

		return true;
	}


	void manageAllocateNext(range active)
	{
		activeRange_ = active;

		if(activeRange_.second > nbsLevels_[0].capacity())
		{
			nbsLevels_[0].checkAllocateNext(activeRange_.second);

			auto next0 = nbsLevels_[0].next();

			for(int32 lvl=1; lvl<numLevels_; lvl++)
			{
				nbsLevels_[lvl].setNext(next0);
			}
		}
	}

	void nullify( range active)
	{
		for(int32 lvl=0; lvl<numLevels_; lvl++)
		{
			nbsLevels_[lvl].nullify(active);
		}
	}

public:

	NBSLevels(
		const 	box& domain,
		real 	minSize,
		real 	maxSize,
		real 	sizeRatio,
		const 	ViewType1D<realx3, memory_space>& position,
		const 	ViewType1D<real, memory_space>& diam)
	:
	minSize_(minSize),
	maxSize_(maxSize),
	numLevels_(setNumLevels()),
	nbsLevels_("nbsLevels", numLevels_, numLevels_, RESERVE()),
	sizeRangeLevels_("sizeRangeLevels", numLevels_),
	sizeRangeLevelsHost_("sizeRangeLevelsHost", numLevels_),
	maxSizeLevels_("maxSizeLevels", numLevels_),
	maxSizeLevelsHost_("maxSizeLevelsHost", numLevels_)
	{

		setDiameterRange(sizeRatio);

		initLevels(domain, sizeRatio, position, diam);
	}

	auto getCellIterator(int32 lvl)const
	{
		return nbsLevels_[lvl].getCellIterator();
	}

	int32 numLevels()const
	{
		return numLevels_;
	}

	Cells getCells(int32 lvl)const
	{
		return nbsLevels_[lvl].getCells();
	}

	template<typename PairsContainer>	
	INLINE_FUNCTION_H
	bool findPairs(PairsContainer& pairs)
	{
		
		int32 getFull = 1;
		
		// loop until the container size fits the numebr of contact pairs
		while (getFull > 0)
		{

			getFull = findPairsCount(pairs);		
		
			if(getFull)
			{
				// - resize the container
				//   note that getFull now shows the number of failed insertions.
				uint32 len = max(getFull,100) ;
				
				auto oldCap = pairs.capacity();
				
				pairs.increaseCapacityBy(len);

				INFORMATION<< "The contact pair container capacity increased from "<<
				oldCap << " to "<<pairs.capacity()<<" in NBSLevels."<<endINFO;
				
			}

			Kokkos::fence();
		}
		
		return true;
	}


	template<typename PairsContainer>	
	INLINE_FUNCTION_H
	int32 findPairsCount(PairsContainer& pairs)
	{

		int32 notInsertedCount = 0;

		for(int32 lvl=0; lvl<numLevels_; lvl++)
		{
			
			// the same level
			notInsertedCount+= nbsLevels_[lvl].findPairsCount(pairs);
			
			for(int32 crsLvl = lvl+1; crsLvl<numLevels_; crsLvl++)
			{
				// cross levels 
				notInsertedCount+= 
					nbsLevels_[lvl].findPairsCountCross(pairs, nbsLevels_[crsLvl]);
				
			}
		}
		
		return notInsertedCount;
	}

	INLINE_FUNCTION_H
	void build(range activeRange)
	{
				
		// nullify next and heads
		findParticleLevel(activeRange.first, activeRange.second);
		manageAllocateNext(activeRange);
		nullify(activeRange);
		
		//

		rangePolicyType rPolicy(activeRange.first, activeRange.second);
		
		auto nbsLevel0 = nbsLevels_[0];
		auto pointPosition = nbsLevel0.pointPosition();
		auto particleLevel = particleLevel_;
		auto next = nbsLevel0.next();
		auto head0 = nbsLevel0.head();
		
		typename NBSLevelType::HeadType head1, head2, head3, head4, head5, head6;

		if(numLevels_>1) head1 = nbsLevels_[1].head();
		if(numLevels_>2) head2 = nbsLevels_[2].head();
		if(numLevels_>3) head3 = nbsLevels_[3].head();
		if(numLevels_>4) head4 = nbsLevels_[4].head();
		if(numLevels_>5) head5 = nbsLevels_[5].head();
		if(numLevels_>6) head6 = nbsLevels_[6].head();


		
		Kokkos::parallel_for(
			"NBSLevels::build",
			rPolicy,
			LAMBDA_HD(int32 i){
				
				int8 lvl = particleLevel[i];
				auto ind = nbsLevel0.pointIndex(pointPosition[i]);
				ind = mapIndexLevels(ind, 0, lvl);
				int32 old;
				if(lvl==0)
					old =Kokkos::atomic_exchange(&head0(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==1)
					old =Kokkos::atomic_exchange(&head1(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==2)
					old =Kokkos::atomic_exchange(&head2(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==3)
					old =Kokkos::atomic_exchange(&head3(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==4)
					old =Kokkos::atomic_exchange(&head4(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==5)
					old =Kokkos::atomic_exchange(&head5(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==6)
					old =Kokkos::atomic_exchange(&head6(ind.x(), ind.y(), ind.z()),i);

				next(i) = old;
			});

		Kokkos::fence();
	}

	template<typename IncludeFunction>
	INLINE_FUNCTION_H
	void build(range activeRange, IncludeFunction incld)
	{
		// nullify next and heads
		findParticleLevel(activeRange.first, activeRange.second);
		manageAllocateNext(activeRange);
		nullify(activeRange);
		

		rangePolicyType rPolicy(activeRange.first, activeRange.second);

		auto nbsLevel0 = nbsLevels_[0];
		auto pointPosition = nbsLevel0.pointPosition();
		auto particleLevel = particleLevel_;
		auto next = nbsLevel0.next();
		auto head0 = nbsLevel0.head();

		typename NBSLevelType::HeadType head1, head2, head3, head4, head5, head6;

		if(numLevels_>1) head1 = nbsLevels_[1].head();
		if(numLevels_>2) head2 = nbsLevels_[2].head();
		if(numLevels_>3) head3 = nbsLevels_[3].head();
		if(numLevels_>4) head4 = nbsLevels_[4].head();
		if(numLevels_>5) head5 = nbsLevels_[5].head();
		if(numLevels_>6) head6 = nbsLevels_[6].head();

		Kokkos::parallel_for(
			"NBSLevels::build",
			rPolicy,
			LAMBDA_HD(int32 i){
				if(!incld(i)) return;
				
				int8 lvl = particleLevel[i];
				auto ind = nbsLevel0.pointIndex(pointPosition[i]);
				
				ind = mapIndexLevels(ind, 0, lvl);
				int32 old;
				if(lvl==0)
					old =Kokkos::atomic_exchange(&head0(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==1)
					old =Kokkos::atomic_exchange(&head1(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==2)
					old =Kokkos::atomic_exchange(&head2(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==3)
					old =Kokkos::atomic_exchange(&head3(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==4)
					old =Kokkos::atomic_exchange(&head4(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==5)
					old =Kokkos::atomic_exchange(&head5(ind.x(), ind.y(), ind.z()),i);
				else if(lvl==6)
					old =Kokkos::atomic_exchange(&head6(ind.x(), ind.y(), ind.z()),i);
				
				next(i) = old;
				
			});

		Kokkos::fence();

	}

	bool findParticleLevel(int32 first, int32 last)
	{

		if(last > particleLevel_.size())
		{
			reallocNoInit(particleLevel_,last);
		}

		auto diameter  = nbsLevels_[0].diameter();
		auto const maxSizes = maxSizeLevels_;
		auto particleLevel = particleLevel_;
		auto const sizeRatio = 0.999*nbsLevels_[0].sizeRatio();

		int8 maxLvl = sizeRangeLevels_.size();

		rangePolicyType rPolicy(first, last);

		Kokkos::parallel_for(
			"NBSLevels::findParticleLevel",
			rPolicy,
			LAMBDA_HD(int32 i)
			{
				for(int8 lvl = 0; lvl<maxLvl; lvl++)
				{
					if( sizeRatio*diameter[i]<= maxSizes[lvl] )
					{
						particleLevel[i] = lvl;
						return;
					}
				}
				particleLevel[i] = static_cast<int8>(-1);
			});
		Kokkos::fence();

		return true;
	}


}; //NBSLevels

}

#endif
