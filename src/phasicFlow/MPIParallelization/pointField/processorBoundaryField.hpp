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
#ifndef __processorBoundaryField_hpp__
#define __processorBoundaryField_hpp__

#include "boundaryField.hpp"
#include "dataSender.hpp"
#include "dataReciever.hpp"

namespace pFlow::MPI
{

template< class T, class MemorySpace = void>
class processorBoundaryField
:
    public boundaryField<T, MemorySpace> 
{
public:
	
    using processorBoundaryFieldType = processorBoundaryField<T, MemorySpace>;

	using BoundaryFieldType = boundaryField<T, MemorySpace>;

	using InternalFieldType = typename BoundaryFieldType::InternalFieldType;

	using memory_space 		= typename BoundaryFieldType::memory_space;

	using execution_space 	= typename BoundaryFieldType::execution_space;

	using FieldAccessType 	= typename BoundaryFieldType::FieldAccessType;

	using ProcVectorType  = typename BoundaryFieldType::ProcVectorType;

private:

	dataSender<T, MemorySpace>   sender_;

	dataReciever<T, MemorySpace> reciever_;

	mutable bool                 dataRecieved_ = true;

	void checkDataRecieved()const;

	bool updateBoundary(int step, DataDirection direction);
	

public:

	TypeInfoTemplate211("boundaryField","processor", T, memory_space::name());

	processorBoundaryField(
		const boundaryBase& boundary, 
		const pointStructure& pStruct,
		InternalFieldType& internal);

	
	~processorBoundaryField()override = default;

	add_vCtor
	(
		BoundaryFieldType,
		processorBoundaryFieldType,
		boundaryBase
	);
	
	ProcVectorType& neighborProcField() override;

	const ProcVectorType& neighborProcField()const override;
	
	void fill(const T& val)override
	{
		reciever_.fill(val);
	}

	bool hearChanges(
		real t,
		real dt,
		uint32 iter,
		const message& msg, 
    	const anyList& varList
	) override;
    
	void sendBackData()const;

	void recieveBackData()const;

	void addBufferToInternalField()const;

};

}

#include "processorBoundaryField.cpp"

#endif //__processorBoundaryField_hpp__
