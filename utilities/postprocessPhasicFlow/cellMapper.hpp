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


#ifndef __cellMapper_hpp__
#define __cellMapper_hpp__

#include "phasicFlowKokkos.hpp"
#include "pointFlag.hpp"
#include "rectangleMesh.hpp"


namespace pFlow
{

class cellMapper
{
public:

	using HeadType 			= hostViewType3D<uint32>;

	using NextType 			= hostViewType1D<uint32>;	

	
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

		INLINE_FUNCTION_H
		int32x3 numCells()const {
			return int32x3(head_.extent(0), head_.extent(1), head_.extent(2));}

		INLINE_FUNCTION_H
		uint32 start(int32 i, int32 j, int32 k)const {
			return head_(i,j,k); }

		INLINE_FUNCTION_H
		uint32 getNext(uint32 n)const {
			if(n == NoPos ) return NoPos;
			return next_(n); }

		INLINE_FUNCTION_H
		uint32 next(uint32 n)const{
			return next_(n);}
	};

private:
	
	const rectangleMesh&  	domainCells_;

	HeadType 			head_{"NBS::head",1,1,1};

	NextType 			next_{"NBS::next", 1};

	uint32 				nextCapacity_ = 0;

	void allocateArrays(rangeU32 nextRng);

	void checkAllocateNext(rangeU32 nextRng);
	
	void nullifyHead();
	
	void nullifyNext(rangeU32 nextRng);
	
public:

	TypeInfoNV("cellMapper");

		
		cellMapper(
			const rectangleMesh& rectMesh,
			const hostViewType1D<realx3>& pointPos,
			const pFlagTypeHost& flags);
					
		INLINE_FUNCTION_HD
		cellMapper(const cellMapper&) = default;

		INLINE_FUNCTION_HD
		cellMapper(cellMapper&&) = default;
		
		INLINE_FUNCTION_HD
		cellMapper& operator = (const cellMapper&) = default;

		INLINE_FUNCTION_HD
		cellMapper& operator = (cellMapper&&) = default;

		INLINE_FUNCTION_HD
		~cellMapper()=default;

	//// - Methods

		auto getCellIterator()const
		{
			return CellIterator(head_, next_);
		}

		
		bool build(
			const hostViewType1D<realx3>& pointPos,
			const pFlagTypeHost& flags);
};

} // pFlow

#endif // __cellMapper_hpp__