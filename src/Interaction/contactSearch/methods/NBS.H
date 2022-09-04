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


#ifndef __NBS_H__
#define __NBS_H__

#include "cells.H"
#include "contactSearchFunctions.H"

namespace pFlow
{


template<
	typename executionSpace,
	typename idType,
	typename indexType=int32
	>
class NBS
:
	public cells<indexType>
{
public:

	using IdType 		= idType;
	
	using IndexType 	= indexType;

	using Cells 		= cells<IndexType>;

	using CellType 		= typename Cells::CellType;

	using ExecutionSpace= executionSpace;

	using memory_space 	= typename ExecutionSpace::memory_space;

	struct TagBuild{};

	struct TagFindPairs{};

protected:

	int32 		capacity_		= 1;

	real 		sizeRatio_ 		= 1.0;

	int32 		updateFrequency_= 1;

	int32 		currentIter_ 	= 0;

	bool 		performedSearch_ = false;

	ViewType1D<realx3, memory_space> 		pointPosition_;

	ViewType1D<real, memory_space>			diameter_;

	ViewType3D<int32, memory_space> 		head_;

	ViewType1D<int32, memory_space> 		next_;


	INLINE_FUNCTION_H
	void nullify()
	{
		fill(
			head_,
			range(0,this->nx()),
			range(0,this->ny()),
			range(0,this->nz()),
			-1
			);

		fill(
			next_,
			range(0,capacity_),
			-1
			);
	}

	void nullify(range nextRng)
	{
		fill(
			head_,
			range(0,this->nx()),
			range(0,this->ny()),
			range(0,this->nz()),
			-1
			);

		fill(
			next_,
			nextRng,
			-1
			);
	}

	using mdrPolicyFindPairs = 
		Kokkos::MDRangePolicy<
			Kokkos::Rank<3>,
			Kokkos::Schedule<Kokkos::Dynamic>,
			ExecutionSpace>;

private:

	void checkAllocateNext(int newCap)
	{	
		
		if( capacity_ < newCap)
		{
			capacity_ = newCap;	
			Kokkos::realloc(next_, capacity_);
		}
		
	}

	void allocateHead()
	{
		Kokkos::realloc(head_, this->nx(), this->ny(), this->nz());
	}

	bool performSearch()
	{
		if(currentIter_ % updateFrequency_ == 0)
		{
			currentIter_++;
			return true;

		}else
		{
			currentIter_++;
			return false;
		}
	}

	static INLINE_FUNCTION_HD
	void Swap(int32& x, int32& y)
	{
		int32 tmp = x;
		x = y;
		y = tmp;
	}

public:

	TypeNameNV("NBS");

		NBS(
			const 	box& domain,
			real 	cellSize,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam,
			int32   initialContainerSize = 1)
		:
			Cells(domain, cellSize),
			pointPosition_(position),
			diameter_(diam),
			head_("NBS::head_",1,1,1), //, this->nx(), this->ny(), this->nz()),
			next_("NBS::next_",1) //,position.size()),
		{
			checkAllocateNext(pointPosition_.size());
			allocateHead();
		}

		NBS(
			dictionary dict,
			const 	box& domain,
			real 	cellSize,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam,
			int32   initialContainerSize = 1
			)
		:
			Cells(domain, cellSize),
			pointPosition_(position),
			diameter_(diam),
			head_("NBS::head_",1,1,1), //, this->nx(), this->ny(), this->nz()),
			next_("NBS::next_",1) //,position.size()),
		{
			
			updateFrequency_ = max( 
				dict.getVal<int32>("updateFrequency"),1 );

			sizeRatio_ = max(dict.getVal<real>(
				"sizeRatio"),1.0);

			this->setCellSize(cellSize*sizeRatio_);
			checkAllocateNext(pointPosition_.size());
			allocateHead();
		}
			
		INLINE_FUNCTION_HD
		NBS(const NBS&) = default;

		INLINE_FUNCTION_HD
		NBS& operator = (const NBS&) = default;

		INLINE_FUNCTION_HD
		~NBS()=default;

	//// - Methods 

	bool enterBoadSearch()const
	{
		return currentIter_%updateFrequency_==0;
	}

	bool performedSearch()const
	{
		return performedSearch_;
	}

	// - Perform the broad search to find pairs
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	//   on all the points in the range of [0,numPoints_)
	template<typename PairsContainer>
	bool broadSearch(PairsContainer& pairs, range activeRange, bool force=false)
	{
		
		if(force) currentIter_ = 0;
		performedSearch_ = false;

		if( !performSearch() ) return true;
		
		build(activeRange);
			
		findPairs(pairs);
		
		performedSearch_ = true;		
		return true;
	}

	// - Perform the broad search to find pairs,
	//   ignore particles with incld(i) = true,
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	template<typename PairsContainer, typename IncludeFunction>
	bool broadSearch(PairsContainer& pairs, range activeRange, IncludeFunction incld, bool force = false)
	{
		if(force) currentIter_ = 0;
		performedSearch_ = false;

		if( !performSearch() ) return true;

		build(activeRange, incld);
	
		findPairs(pairs);
		
		performedSearch_ = true;
		return true;
	}

	// - build based on all points in range [0, numPoints_)
	INLINE_FUNCTION_H
	void build(range activeRange)
	{
		checkAllocateNext(activeRange.second);
		nullify(activeRange);
		
		Cells cellIndex = static_cast<Cells>(*this);
		auto points = pointPosition_;
		auto next = next_;
		auto head = head_;

		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Static>,
			ExecutionSpace> rPolicy(activeRange.first, activeRange.second);
		
		Kokkos::parallel_for(
			"NBS::build",
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

		Kokkos::RangePolicy<
			Kokkos::IndexType<int32>,
			Kokkos::Schedule<Kokkos::Dynamic>,
			ExecutionSpace> rPolicy(activeRange.first, activeRange.second);

		Kokkos::parallel_for(
			"NBS::build",
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

	template<typename PairsContainer>	
	INLINE_FUNCTION_H
	bool findPairs(PairsContainer& pairs)
	{
		mdrPolicyFindPairs
			mdrPolicy(
				{0,0,0},
				{this->nx(),this->ny(),this->nz()} );

		int32 getFull = 1;
				
		// loop until the container size fits the numebr of contact pairs
		while (getFull > 0)
		{

			getFull = 0;
			
			Kokkos::parallel_reduce	(
				"NBS::broadSearch",
				mdrPolicy,
				CLASS_LAMBDA_HD(int32 i, int32 j, int32 k, int32& getFullUpdate){
					#include "NBSLoop.H"
				}, getFull);
						
			if(getFull)
			{
				// - resize the container
				//   note that getFull now shows the number of failed insertions.
				uint32 len = max(getFull,50) ;
				
				auto oldCap = pairs.capacity();
				
				pairs.increaseCapacityBy(len);

				Info<< "The contact pair container capacity increased from "<<
				oldCap << " to "<<pairs.capacity()<<" in NBS."<<endInfo;
				
			}

			Kokkos::fence();
		}
		
		return true;
	}

	bool objectSizeChanged(int32 newSize)
	{
		checkAllocateNext(newSize);
		return true;
	}
	
	/*INLINE_FUNCTION_HD
	void operator()(
		TagFindPairs,
		int32 i,
		int32 j,
		int32 k,
		int32& getFullUpdate)const 
	{

		int32 m = head_(i,j,k);
		CellType currentCell(i,j,k);
		int32 n = -1;

		while( m > -1 )
		{
			
			auto p_m = pointPosition_[m];
			auto d_m = sizeRatio_* diameter_[m];
			
			// the same cell
			n = next_(m);
			
			while(n >-1)
			{
				
				auto p_n = pointPosition_[n];
				auto d_n = sizeRatio_*diameter_[n];
				
				if( sphereSphereCheck(p_m, p_n, d_m, d_n) )
				{
					auto ln = n;
					auto lm = m;
					if(lm>ln) Swap(lm,ln);
					if( auto res = pairs_.insert(lm,ln); res <0)
					{
						getFullUpdate++;
					}
				}
				
				n = next_(n);
			}

			// neighbor cells
			CellType neighborCell;
			for(int32 ni=0; ni<13; ni++)
			{
				if(ni==0)		neighborCell = currentCell + CellType( 0, 0,-1);
				else if(ni==1)	neighborCell = currentCell + CellType(-1, 0,-1);
				else if(ni==2)	neighborCell = currentCell + CellType(-1, 0, 0); 
				else if(ni==3)	neighborCell = currentCell + CellType(-1, 0, 1); 
				else if(ni==4)	neighborCell = currentCell + CellType( 0,-1,-1); 
				else if(ni==5)	neighborCell = currentCell + CellType( 0,-1, 0); 
				else if(ni==6)	neighborCell = currentCell + CellType( 0,-1, 1); 
				else if(ni==7)	neighborCell = currentCell + CellType(-1,-1,-1); 
				else if(ni==8)	neighborCell = currentCell + CellType(-1,-1, 0); 
				else if(ni==9)	neighborCell = currentCell + CellType(-1,-1, 1);
				else if(ni==10)	neighborCell = currentCell + CellType( 1,-1,-1);  
				else if(ni==11)	neighborCell = currentCell + CellType( 1,-1, 0);  
				else if(ni==12)	neighborCell = currentCell + CellType( 1,-1, 1);  
				 
				if( this->isInRange(neighborCell) )
				{
					
					n = head_(neighborCell.x(), neighborCell.y(), neighborCell.z());
					while( n>-1)
					{
						
						auto p_n = pointPosition_[n];
						auto d_n = sizeRatio_*diameter_[n];
						
						if(sphereSphereCheck(p_m, p_n, d_m, d_n))
						{
							auto ln = n;
							auto lm = m;
							if(lm>ln) Swap(lm,ln);
							if( auto res = pairs_.insert(lm,ln); res <0)
							{
								getFullUpdate++;
							}
						}
						n = next_[n];
					}
				}

			}
			m = next_[m];
		}
	}*/

	template <typename PairsContainer, typename teamMemberType>
	INLINE_FUNCTION_HD
	int32 addPointsInBoxToList( 
		const teamMemberType& teamMember,
		IdType id,
		const iBox<IndexType>& triBox,
		const PairsContainer& pairs)const
	{
		int32 getFull = 0;

		auto bExtent = boxExtent(triBox);
		int32 numCellBox = bExtent.x()*bExtent.y()*bExtent.z();

		const auto head = head_;
		const auto next = next_;

		// perform a loop over all cells in the triBox
		Kokkos::parallel_reduce(
			Kokkos::TeamThreadRange(teamMember,numCellBox),
			[=](int32 linIndex, int32& valToUpdate){

				CellType cell;
				indexToCell(linIndex, triBox, cell);

				int32 n = head(cell.x(),cell.y(),cell.z());
				
				while( n>-1)
				{
					
					// id is wall id the pair is (particle id, wall id)
					if( pairs.insert(static_cast<IdType>(n), id) < 0 )
						valToUpdate++;
					n = next(n);
				}

			},
			getFull
			);

		return getFull;

	}

};

}

#endif
