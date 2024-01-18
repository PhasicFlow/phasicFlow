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
#ifndef __exitBoundaryField_hpp__
#define __exitBoundaryField_hpp__

#include "boundaryField.hpp"

namespace pFlow
{

template< template<class, class> class VectorField, class T, class MemorySpace = void>
class exitBoundaryField
:
    public boundaryField<VectorField, T, MemorySpace> 
{
public:
	
    using ExitBoundaryFieldType = exitBoundaryField<VectorField, T, MemorySpace>;

	using BoundaryFieldType = boundaryField<VectorField, T, MemorySpace>;

	using InternalFieldType = typename BoundaryFieldType::InternalFieldType;

	using memory_space 		= typename BoundaryFieldType::memory_space;

	using execution_space 	= typename BoundaryFieldType::execution_space;

   

public:

	TypeInfo("boundaryField<exit>");

	exitBoundaryField(
		const boundaryBase& boundary, 
		InternalFieldType& internal);
	
	

	add_vCtor
	(
		BoundaryFieldType,
		ExitBoundaryFieldType,
		boundaryBase
	);


	bool hearChanges
	(
		const message& msg, 
    	const anyList& varList
	) override
    {
		notImplementedFunction;
		return false;
	}

};

}

#include "exitBoundaryField.cpp"

#endif
