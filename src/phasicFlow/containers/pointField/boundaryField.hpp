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

#include "observer.hpp"
#include "boundaryBase.hpp"
#include "internalField.hpp"

namespace pFlow
{

template< class T, class MemorySpace = void>
class boundaryField
:
    public observer 
{
public:
	
	using BoundaryFieldType = boundaryField<T, MemorySpace>;

	using InternalFieldType = internalField<T, MemorySpace>;

	using memory_space 		= typename InternalFieldType::memory_space;

	using execution_space 	= typename InternalFieldType::execution_space;

protected:

    const boundaryBase&     boundary_;

    /// @brief a ref to the internal field 
    InternalFieldType& 			internal_;

	static inline
	const message defaultMessage_ = 
	(
		message::CAP_CHANGED+
		message::SIZE_CHANGED+
		message::ITEM_INSERT+
		message::ITEM_DELETE
	);

public:

	TypeInfo("boundaryField<none>");

	boundaryField(
		const boundaryBase& boundary, 
		InternalFieldType& internal);
	
	create_vCtor
	(
		boundaryField,
		boundaryBase,
		(
			const boundaryBase& boundary, 
			InternalFieldType& internal
		),
		(boundary, internal)
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
		notImplementedFunction;
		return false;
	}

	auto size()const
	{
		return boundary_.size();
	}

	auto capacity()const
	{
		return boundary_.capacity();
	}

	virtual
	void fill(const T& val)
	{
		return ;
	}

	static
	uniquePtr<boundaryField> create(
		const boundaryBase& boundary, 
		InternalFieldType& internal);

};

}

#include "boundaryField.cpp"

#endif

