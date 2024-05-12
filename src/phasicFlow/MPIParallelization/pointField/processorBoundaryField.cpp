#include "processorBoundaryField.hpp"
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

template<class T, class MemorySpace>
void
pFlow::MPI::processorBoundaryField<T, MemorySpace>::checkDataRecieved() const
{
	if (!dataRecieved_)
	{
		uint32 nRecv = reciever_.waitBufferForUse();
		dataRecieved_ = true;
		if (nRecv != this->neighborProcSize())
		{
			fatalErrorInFunction;
			fatalExit;
		}
	}
}

template<class T, class MemorySpace>
bool
pFlow::MPI::processorBoundaryField<T, MemorySpace>::updateBoundary(
  int           step,
  DataDirection direction
)
{
	if (step == 1)
	{
		// Isend
		if (direction == DataDirection::TwoWay || 
		( this->isBoundaryMaster() && direction == DataDirection::MasterToSlave) || 
		(!this->isBoundaryMaster() && direction == DataDirection::SlaveToMaster))
		{
			sender_.sendData(pFlowProcessors(), this->thisField());
			dataRecieved_ = false;
		}
	}
	else if (step == 2)
	{
		// Irecv
		if (direction == DataDirection::TwoWay || 
		(!this->isBoundaryMaster() && direction == DataDirection::MasterToSlave) || 
		( this->isBoundaryMaster() && direction == DataDirection::SlaveToMaster))
		{
			reciever_.recieveData(pFlowProcessors(), this->neighborProcSize());
			dataRecieved_ = false;
		}
	}
	else
	{
		fatalErrorInFunction << "Invalid step number " << step << endl;
		return false;
	}

	return true;
}

template<class T, class MemorySpace>
pFlow::MPI::processorBoundaryField<T, MemorySpace>::processorBoundaryField(
  const boundaryBase&   boundary,
  const pointStructure& pStruct,
  InternalFieldType&    internal
)
  : BoundaryFieldType(boundary, pStruct, internal),
    sender_(
      groupNames("sendBufferField", boundary.name()),
      boundary.neighborProcessorNo(),
      boundary.thisBoundaryIndex()
    ),
    reciever_(
      groupNames("neighborProcField", boundary.name()),
      boundary.neighborProcessorNo(),
      boundary.mirrorBoundaryIndex()
    )
{
	this->addEvent(message::BNDR_PROCTRANSFER_SEND).
	addEvent(message::BNDR_PROCTRANSFER_RECIEVE).
	addEvent(message::BNDR_PROCTRANSFER_WAITFILL).
	addEvent(message::BNDR_PROC_SIZE_CHANGED);
}

template<class T, class MemorySpace>
typename pFlow::MPI::processorBoundaryField<T, MemorySpace>::ProcVectorType&
pFlow::MPI::processorBoundaryField<T, MemorySpace>::neighborProcField()
{
	checkDataRecieved();
	return reciever_.buffer();
}

template<class T, class MemorySpace>
const typename pFlow::MPI::processorBoundaryField<T, MemorySpace>::
  ProcVectorType&
  pFlow::MPI::processorBoundaryField<T, MemorySpace>::neighborProcField() const
{
	checkDataRecieved();
	return reciever_.buffer();
}

template<class T, class MemorySpace>
bool pFlow::MPI::processorBoundaryField<T, MemorySpace>::hearChanges(
	real t,
	real dt,
	uint32 iter,
	const message& msg, 
	const anyList& varList
)
{
	BoundaryFieldType::hearChanges(t,dt,iter, msg,varList);
	if(msg.equivalentTo(message::BNDR_PROC_SIZE_CHANGED))
	{
		auto newProcSize = varList.getObject<uint32>("size");
		reciever_.resize(newProcSize);
	}

	if(msg.equivalentTo(message::BNDR_PROCTRANSFER_SEND))
	{
		const auto& indices = varList.getObject<uint32Vector_D>(
			message::eventName(message::BNDR_PROCTRANSFER_SEND)
		);

		FieldAccessType transferData(
			indices.size(), 
			indices.deviceViewAll(),
			this->internal().deviceViewAll()
		);
		sender_.sendData(pFlowProcessors(),transferData);
	}
	else if(msg.equivalentTo(message::BNDR_PROCTRANSFER_RECIEVE))
	{
		uint32 numRecieved = varList.getObject<uint32>(
			message::eventName(message::BNDR_PROCTRANSFER_RECIEVE)
		);
		reciever_.recieveData(pFlowProcessors(), numRecieved);
	}
	else if(msg.equivalentTo(message::BNDR_PROCTRANSFER_WAITFILL))
	{
		
		uint32 numRecieved = reciever_.waitBufferForUse();

		if(msg.equivalentTo(message::CAP_CHANGED))
		{
			auto newCap = varList.getObject<uint32>(
				message::eventName(message::CAP_CHANGED));
			this->internal().field().reserve(newCap);

		}
		if(msg.equivalentTo(message::SIZE_CHANGED))
		{
			auto newSize = varList.getObject<uint32>(
				message::eventName(message::SIZE_CHANGED));
			this->internal().field().resize(newSize);
		}
		
		const auto& indices = varList.getObject<uint32IndexContainer>(
			message::eventName(message::ITEM_INSERT));
		
		this->internal().field().insertSetElement(indices, reciever_.buffer().deviceView());

		return true;
	}

	return true;
}
template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::sendBackData() const
{
	reciever_.sendBackData(pFlowProcessors());
	dataRecieved_ = false;
}

template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::recieveBackData() const
{
	sender_.recieveBackData(pFlowProcessors(), this->size());
}

template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::addBufferToInternalField()const
{
	using RPolicy = Kokkos::RangePolicy<
		execution_space,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<pFlow::uint32>>;

	sender_.waitBufferForUse();

	const auto& buffView = sender_.buffer().deviceViewAll();
	const auto& field = this->internal().deviceViewAll();

	if constexpr( isDeviceAccessible<execution_space> )
	{
		const auto& indices = this->indexList().deviceViewAll();
		Kokkos::parallel_for(
			"dataSender::recieveBackData",
			RPolicy(0,this->size()),
			LAMBDA_HD(uint32 i)
			{
				field[indices[i]] += buffView[i]; 
			}
		);
		Kokkos::fence();
	}
	else
	{
		const auto& indices = this->boundary().indexListHost().deviceViewAll();
		Kokkos::parallel_for(
			"dataSender::recieveBackData",
			RPolicy(0,this->size()),
			LAMBDA_HD(uint32 i)
			{
				field[indices[i]] += buffView[i]; 
			}
		);
		Kokkos::fence();
	}
}