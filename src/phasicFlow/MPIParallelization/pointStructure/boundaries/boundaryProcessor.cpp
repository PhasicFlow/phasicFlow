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
#include "dictionary.hpp"
#include "mpiCommunication.hpp"

void
pFlow::MPI::boundaryProcessor::checkSize() const
{
	
}

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
pFlow::MPI::boundaryProcessor::beforeIteration(uint32 iterNum, real t, real dt)
{
	thisNumPoints_ = size();

	auto req = MPI_REQUEST_NULL;
	MPI_Isend(
		&thisNumPoints_,
		1,
		MPI_UNSIGNED,
		neighborProcessorNo(),
		thisBoundaryIndex(),
		pFlowProcessors().localCommunicator(),
		&req);

	MPI_Recv(
		&neighborProcNumPoints_,
		1,
		MPI_UNSIGNED,
		neighborProcessorNo(),
		mirrorBoundaryIndex(),
		pFlowProcessors().localCommunicator(),
		MPI_STATUS_IGNORE
	);
	MPI_Request_free(&req);

	return true;
}

pFlow::uint32
pFlow::MPI::boundaryProcessor::neighborProcSize() const
{
	checkSize();
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
pFlow::MPI::boundaryProcessor::updataBoundary(int step)
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

bool
pFlow::MPI::boundaryProcessor::iterate(uint32 iterNum, real t, real dt)
{
	return true;
}

bool
pFlow::MPI::boundaryProcessor::afterIteration(uint32 iterNum, real t, real dt)
{
	return true;
}