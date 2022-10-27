#ifndef __NBSLevel_H__
#define __NBSLevel_H__


#include "cells.H"
#include "contactSearchFunctions.H"

template<typename executionSpace>
class
NBSLevel
{
public:


	using execution_space= executionSpace;

	using memory_space 	= typename execution_space::memory_space;

	using rangePolicy = Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Static>,
			execution_space>

protected:

	int32x3 								numCells_{1,1,1};

	ViewType3D<int32, memory_space> 		head_;

	int8 									level_ = 0;

	
public:
	
	NBSLevel()
	{}

	NBSLevel(int8 lvl, int32x3 numCells)
	:
		numCells_(gridExtent),
		head_("NBSLevel::head", numCells_.x(), numCells_.y(), numCells_.z()),
		level_(lvl)
	{}

	INLINE_FUNCION_HD
	auto& head(int32 i, int32 j, int32 k)
	{
		return head_(i,j,k);
	}

	INLINE_FUNCION_HD
	auto& head()
	{
		return head_;
	}

	INLINE_FUNCION_HD
	auto level()const
	{
		return level_;
	}

	INLINE_FUNCION_HD
	const auto& numCells()const
	{
		return gridExtent_;
	}
	 
	void nullify()
	{
		fill(
			head_,
			range(0,numCells_.x()),
			range(0,numCells_.y()),
			range(0,numCells_.z()),
			static_cast<int32>(-1)
			);
	}
};

INLINE_FUNCION_HD
int32x3 mapIndexLevels( int32x3 ind, int32 lowerLevel, int32 upperLevel)
{
	int32 a = pow(2, static_cast<int32>(upperLevel-lowerLevel));
	return ind/a;
}


}

#endif
