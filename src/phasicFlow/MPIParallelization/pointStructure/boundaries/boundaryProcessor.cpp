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

#include "boundaryProcessor.hpp"
#include "boundaryProcessorKernels.hpp"
#include "dictionary.hpp"
#include "mpiCommunication.hpp"
#include "boundaryBaseKernels.hpp"
#include "internalPoints.hpp"
#include "Time.hpp"
#include "anyList.hpp"


void
pFlow::MPI::boundaryProcessor::checkDataRecieved() const
{
	if (!dataRecieved_)
	{
		uint32 nRecv = neighborProcPoints_.waitBufferForUse();
		dataRecieved_ = true;
		if (nRecv != neighborProcSize())
		{
			fatalErrorInFunction<<"In boundary "<<this->name()<<
			" ,number of recieved data is "<< nRecv<<
			" and neighborProcSize is "<<neighborProcSize()<<endl;
			fatalExit;
		}
	}
}

pFlow::MPI::boundaryProcessor::boundaryProcessor(
  const dictionary& dict,
  const plane&      bplane,
  internalPoints&   internal,
  boundaryList&     bndrs,
  uint32            thisIndex
)
  : boundaryBase(dict, bplane, internal, bndrs, thisIndex),
    thisPointsInNeighbor_(
      groupNames("sendBuffer", name()),
      neighborProcessorNo(),
      thisBoundaryIndex()
    ),
    neighborProcPoints_(
      groupNames("neighborProcPoints", name()),
      neighborProcessorNo(),
      mirrorBoundaryIndex()
    )
{
}

bool
pFlow::MPI::boundaryProcessor::beforeIteration(
	uint32 step, 
	const timeInfo& ti, 
	bool updateIter, 
	bool iterBeforeUpdate , 
	bool& callAgain
)
{
	if(step == 1)
	{
		boundaryBase::beforeIteration(step, ti, updateIter, iterBeforeUpdate, callAgain);
		callAgain = true;
	}
	else if(step == 2 )
	{

    #ifdef BoundaryModel1
        callAgain = true;
    #else
        if(!performBoundaryUpdate())
        {
            callAgain = false;
            return true;
        }
    #endif

	thisNumPoints_ = size();

	MPI_Isend(
		&thisNumPoints_,
		1,
		MPI_UNSIGNED,
		neighborProcessorNo(),
		thisBoundaryIndex(),
		pFlowProcessors().localCommunicator(),
		&numPointsRequest0_);

	MPI_Irecv(
		&neighborProcNumPoints_,
		1,
		MPI_UNSIGNED,
		neighborProcessorNo(),
		mirrorBoundaryIndex(),
		pFlowProcessors().localCommunicator(),
		&numPointsRequest_
	);

	}
	else if(step == 3 )
	{
		callAgain = true;

		if(numPointsRequest_ != RequestNull)
		{
			MPI_Wait(&numPointsRequest_, MPI_STATUS_IGNORE);
			if(numPointsRequest0_!= RequestNull)
			{
				MPI_Wait(&numPointsRequest0_, MPI_STATUS_IGNORE);
			}
		}
		
		// Size has not been changed. Notification is not required. 
		if(neighborProcNumPoints_ == neighborProcPoints_.size()) return true;

		anyList varList;
		message msg;

		varList.emplaceBack(msg.addAndName(message::BNDR_PROC_SIZE_CHANGED), neighborProcNumPoints_);

		if( !notify(ti, msg, varList) )
		{
			fatalErrorInFunction;
			callAgain = false;
			return false;
		}
		
	}
	else if(step == 4)
	{
		dataRecieved_ = false;
		if ( !isBoundaryMaster())
		{
			thisPointsInNeighbor_.sendData(pFlowProcessors(), thisPoints(),"positions");
		}
		else if (isBoundaryMaster())
		{
			neighborProcPoints_.recieveData(pFlowProcessors(), neighborProcSize(), "positions");
		}

		callAgain = false;
	}

	return true;
}

pFlow::uint32
pFlow::MPI::boundaryProcessor::neighborProcSize() const
{
	return neighborProcNumPoints_;
}

pFlow::realx3Vector_D&
pFlow::MPI::boundaryProcessor::neighborProcPoints()
{
	checkDataRecieved();
	return neighborProcPoints_.buffer();
}

const pFlow::realx3Vector_D&
pFlow::MPI::boundaryProcessor::neighborProcPoints() const
{
	checkDataRecieved();
	return neighborProcPoints_.buffer();
}

bool
pFlow::MPI::boundaryProcessor::updataBoundaryData(int step)
{
	return true;
}

