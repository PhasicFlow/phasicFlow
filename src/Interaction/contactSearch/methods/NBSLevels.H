#ifndef __NBSLevels_H__
#define __NBSLevels_H__



namespace pFlow
{

template<typename executionSpace>
class NBSLevels
:
	public NBSLevel0<executionSpace>
{

public:

	using NBSLevel0Type	= NBSLevel0<executionSpace>;

	using IdType 		= typename NBSLevel0Type::IdType;
	
	using IndexType 	= typename NBSLevel0Type::IndexType;

	using Cells 		= typename NBSLevel0Type::Cells;

	using CellType 		= typename Cells::CellType;

	using execution_space 	= typename NBSLevel0Type::execution_space;

	using memory_space 		= typename NBSLevel0Type::memory_space;

	using realRange 	= kPair<real,real>;

protected:

	
	real 		minSize_;

	real 		maxSize_;
	
	int32 								numLevles_=1;

	ViewType1D<NBSLevel, memory_space> 	nbsSuperLevels_;

	ViewType1D<realRange, memory_space> sizeRangeLevels_;

	ViewTypeID<int8, memory_space> 		particleLevel_;

	range 								activeRange_;

	using rangePolicyType	=
		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Static>,
			execution_space>;

	bool setNumLevels()
	{

		int32 maxOvermin = static_cast<int32>(maxSize_/minSize_);

		if (maxOvermin <=1)
			numLevles_ = 1;
		else if(maxOvermin<=3)
			numLevles_ = 2;
		else if(maxOvermin<=7)
			numLevles_ = 3;
		else if(maxOvermin<15)
			numLevles_ = 4;
		else if(maxOvermin<31)
			numLevles_ = 5;
		else if(maxOvermin<63)
			numLevles_ = 6;
		else if(maxOvermin <127)
			numLevles_ = 7;
		else
		{
			fatalErrorInFunction<<
			"size ratio is not valid for multi-grid NBS "<< maxOvermin<<endl;
			return false;
		}

		return true;
	}

	bool setDiameterRange()
	{
		auto sizeRatio = this->sizeRatio();
		real lvl_minD  = 0.1*minSize_;
		real lvl_maxD  = sizeRatio* minSize_;
		reallocNoInit(sizeRangeLevels_, numLevles_);

		for(int32 i=0; i<numLevles_; i++)
		{
			sizeRangeLevels_[i] = {lvl_minD, lvl_maxD};
			lvl_minD = lvl_maxD;
			lvl_maxD *= 2.0;
		}

		return true;
	}

	bool initSuperLevels()
	{
		if(numLevles_== 1) return true;
		reallocNoInit(nbsSuperLevels_, numLevles_);

		if(int lvl = 1; lvl<numLevles_-1; lvl++)
		{
			Cells lvlCells(this->domain(), sizeRangeLevels_[lvl].second);
			nbsSuperLevels_[lvl] = NBSLevle(lvl, lvlCells.numCells());
		}

		return true;
	}

	INLINE_FUNCTION_H
	void nullify()
	{
		// level 0
		NBSLevel0Type::nullify(activeRange_);
		
		// super levels
		for(int32 lvl=1; lvl<numLevles_; lvl++)
		{
			nbsSuperLevels_[lvl].nullify();
		}
	}

public:

	NBSLevels(
		const 	box& domain,
		real 	minSize,
		real 	maxSize,
		real 	sizeRatio,
		const 	ViewType1D<realx3, memory_space>& position,
		const 	ViewType1D<real, memory_space>& diam),
		range 	active
	:
	NBSLevel0Type(
		domain, 
		sizeRatio*minSize,
		position,
		diam),
	minSize_(minSize),
	maxSize_(maxSize),
	activeRange_(active)
	{
		if(!setNumLevels())
		{
			fatalExit;
		}

		setDiameterRange();

		initSuperLevels();

		findParticleLevel(activeRange_.first, activeRange_.last);
	}


	INLINE_FUNCTION_H
	void build(range activeRange)
	{
				
		// check for active ragne************************************************************************
		checkAllocateNext(activeRange.second);
		nullify();
		//

		rangePolicyType rPolicy(activeRange.first, activeRange.second);
		
		Kokkos::parallel_for(
			"NBSLevels::build",
			rPolicy,
			CLASS_LAMBDA_HD(int32 i){
				CellType ind = this->pointIndex(points[i]);	
				int8 lvl = particleLevel_[i];
				int32 old;
				if( lvl == 0)
				{
					int32 old = 
						Kokkos::atomic_exchange(
							&this->head_(ind.x(), ind.y(), ind.z()), 
							i);
					this->next_[i] = old;
				}
				else
				{
					ind = upper.mapIndexUpperLevel(ind, 0, lvl);      
					int32 old = 
						Kokkos::atomic_exchange(
							&nbsSuperLevels_[lvl].head()(ind.x(), ind.y(), ind.z()),
							i);
					this->next_[i] = old;
				}
			});

		Kokkos::fence();
	}

	template<typename IncludeFunction>
	INLINE_FUNCTION_H
	void build(range activeRange, IncludeFunction incld)
	{
		// check for active ragne************************************************************************
		checkAllocateNext(activeRange.second);
		nullify();
		//

		rangePolicyType rPolicy(activeRange.first, activeRange.second);

		Kokkos::parallel_for(
			"NBSLevels::build",
			rPolicy,
			CLASS_LAMBDA_HD(int32 i){
				if(incld(i))
				{
					CellType ind = this->pointIndex(points[i]);	
					int8 lvl = particleLevel_[i];
					int32 old;
					if( lvl == 0)
					{
						int32 old = 
						Kokkos::atomic_exchange(
							&this->head_(ind.x(), ind.y(), ind.z()),
							i);
						this->next_[i] = old;
					}
					else
					{
						ind = upper.mapIndexUpperLevel(ind, 0, lvl);      
						int32 old = 
							Kokkos::atomic_exchange(
								&nbsSuperLevels_[lvl].head()(ind.x(), ind.y(), ind.z()),
								i);
						this->next_[i] = old;
					}
				}
			});

		Kokkos::fence();

	}


	bool findParticleLevel(int32 first, int32 last)
	{
		auto diameter  = this->diameter_;
		auto sizeRange = sizeRangeLevels_;
		auto particleLevel = particleLevel_;

		int8 maxLvl = sizeRangeLevels_.size();

		rangePolicyType rPolicy(first, last);
		
		Kokkos::parallel_for(
			"NBSLevels::findParticleLevel",
			rPolicy,
			LAMBDA_HD(int32 i)
			{
				for(int8 lvl = 0; lvl<maxLvl; lvl++)
				{
					if( diameter[i]<= sizeRange[lvl].second )
					{
						particleLevel[i] = lvl;
						break;
					}
				}
			});

		return true;
	}


}; //NBSLevels

}

#endif
