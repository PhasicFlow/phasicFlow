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


#ifndef __mapperNBS_hpp__
#define __mapperNBS_hpp__

#include "phasicFlowKokkos.hpp"
#include "pointFlag.hpp"
#include "cells.hpp"
//#include "contactSearchFunctions.hpp"


namespace pFlow
{

class mapperNBS
{
public:

	using HeadType 			= deviceViewType3D<uint32>;

	using NextType 			= deviceViewType1D<uint32>;	

	
	static constexpr uint32 NoPos 	= 0xFFFFFFFF;

	class CellIterator
	{
	private:
		HeadType 		head_;

		NextType 		next_;

	public:

		CellIterator(const HeadType& head, const NextType& next)
		:
			head_(head),
			next_(next)
		{}
		
		static constexpr uint32 NoPos = 0xFFFFFFFF;

		INLINE_FUNCTION_HD
		int32x3 numCells()const {
			return int32x3(head_.extent(0), head_.extent(1), head_.extent(2));}

		INLINE_FUNCTION_HD
		uint32 start(int32 i, int32 j, int32 k)const {
			return head_(i,j,k); }

		INLINE_FUNCTION_HD
		uint32 getNext(uint32 n)const {
			if(n == NoPos ) return NoPos;
			return next_(n); }

		INLINE_FUNCTION_HD
		uint32 next(uint32 n)const{
			return next_(n);}
	};

private:
	
	cells  				domainCells_;

	cells 				searchCells_;

	HeadType 			head_{"NBS::head",1,1,1};

	NextType 			next_{"NBS::next", 1};

	uint32 				nextCapacity_ = 0;

	uint32 				lastCheckForBox_ = 0;

	uint32 				buildCount_ = 0;

	bool 				adjustableBox_ = false;

	bool 				nextOwner_ 	= true;

	static uint32 		checkInterval_;	

	static real 		enlargementFactor_;

	bool setSearchBox(
		const deviceViewType1D<realx3>& pointPos,
		const pFlagTypeDevice& flags,
		real cellSize
	);

	void allocateArrays(rangeU32 nextRng);

	void checkAllocateNext(rangeU32 nextRng);
	
	void nullifyHead();
	
	void nullifyNext(rangeU32 nextRng);
	
public:

	TypeInfoNV("mapperNBS");

		INLINE_FUNCTION_HD
		mapperNBS() = default;

		mapperNBS(
			const 	box& domain,
			real 	cellSize,
			const deviceViewType1D<realx3>& pointPos,
			const pFlagTypeDevice& flags,
			bool 	adjustableBox,
			bool 	nextOwner = true);
					
		INLINE_FUNCTION_HD
		mapperNBS(const mapperNBS&) = default;

		INLINE_FUNCTION_HD
		mapperNBS(mapperNBS&&) = default;
		
		INLINE_FUNCTION_HD
		mapperNBS& operator = (const mapperNBS&) = default;

		INLINE_FUNCTION_HD
		mapperNBS& operator = (mapperNBS&&) = default;

		INLINE_FUNCTION_HD
		~mapperNBS()=default;

	//// - Methods

		auto getCellIterator()const
		{
			return CellIterator(head_, next_);
		}

		const auto& getDomainCells()const
		{
			return domainCells_;
		}

		const auto& getSearchCells()const
		{
			return searchCells_;
		}

		bool build(
			const deviceViewType1D<realx3>& pointPos,
			const pFlagTypeDevice& flags,
			bool&  searchBoxChanged);

		
	

};

} // pFlow

#endif // __mapperNBS_hpp__




/*
INLINE_FUNCTION_HD
	auto& head()
	{
		return head_;
	}

	INLINE_FUNCTION_HD
	auto& next()
	{
		return next_;
	}

	INLINE_FUNCTION_HD
	const auto& head()const
	{
		return head_;
	}

	INLINE_FUNCTION_HD
	const auto& next()const
	{
		return next_;
	}

	INLINE_FUNCTION_HD
	auto& pointPosition()
	{
		return pointPosition_;
	}
*/