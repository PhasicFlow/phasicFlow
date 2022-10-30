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


#ifndef __NBSLevel0_H__
#define __NBSLevel0_H__

#include "mapperNBS.H"

namespace pFlow
{


template<typename executionSpace>
class NBSLevel0
:
	public mapperNBS<executionSpace>
{
public:

	using MapperType 		= mapperNBS<executionSpace>;

	using cellIterator  	= typename MapperType::cellIterator;

	using IdType 			= typename MapperType::IdType;
	
	using IndexType 		= typename MapperType::IndexType;

	using Cells 			= typename MapperType::Cells;

	using CellType 			= typename Cells::CellType;

	using execution_space 	= typename MapperType::execution_space;

	using memory_space 		= typename MapperType::memory_space;

	using HeadType 			= typename MapperType::HeadType;

	using NextType 			= typename MapperType::NextType;	

	struct TagFindPairs{};


protected:

	real 			sizeRatio_ 		= 1.0;

	// borrowed ownership
	ViewType1D<real, memory_space>			diameter_;


	using mdrPolicyFindPairs = 
		Kokkos::MDRangePolicy<
			Kokkos::Rank<3>,
			Kokkos::Schedule<Kokkos::Dynamic>,
			execution_space>;

	static INLINE_FUNCTION_HD
	void Swap(int32& x, int32& y)
	{
		int32 tmp = x;
		x = y;
		y = tmp;
	}

public:

	TypeNameNV("NBSLevel0");

		INLINE_FUNCTION_HD
		NBSLevel0(){}

		NBSLevel0(
			const 	box& domain,
			real 		cellSize,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam)
		:
			MapperType(domain, cellSize, position),
			diameter_(diam)
		{}

		NBSLevel0(
			const 	box& domain,
			int32 	nx,
			int32 	ny,
			int32 	nz,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam)
		:
			MapperType(domain, nx, ny, nz, position),
			diameter_(diam)
		{	}

		NBSLevel0(
			const 	box& domain,
			real 		cellSize,
			real 		sizeRatio,
			const 	ViewType1D<realx3, memory_space>& position,
			const 	ViewType1D<real, memory_space>& diam,
			bool 		nextOwner = true)
		:
			MapperType(domain, cellSize, position, nextOwner),
			sizeRatio_(sizeRatio),
			diameter_(diam)
		{}

		INLINE_FUNCTION_HD
		NBSLevel0(const NBSLevel0&) = default;

		INLINE_FUNCTION_HD
		NBSLevel0& operator = (const NBSLevel0&) = default;

		INLINE_FUNCTION_HD
		~NBSLevel0()=default;


	INLINE_FUNCTION_HD
	auto sizeRatio()const
	{
		return sizeRatio_;
	}

	INLINE_FUNCTION_HD
	auto& diameter()
	{
		return diameter_;
	}

	// - Perform the broad search to find pairs
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	//   on all the points in the range of [0,numPoints_)
	template<typename PairsContainer>
	bool broadSearch(PairsContainer& pairs, range activeRange)
	{
		
		
		this->build(activeRange);
		
		findPairs(pairs);
			
		return true;
	}

	// - Perform the broad search to find pairs,
	//   ignore particles with incld(i) = true,
	//	 with force = true, perform broad search regardless of 
	//   updateFrequency_ value
	template<typename PairsContainer, typename IncludeFunction>
	bool broadSearch(PairsContainer& pairs, range activeRange, IncludeFunction incld)
	{
		
		this->build(activeRange, incld);
	
		findPairs(pairs);
		
		return true;
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
				uint32 len = max(getFull,500) ;
				
				auto oldCap = pairs.capacity();
				
				pairs.increaseCapacityBy(len);

				Info<< "The contact pair container capacity increased from "<<
				oldCap << " to "<<pairs.capacity()<<" in NBSLevel0."<<endInfo;
				
			}

			Kokkos::fence();
		}
		
		return true;
	}

	template<typename PairsContainer>	
	INLINE_FUNCTION_H
	int32 findPairsCount(PairsContainer& pairs)
	{
		mdrPolicyFindPairs
			mdrPolicy(
				{0,0,0},
				{this->nx(),this->ny(),this->nz()} );

		int32 notInsertedPairs;
			
		Kokkos::parallel_reduce	(
			"NBSLevel0::findPairs",
			mdrPolicy,
			CLASS_LAMBDA_HD(int32 i, int32 j, int32 k, int32& getFullUpdate){
				#include "NBSLoop.H"
			}, notInsertedPairs);
			
		return notInsertedPairs;
			
	}

};

} // pFlow

#endif // __NBSLevel0_H__
