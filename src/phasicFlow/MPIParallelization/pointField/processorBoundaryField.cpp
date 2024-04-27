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
		//uint32 nRecv  = reciever_.waitComplete();
		dataRecieved_ = true;
		/*if (nRecv != this->neighborProcSize())
		{
			fatalErrorInFunction;
			fatalExit;
		}*/
	}
}

template<class T, class MemorySpace>
bool
pFlow::MPI::processorBoundaryField<T, MemorySpace>::updateBoundary(
  int           step,
  DataDirection direction
)
{
	/*if (step == 1)
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
	}*/

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