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
		uint32 nRecv = neighborProcField_.waitBufferForUse();
		dataRecieved_ = true;
		if (nRecv != this->neighborProcSize())
		{
			fatalErrorInFunction<<
			"number of recived data is "<< nRecv <<" and expected number is "<<
			this->neighborProcSize()<< " in "<<this->name() <<endl;
			fatalExit;
		}

		//pOutput<<"field data "<< this->name()<<" has recieved with size "<< nRecv<<endl;
	}
}

template<class T, class MemorySpace>
bool
pFlow::MPI::processorBoundaryField<T, MemorySpace>::updateBoundary(
  int           step,
  DataDirection direction
)
{
#ifndef BoundaryModel1
	if(!this->boundary().performBoundaryUpdate())
		return true;
#endif

	if (step == 1)
	{
		// Isend
		if (direction == DataDirection::TwoWay || 
		( this->isBoundaryMaster() && direction == DataDirection::MasterToSlave) || 
		(!this->isBoundaryMaster() && direction == DataDirection::SlaveToMaster))
		{
			thisFieldInNeighbor_.sendData(pFlowProcessors(), this->thisField(), this->name());
			dataRecieved_ = false;
			//pOutput<<"request for boundary update "<< this->name()<<" direction "<< (int)direction<<endl; 
		}
		
	}
	else if (step == 2)
	{
		// Irecv
		if (direction == DataDirection::TwoWay || 
		(!this->isBoundaryMaster() && direction == DataDirection::MasterToSlave) || 
		( this->isBoundaryMaster() && direction == DataDirection::SlaveToMaster))
		{
			neighborProcField_.recieveData(pFlowProcessors(), this->neighborProcSize(), this->name());
			dataRecieved_ = false;
			//pOutput<<"request for boundary update "<< this->name()<<" direction "<< (int)direction<<endl;
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
    thisFieldInNeighbor_(
      groupNames("sendBuffer", this->name()),
      boundary.neighborProcessorNo(),
      boundary.thisBoundaryIndex()
    ),
    neighborProcField_(
      groupNames("recieveBuffer", boundary.name()),
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
	return neighborProcField_.buffer();
}

template<class T, class MemorySpace>
const typename pFlow::MPI::processorBoundaryField<T, MemorySpace>::
  ProcVectorType&
  pFlow::MPI::processorBoundaryField<T, MemorySpace>::neighborProcField() const
{
	checkDataRecieved();
	return neighborProcField_.buffer();
}

template<class T, class MemorySpace>
bool pFlow::MPI::processorBoundaryField<T, MemorySpace>::hearChanges(
	const timeInfo & ti,
	const message& msg, 
	const anyList& varList
)
{
	
	if(msg.equivalentTo(message::BNDR_PROC_SIZE_CHANGED))
	{
		auto newProcSize = varList.getObject<uint32>(
			message::eventName(message::BNDR_PROC_SIZE_CHANGED));
		neighborProcField_.resize(newProcSize);
	}
	else if(msg.equivalentTo(message::BNDR_PROCTRANSFER_SEND))
	{
		const auto& indices = varList.getObject<uint32Vector_D>(
			message::eventName(message::BNDR_PROCTRANSFER_SEND)
		);

        if constexpr( isDeviceAccessible<execution_space>())
        {
            FieldAccessType transferData(
			    indices.size(), 
			    indices.deviceViewAll(),
			    this->internal().deviceViewAll()
		    );

		    thisFieldInNeighbor_.sendData(pFlowProcessors(),transferData);
        }
        else
        {
            FieldAccessType transferData(
			    indices.size(), 
			    indices.hostViewAll(),
			    this->internal().deviceViewAll()
		    );
            
		    thisFieldInNeighbor_.sendData(pFlowProcessors(),transferData);
        }

	}
	else if(msg.equivalentTo(message::BNDR_PROCTRANSFER_RECIEVE))
	{
		uint32 numRecieved = varList.getObject<uint32>(
			message::eventName(message::BNDR_PROCTRANSFER_RECIEVE)
		);
		neighborProcField_.recieveData(pFlowProcessors(), numRecieved);
	}
	else if(msg.equivalentTo(message::BNDR_PROCTRANSFER_WAITFILL))
	{
		
		uint32 numRecieved = neighborProcField_.waitBufferForUse();
		if(numRecieved == 0u)
		{
			return true;
		}

		if(msg.equivalentTo(message::RANGE_CHANGED))
		{
			auto newRange = varList.getObject<rangeU32>(
				message::eventName(message::RANGE_CHANGED));
			this->internal().field().resize(newRange.end());
		}
		
		if(msg.equivalentTo(message::ITEMS_INSERT))
		{
			const auto& indices = varList.getObject<uint32IndexContainer>(
				message::eventName(message::ITEMS_INSERT));	
			
			this->internal().field().insertSetElement(
				indices, 
				neighborProcField_.buffer().deviceView());
		}
	}
	else
	{
		if(!BoundaryFieldType::hearChanges(ti, msg,varList) )
		{
			return false;
		}
	}

	return true;
	
}
template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::sendBackData() const
{
	neighborProcField_.sendBackData(pFlowProcessors());
	dataRecieved_ = false;
}

template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::recieveBackData() const
{
	thisFieldInNeighbor_.recieveBackData(pFlowProcessors(), this->size());
}

template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::addBufferToInternalField()const
{
	using RPolicy = Kokkos::RangePolicy<
		execution_space,
		Kokkos::Schedule<Kokkos::Static>,
		Kokkos::IndexType<pFlow::uint32>>;

	//pOutput<<"waiting for buffer to be recived in addBufferToInternalField "<<this->name()<<endl;
	thisFieldInNeighbor_.waitBufferForUse();

	const auto& buffView = thisFieldInNeighbor_.buffer().deviceViewAll();
	const auto& field = this->internal().deviceViewAll();

	if constexpr( isDeviceAccessible<execution_space> )
	{
		const auto& indices = this->indexList().deviceViewAll();
		Kokkos::parallel_for(
			"recieveBackData::"+this->name(),
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
			"recieveBackData::"+this->name(),
			RPolicy(0,this->size()),
			LAMBDA_HD(uint32 i)
			{
				field[indices[i]] += buffView[i]; 
			}
		);
		Kokkos::fence();
	}
}


template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::updateBoundaryToMaster()const
{
	if (!this->isBoundaryMaster() )
	{
		thisFieldInNeighbor_.sendData(pFlowProcessors(), this->thisField(), this->name());
		dataRecieved_ = false;
	}
}

template <class T, class MemorySpace>
void pFlow::MPI::processorBoundaryField<T, MemorySpace>::updateBoundaryFromSlave()const
{
	if( this->isBoundaryMaster() )
	{
		neighborProcField_.recieveData(pFlowProcessors(), this->neighborProcSize(), this->name());
		dataRecieved_ = false;
	}
}