bool pFlow::MPI::boundaryProcessor::transferData(
	uint32 iter, 
	int step,
	bool& callAgain
)
{
	
	if( !iterBeforeBoundaryUpdate() )
	{
		callAgain = false;
		return true;
	}

    if(step == 1)
	{
		
		uint32Vector_D transferFlags("transferFlags"+this->name());
		
		numToTransfer_ = markInNegativeSide(
			"transferData::markToTransfer"+this->name(),
			transferFlags);
			
		uint32Vector_D keepIndices("keepIndices");
		
		if(numToTransfer_ != 0u)
		{
			pFlow::boundaryBaseKernels::createRemoveKeepIndices
			(
				indexList(),
				numToTransfer_,
				transferFlags,
				transferIndices_,
				keepIndices,
				false
			);
			
			// delete transfer point from this processor 
			if( !setRemoveKeepIndices(transferIndices_, keepIndices))
			{
				fatalErrorInFunction<<
				"error in setting transfer and keep points in boundary "<< name()<<endl;
				return false;
			}
		}
		else
		{
			transferIndices_.clear();
		}
		
		CheckMPI( Isend(
			numToTransfer_, 
			neighborProcessorNo(), 
			thisBoundaryIndex(),
			pFlowProcessors().localCommunicator(),
			&numTransferRequest_), true );
        
		CheckMPI(Irecv(
			numToRecieve_,
			neighborProcessorNo(),
			mirrorBoundaryIndex(),
			pFlowProcessors().localCommunicator(),
			&numRecieveRequest_), true);
        
		callAgain = true;
		return true;
	}
	else if(step ==2) // to transferData to neighbor 
	{
		if(numTransferRequest_!= RequestNull)
		{
			Wait(&numTransferRequest_, StatusIgnore);
		}
		
		if( numToTransfer_ == 0u)
		{
			callAgain = true;
			return true;
		} 

		pointFieldAccessType transferPoints(
			transferIndices_.size(), 
			transferIndices_.deviceViewAll(),
			internal().pointPositionDevice()
		);

		// this buffer is used temporarily 
		thisPointsInNeighbor_.sendData(pFlowProcessors(), transferPoints);
		
		message msg;
		anyList varList;
		varList.emplaceBack( 
		msg.addAndName(message::BNDR_PROCTRANSFER_SEND),
		transferIndices_);

		const auto ti = internal().time().TimeInfo();
		
		if(!notify(ti, msg,	varList)
		)
		{
			fatalErrorInFunction;
			callAgain = false;
			return false;
		} 
		callAgain = true;
		return true;
	}
	else if(step == 3) // to recieve data 
	{

		if(numRecieveRequest_ != RequestNull)
		{
			Wait(&numRecieveRequest_, StatusIgnore);
		}
		
		if(numToRecieve_ == 0u)
		{
			callAgain = false;
			return true;
		} 
		
		// this buffer is being used temporarily 
		neighborProcPoints_.recieveData(pFlowProcessors(), numToRecieve_);
		
		message msg;
		anyList varList;
		varList.emplaceBack( 
		msg.addAndName(message::BNDR_PROCTRANSFER_RECIEVE),
		numToRecieve_);

		const auto ti = internal().time().TimeInfo();
		if(!notify( ti,	msg, varList))
		{
			fatalErrorInFunction;
			callAgain = false;
			return false;
		}

		callAgain = true;
		return true;
	}
	else if(step == 4) // to insert data 
	{
		if(numToRecieve_ == 0u)
		{
			callAgain = false;
			return true;
		}
		
		// points should be inserted first 
		message msg(message::BNDR_PROCTRANSFER_WAITFILL);
		anyList varList;

		neighborProcPoints_.waitBufferForUse();
		internal().insertPointsOnly(neighborProcPoints_.buffer(), msg, varList);
		
		const auto& indices = varList.getObject<uint32IndexContainer>(message::eventName(message::ITEMS_INSERT));
		
        // creates a view (does not copy data)
		auto indView = deviceViewType1D<uint32>(indices.deviceView().data(), indices.deviceView().size());
		
		uint32Vector_D newIndices("newIndices", indView);

		if(! appendNewIndices(newIndices))
		{
			fatalErrorInFunction;
			callAgain = false;
			return false;
		}

		const auto& ti = internal().time().TimeInfo();
		if(!notify(ti, msg, varList))
		{
			fatalErrorInFunction;
			callAgain = false;
			return false;
		}

		callAgain = false;
		return true;
	}
	return true;
}

bool
pFlow::MPI::boundaryProcessor::iterate(const timeInfo& ti)
{
	return true;
}

bool
pFlow::MPI::boundaryProcessor::afterIteration(const timeInfo& ti)
{
	
	uint32 s = size();
	pOutput<<"size of boundary is "<< s <<endl;
	uint32Vector_D transferFlags("transferFlags",s+1, s+1, RESERVE());
	transferFlags.fill(0u);

	const auto& transferD = transferFlags.deviceViewAll();
	auto points = thisPoints();
	auto p = boundaryPlane().infPlane();

	uint32 numTransfer = 0;	

	Kokkos::parallel_reduce
	(
		"boundaryProcessor::afterIteration",
		deviceRPolicyStatic(0,s),
		LAMBDA_HD(uint32 i, uint32& transferToUpdate)
		{
			if(p.pointInNegativeSide(points(i)))
			{
				transferD(i)=1;
				transferToUpdate++;
			}
		}, 
		numTransfer
	);
		
	pOutput<<"Numebr to be transfered "<< numTransfer<<endl;
		
	uint32Vector_D transferIndices("transferIndices");
	uint32Vector_D keepIndices("keepIndices");

	pFlow::boundaryBaseKernels::createRemoveKeepIndices
	(
		indexList(),
		numTransfer,
		transferFlags,
		transferIndices,
		keepIndices
	);
	
	// delete transfer point from this processor 
	if( !setRemoveKeepIndices(transferIndices, keepIndices))
	{
		fatalErrorInFunction<<
		"error in setting transfer and keep points in boundary "<< name()<<endl;
		return false;
	}
	
	return true;
}