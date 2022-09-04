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

#ifndef __unsortedContactList_H__
#define __unsortedContactList_H__ 

namespace pFlow
{


template<typename valueType, typename executionSpace, typename idType>
class unsortedContactList
:
	public unsortedPairs<executionSpace, idType>
{
public:
	
	using ValueType 	= valueType;

	using UnsortedPairs	= unsortedPairs<executionSpace, idType>;

	using IdType 			= typename UnsortedPairs::IdType;

	using ExecutionSpace 	= typename UnsortedPairs::ExecutionSpace;

	using memory_space 		= typename ExecutionSpace::memory_space;

	using PairType 			= typename UnsortedPairs::PairType;

	using ContainerType 	= typename UnsortedPairs::ContainerType;

	class TagReFillPairs{};
	
protected:

	/// storage for keeping the values of the current list 
	ViewType1D<ValueType,ExecutionSpace> 	values_;

	/// storage for keeping pairs from the previous list  
	ContainerType 							container0_;

	/// storage for keeping values from the previous list 
	ViewType1D<ValueType,ExecutionSpace>    values0_;

	void adjustCapacity()
	{
		auto cap = this->capacity();
		if(cap > values_.size())
		{
			reallocNoInit(values_, cap);
		}

	}

	using rpFillPairs = Kokkos::RangePolicy<
		ExecutionSpace,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<int32>,
		TagReFillPairs>;


public:

	TypeNameNV("unsortedContactList");

	unsortedContactList(int32 capacity=1)
	:
		UnsortedPairs(capacity),
		values_("values", UnsortedPairs::capacity()),
		container0_(capacity),
		values0_("values0",container0_.capacity())
	{}


	bool beforeBroadSearch()
	{
		// swap conainer and values 
		swapViews(values0_, values_);
		swapViews(container0_, this->container_);
		return UnsortedPairs::beforeBroadSearch();
	}

	bool afterBroadSearch()
	{

		UnsortedPairs::afterBroadSearch();

		adjustCapacity();

		Kokkos::parallel_for(
			"reFillPairs",
			rpFillPairs(0,this->capacity()),
			*this);
		Kokkos::fence();

		return true; 
	}

	INLINE_FUNCTION_HD
	ValueType getValue(int32 idx)const
	{
		return values_[idx];
	}

	INLINE_FUNCTION_HD
	bool getValue(const PairType& p, ValueType& val)const
	{
		if(auto idx = this->find(p); idx>=0)
		{
			val = getValue(idx); 
			return true;
		}
		return false;
	}

	INLINE_FUNCTION_HD
	void setValue(int32 idx, const ValueType& val)const
	{
		values_[idx] = val;
	}

	INLINE_FUNCTION_HD
	bool setValue(const PairType& p, const ValueType& val)const
	{
		if(auto idx = this->find(p); idx>=0)
		{
			setValue(idx, val);
			return true;;
		}
		return false;
	}

	INLINE_FUNCTION_HD
	void operator()(TagReFillPairs, int32 idx)const
	{
		if( this->isValid(idx) )
		{
			if( int32 idx0 = 
					container0_.find(this->getPair(idx));
					idx0>=0 )
			{
				values_[idx] = values0_[idx0];
			}
			else
			{
				values_[idx] = ValueType();
			}
		}	
		// invalid locations should not be filled.	
	}

}; //unsortedContactList


} // pFlow


#endif //__unsortedContactList_H__
