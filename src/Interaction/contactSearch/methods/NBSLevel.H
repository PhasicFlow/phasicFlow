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

	int32x3 								gridExtent_;

	ViewType3D<int32, memory_space> 		head_;

	int8 									level_ = 1;

	
public:
	
	NBSLevel(int8 lvl, int32x3 gridExtent)
	:
		gridExtent_(gridExtent),
		head_("NBSLevel::head", gridExtent.x(), gridExtent.y(), gridExtent.z()),
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
	const auto& gridExtent()const
	{
		return gridExtent_;
	}

};



#endif
