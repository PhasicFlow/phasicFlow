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

#include "phasicFlowConfig.H"

#ifdef pFlow_Build_MPI
	#include <mpi.h>
#endif

// from PhasicFlow
#include "error.hpp"
#include "processors.hpp"
#include "streams.hpp"



void pFlow::processors::initProcessors(int argc, char *argv[])
{

#ifdef pFlow_Build_MPI
	if(!processors::isInitialized())
	{
		CheckMPI(MPI_Init(&argc, &argv), true);
		isSelfInitialized_ = true;

		processors::globalSize_ = MPI::COMM_WORLD.Get_size();
		processors::globalRank_ = MPI::COMM_WORLD.Get_rank();
		//CheckMPI(MPI_comm_size( MPI_COMM_WORLD, &processors::globalSize_), true );
		//CheckMPI(MPI_comm_rank( MPI_COMM_WORLD, &processors::globalRank_), true );
	}
#else

#endif

}

void pFlow::processors::finalizeProcessors()
{

#ifdef pFlow_Build_MPI
	if(isSelfInitialized_ && !isFinalized())
	{
		CheckMPI(MPI_Finalize(), true);
	}
#else

#endif
}

pFlow::processors::processors()
{

#ifdef pFlow_Build_MPI	
	if(isParallel() && !isInitialized())
	{
		fatalErrorInFunction<<
		"MPI communication is not initialized yet!"<<endl;
		processors::abort(MPI_ERR_OP);
	}
#endif
	
}

pFlow::processors::~processors()
{}


bool pFlow::processors::isInitialized()
{
	
#ifdef pFlow_Build_MPI
	int res;
	MPI_Initialized(&res);
	return res;
#endif
	return false;

}

bool pFlow::processors::isFinalized()
{

#ifdef pFlow_Build_MPI
	int res;
	MPI_Finalized(&res);
	return res;
#endif
	return false;

}

void pFlow::processors::abort(int error)
{

#ifdef pFlow_Build_MPI
	int flag=0;
	MPI_Initialized(&flag);
	if(flag == 1)
	{
		MPI_Abort(MPI_COMM_WORLD, error);
		MPI_Finalize();
  }
    exit(error);
#else
    exit(error);
#endif

}

bool pFlow::checkMPI(const char* funcName, int error, bool forceAbort, const char* fileName, int lineNumebr)
{

#ifdef pFlow_Build_MPI
	
	if(error == MPI_SUCCESS) return true;
	fatalErrorInMessage(funcName, fileName, lineNumebr);
	
	if(!forceAbort) return false;
	reportAndExit(error);
	
	return false;

#else

	return true;

#endif
}






