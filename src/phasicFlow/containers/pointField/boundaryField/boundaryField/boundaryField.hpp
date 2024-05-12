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
#ifndef __boundaryField_hpp__
#define __boundaryField_hpp__

#include "generalBoundary.hpp"
#include "internalField.hpp"

namespace pFlow
{


// forward 
template< class T, class MemorySpace>
	class boundaryFieldList;

enum class DataDirection
{
	MasterToSlave,
	SlaveToMaster,
	TwoWay
};

template< class T, class MemorySpace = void>
class boundaryField
:
    public generalBoundary 
{
public:
	
	using BoundaryFieldType = boundaryField<T, MemorySpace>;

	using InternalFieldType = internalField<T, MemorySpace>;

	using memory_space 		= typename InternalFieldType::memory_space;

	using execution_space 	= typename InternalFieldType::execution_space;

	using FieldAccessType 	= scatteredFieldAccess<T, memory_space>;

	using ProcVectorType 	= VectorSingle<T,MemorySpace>;

private:

	/// friend et al. 
	friend class boundaryFieldList<T,MemorySpace>;
	
    /// @brief a ref to the internal field 
    InternalFieldType& 			internal_;

	virtual 
	bool updateBoundary(int step, DataDirection direction)
	{
		return true;
	}

public:

	TypeInfoTemplate211("boundaryField","none" ,T, memory_space::name());

	boundaryField(
		const boundaryBase& boundary, 
		const pointStructure& pStruct,
		InternalFieldType& internal);
	
	create_vCtor
	(
		boundaryField,
		boundaryBase,
		(
			const boundaryBase& boundary, 
			const pointStructure& pStruct,
			InternalFieldType& internal
		),
		(boundary, pStruct, internal)
	);


	add_vCtor
	(
		BoundaryFieldType,
		BoundaryFieldType,
		boundaryBase
	);

	bool hearChanges
	(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
    	const anyList& varList
	) override
    {
		
		if(msg.equivalentTo(message::BNDR_REARRANGE))
		{
			// do nothing
		}
		if(msg.equivalentTo(message::BNDR_RESET))
		{
			//do nothing
		}
		return true;
	}

	inline
	InternalFieldType& internal()
	{
		return internal_;
	}

	inline
	const InternalFieldType& internal()const
	{
		return internal_;
	}

	FieldAccessType thisField()const
	{
		return FieldAccessType(
			this->size(),
			this->indexList().deviceViewAll(),
			internal_.deviceViewAll());
	}

	FieldAccessType mirrorField()const
	{
		return FieldAccessType(
			this->mirrorBoundary().size(),
			this->mirrorBoundary().indexList().deviceViewAll(),
			internal_.deviceViewAll());
	}
	
	virtual 
	ProcVectorType& neighborProcField();

	virtual
	const ProcVectorType& neighborProcField()const;

	virtual
	void fill(const T& val)
	{
		return;
	}

	static
	uniquePtr<boundaryField> create(
		const boundaryBase& boundary, 
		const pointStructure& pStruct,
		InternalFieldType& internal);

};

}

#include "boundaryField.cpp"

#endif

