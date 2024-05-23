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
		uint32 nRecv = reciever_.waitBufferForUse();
		dataRecieved_ = true;
		if (nRecv != neighborProcSize())
		{
			fatalErrorInFunction;
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
    sender_(
      groupNames("sendBuffer", name()),
      neighborProcessorNo(),
      thisBoundaryIndex()
    ),
    reciever_(
      groupNames("neighborProcPoints", name()),
      neighborProcessorNo(),
      mirrorBoundaryIndex()
    )
{
}

bool
pFlow::MPI::boundaryProcessor::beforeIteration(
	uint32 step, 
	uint32 iterNum, 
	real t, 
	real dt)
{
		if(step == 1 )
	{
		thisNumPoints_ = size();

		uint32 oldNeighborProcNumPoints = neighborProcNumPoints_;

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
	else if(step == 2 )
	{
		if(numPointsRequest_ != RequestNull)
		{
			MPI_Wait(&numPointsRequest_, MPI_STATUS_IGNORE);
			if(numPointsRequest0_!= RequestNull)
			{
				MPI_Request_free(&numPointsRequest0_);
			}
		}

		anyList varList;
		message msg;

		varList.emplaceBack(msg.addAndName(message::BNDR_PROC_SIZE_CHANGED), neighborProcNumPoints_);

		if( !notify(iterNum, t, dt, msg, varList) )
		{
			fatalErrorInFunction;
			return false;
		}
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
	return reciever_.buffer();
}

const pFlow::realx3Vector_D&
pFlow::MPI::boundaryProcessor::neighborProcPoints() const
{
	checkDataRecieved();
	return reciever_.buffer();
}

bool
pFlow::MPI::boundaryProcessor::updataBoundaryData(int step)
{
	if (step == 1)
	{
		sender_.sendData(pFlowProcessors(), thisPoints());
		dataRecieved_ = false;
	}
	else if (step == 2)
	{
		reciever_.recieveData(pFlowProcessors(), neighborProcSize());
		dataRecieved_ = false;
	}
	return true;
}

bool pFlow::MPI::boundaryProcessor::transferData(uint32 iter, int step)
{
	if(!boundaryListUpdate(iter))return false;
	
    if(step==1)
	{
		uint32 s = size();
		uint32Vector_D transferFlags("transferFlags",s+1, s+1, RESERVE());
		transferFlags.fill(0u);

		const auto& transferD = transferFlags.deviceViewAll();
		deviceScatteredFieldAccess<realx3> points = thisPoints();
		auto p = boundaryPlane().infPlane();

		numToTransfer_ = 0;	

		
        Kokkos::parallel_reduce
		(
			"boundaryProcessor::afterIteration",
			deviceRPolicyStatic(0,s),
			boundaryProcessorKernels::markNegative(
                boundaryPlane().infPlane(),
                transferFlags.deviceViewAll(),
                thisPoints()
            ),
			numToTransfer_
		);
			
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

		auto req = RequestNull;
		CheckMPI( Isend(
			numToTransfer_, 
			neighborProcessorNo(), 
			thisBoundaryIndex(),
			pFlowProcessors().localCommunicator(),
			&req), true );
        //pOutput<<"sent "<< numToTransfer_<<endl;
		CheckMPI(recv(
			numToRecieve_,
			neighborProcessorNo(),
			mirrorBoundaryIndex(),
			pFlowProcessors().localCommunicator(),
			StatusesIgnore), true);
        
        //pOutput<<"recieved "<<numToRecieve_<<endl;

		MPI_Request_free(&req);
		return true;
	}
	else if(step ==2 )
	{
		if( transferIndices_.empty() )return true; 

		pointFieldAccessType transferPoints(
		transferIndices_.size(), 
		transferIndices_.deviceViewAll(),
		internal().pointPositionDevice());

		sender_.sendData(pFlowProcessors(), transferPoints);
		message msg;
		anyList varList;
		varList.emplaceBack( 
		msg.addAndName(message::BNDR_PROCTRANSFER_SEND),
		transferIndices_);

		if(!notify(
		internal().time().currentIter(),
		internal().time().currentTime(),
		internal().time().dt(),
		msg,
		varList))
		{
			fatalErrorInFunction;
			return false;
		} 

		return true;
	}
	else if(step == 3)
	{
		if(numToRecieve_ == 0u) return false;
		reciever_.recieveData(pFlowProcessors(), numToRecieve_);
		
		message msg;
		anyList varList;
		varList.emplaceBack( 
		msg.addAndName(message::BNDR_PROCTRANSFER_RECIEVE),
		numToRecieve_);

		if(!notify(
		internal().time().currentIter(),
		internal().time().currentTime(),
		internal().time().dt(),
		msg,
		varList))
		{
			fatalErrorInFunction;
			return false;
		}

		return true;
	}
	else if(step == 4)
	{
		if(numToRecieve_ == 0u) return false;
		reciever_.waitBufferForUse();
		
		// points should be inserted first 
		message msg(message::BNDR_PROCTRANSFER_WAITFILL);
		anyList varList;

		internal().insertPointsOnly(reciever_.buffer(), msg, varList);
		const auto& indices = varList.getObject<uint32IndexContainer>(message::eventName(message::ITEM_INSERT));
		auto indView = deviceViewType1D<uint32>(indices.deviceView().data(), indices.deviceView().size());
		uint32Vector_D newIndices("newIndices", indView);

		if(! appendNewIndices(newIndices))
		{
			fatalErrorInFunction;
			return false;
		}

		if(!notify(
			internal().time().currentIter(),
			internal().time().currentTime(),
			internal().time().dt(),
			msg,
			varList))
		{
			fatalErrorInFunction;
			return false;
		}

		return false;
	}

	return false;
	
}

bool
pFlow::MPI::boundaryProcessor::iterate(uint32 iterNum, real t, real dt)
{
	return true;
}

bool
pFlow::MPI::boundaryProcessor::afterIteration(uint32 iterNum, real t, real dt)
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