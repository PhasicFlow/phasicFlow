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


#ifndef __mapperNBS_H__
#define __mapperNBS_H__

#include "cells.H"
#include "contactSearchFunctions.H"
#include "baseAlgorithms.H"

namespace pFlow
{

template<typename executionSpace>
class mapperNBS
:
	public cells<int32>
{
public:

	using IdType 			= int32;
	
	using IndexType 	= int32;

	using Cells 			= cells<IndexType>;

	using CellType 		= typename Cells::CellType;

	using execution_space = executionSpace;

	using memory_space 		= typename execution_space::memory_space;

	using HeadType 		= ViewType3D<int32, memory_space>;

	using NextType 		= ViewType1D<int32, memory_space>;	

	class cellIterator
	{
	private:
		HeadType 		head_;

		NextType 		next_;

	public:

		cellIterator(ViewType3D<int32, memory_space> head, ViewType1D<int32, memory_space> next)
		:
			head_(head),
			next_(next)
		{}

		INLINE_FUNCTION_HD
		Cells cellsSize()const {
			return Cells(head_.extent(0), head_.extent(1), head_.extent(2));}

		INLINE_FUNCTION_HD
		int32 start(IndexType i, IndexType j, IndexType k)const {
			return head_(i,j,k); }

		INLINE_FUNCTION_HD
		int32 getNext(int32 n)const {
			if(n<0) return n;
			return next_(n); }
	};

protected:

	int32 									capacity_ = 1;

	ViewType3D<int32, memory_space> 		head_;

	ViewType1D<int32, memory_space> 		next_;

	bool 									nextOwner_ = true;

	// borrowed ownership
	ViewType1D<realx3, memory_space> 		pointPosition_;

	using rangePolicyType	=
		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Static>,
			execution_space>;
	
	INLINE_FUNCTION_H
	void nullifyHead()
	{
		fill(
			head_,
			range(0,this->nx()),
			range(0,this->ny()),
			range(0,this->nz()),
			static_cast<int32>(-1)
			);
	}

	void nullifyNext(range nextRng)
	{
		if(!nextOwner_)return;
		fill(
			next_,
			nextRng,
			static_cast<int32>(-1)
			);
	}

	void nullify()
	{
		nullifyHead();

		nullifyNext(range(0,capacity_));
	}

	void nullify(range nextRng)
	{
		nullifyHead();

		nullifyNext(nextRng);
	}
	

	void checkAllocateNext(int newCap)
	{
		if( capacity_ < newCap)
		{
			capacity_ = newCap;	
			if(!nextOwner_)return;
			reallocNoInit(next_, capacity_);
		}
	}

	void allocateHead()
	{
		reallocNoInit(head_, this->nx(), this->ny(), this->nz());
	}


	
public:

	TypeNameNV("mapperNBS");

		INLINE_FUNCTION_HD
		mapperNBS(){}

		mapperNBS(
			const 	box& domain,
			real 	cellSize,
			const 	ViewType1D<realx3, memory_space>& position,
			bool 	nextOwner = true)
		:
			Cells(domain, cellSize),
			pointPosition_(position),
			head_(
				"mapperNBS::head_",
				this->nx(),
				this->ny(),
				this->nz()
				), 
			next_("mapperNBS::next_",1), //,position.size()),
			nextOwner_(nextOwner)
		{
			checkAllocateNext(pointPosition_.size());
		}

		mapperNBS(
			const 	box& domain,
			int32 	nx,
			int32 	ny,
			int32 	nz,
			const 	ViewType1D<realx3, memory_space>& position,
			bool 	nextOwner = true)
		:
			Cells(domain, nx, ny, nz),
			pointPosition_(position),
			head_("mapperNBS::head_",nx,ny,nz), 
			next_("mapperNBS::next_",1),
			nextOwner_(nextOwner)
		{
			checkAllocateNext(pointPosition_.size());
		}

			
		INLINE_FUNCTION_HD
		mapperNBS(const mapperNBS&) = default;

		INLINE_FUNCTION_HD
		mapperNBS& operator = (const mapperNBS&) = default;

		INLINE_FUNCTION_HD
		~mapperNBS()=default;

	//// - Methods 
	INLINE_FUNCTION_HD
	auto capacity()const
	{
		return capacity_;
	}

	cellIterator getCellIterator()const
	{
		return cellIterator(head_, next_);
	}

	bool particlesCapcityChanged(int32 newCap)
	{
		checkAllocateNext(newCap);
		return true;
	}

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

	INLINE_FUNCTION_H
	void setNext(ViewType1D<int32, memory_space>& next)
	{
		if(!nextOwner_)
		{
			next_ = next;
			capacity_ = next.size();
		}
	}

	

	// - build based on all points in active range
	INLINE_FUNCTION_H
	void build(range activeRange)
	{
		checkAllocateNext(activeRange.second);
		nullify(activeRange);
		
		Cells cellIndex = static_cast<Cells>(*this);
		auto points = pointPosition_;
		auto next = next_;
		auto head = head_;

		rangePolicyType rPolicy(activeRange.first, activeRange.second);
		
		Kokkos::parallel_for(
			"mapperNBS::build",
			rPolicy,
			LAMBDA_HD(int32 i){
				CellType ind = cellIndex.pointIndex(points[i]);	
				int32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
				next[i] = old;
			});
		Kokkos::fence();
	}


	template<typename IncludeFunction>
	INLINE_FUNCTION_H
	void build(range activeRange, IncludeFunction incld)
	{
		checkAllocateNext(activeRange.second);
		nullify(activeRange);

		Cells cellIndex = static_cast<Cells>(*this);
		auto points = pointPosition_;
		auto next = next_;
		auto head = head_;

		rangePolicyType rPolicy(activeRange.first, activeRange.second);

		Kokkos::parallel_for(
			"mapperNBS::build_Include",
			rPolicy,
			LAMBDA_HD(int32 i){
				if( incld(i) )
				{
					CellType ind = cellIndex.pointIndex(points[i]);
					auto old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
					next[i] = old;
				}
			});
		Kokkos::fence();

	}

	
	INLINE_FUNCTION_H
	void buildCheckInDomain(range activeRange)
	{
		checkAllocateNext(activeRange.second);
		nullify(activeRange);
		
		Cells cellIndex = static_cast<Cells>(*this);
		auto points = pointPosition_;
		auto next = next_;
		auto head = head_;

		rangePolicyType rPolicy(activeRange.first, activeRange.second);
		
		Kokkos::parallel_for(
			"mapperNBS::buildCheckInDomain",
			rPolicy,
			LAMBDA_HD(int32 i){	
				CellType ind;
				if( cellIndex.pointIndexInDomain(points[i], ind) )
				{
					int32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
					next[i] = old;	
				}
			});

		Kokkos::fence();

	}

	template<typename IncludeFunction>
	INLINE_FUNCTION_H
	void buildCheckInDomain(range activeRange, IncludeFunction incld)
	{
		checkAllocateNext(activeRange.second);
		nullify(activeRange);

		Cells cellIndex = static_cast<Cells>(*this);
		auto points = pointPosition_;
		auto next = next_;
		auto head = head_;

		rangePolicyType rPolicy(activeRange.first, activeRange.second);

		Kokkos::parallel_for(
			"mapperNBS::buildCheckInDomain_Include",
			rPolicy,
			LAMBDA_HD(int32 i){
				CellType ind;
				if( incld(i) && cellIndex.pointIndexInDomain(points[i], ind) )
				{
					auto old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
					next[i] = old;
				}
			});
		Kokkos::fence();
	}

};

} // pFlow

#endif // __mapperNBS_H__