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
#ifndef __reflectiveBoundaryField_hpp__
#define __reflectiveBoundaryField_hpp__

#include "boundaryField.hpp"

namespace pFlow
{

template< class T, class MemorySpace = void>
class reflectiveBoundaryField
:
    public boundaryField<T, MemorySpace> 
{
public:
	
    using ReflectiveBoundaryFieldType = reflectiveBoundaryField<T, MemorySpace>;

	using BoundaryFieldType = boundaryField<T, MemorySpace>;

	using InternalFieldType = typename BoundaryFieldType::InternalFieldType;

	using memory_space 		= typename BoundaryFieldType::memory_space;

	using execution_space 	= typename BoundaryFieldType::execution_space;

   

public:

	TypeInfoTemplate211("boundaryField","reflective", T, memory_space::name());

	reflectiveBoundaryField(
		const boundaryBase& boundary, 
		const pointStructure& pStruct,
		InternalFieldType& internal);
		

	add_vCtor
	(
		BoundaryFieldType,
		ReflectiveBoundaryFieldType,
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
		BoundaryFieldType::hearChanges(t,dt,iter, msg,varList);
		
		if(msg.equivalentTo(message::BNDR_DELETE))
		{
			// do nothing;
		}
		
		return true;
	}

};

}

#include "reflectiveBoundaryField.cpp"

#endif
