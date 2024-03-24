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
	#include "mpiCommunication.hpp"
#endif

// from PhasicFlow
#include "error.hpp"
#include "processors.hpp"
#include "streams.hpp"

static int numVarsInitialized__ = 0;

#ifdef pFlow_Build_MPI

    pFlow::MPI::DataType pFlow::MPI::realx3Type__;
    pFlow::MPI::DataType pFlow::MPI::realx4Type__;
    pFlow::MPI::DataType pFlow::MPI::int32x3Type__;
    
#endif

void pFlow::processors::initProcessors(int argc, char *argv[])
{

#ifdef pFlow_Build_MPI
	if(!processors::isInitialized())
	{
		CheckMPI(MPI_Init(&argc, &argv), true);
		initProcessorsCelled_ = true;

        argc_ = argc;
        argv_ = argv;

		MPI_Comm_rank(MPI_COMM_WORLD, &processors::globalRank_);
        MPI_Comm_size(MPI_COMM_WORLD, &processors::globalSize_);
        
		
		if(processors::globalParallel())
		{
			pFlow::pOutput.activatePrefix();
			pFlow::pOutput.setPrefixNum(processors::globalRank_);	
		}
		
		pFlow::mOutput.setMasterSlave(processors::globalMaster());
		pFlow::errReport.setMasterSlave(processors::globalMaster());

        MPI_Type_contiguous(3, pFlow::MPI::Type<real>(), &pFlow::MPI::realx3Type__);
		MPI_Type_commit(&pFlow::MPI::realx3Type__);

        MPI_Type_contiguous(4, pFlow::MPI::Type<real>(), &pFlow::MPI::realx4Type__);
		MPI_Type_commit(&pFlow::MPI::realx3Type__);

		MPI_Type_contiguous(3, pFlow::MPI::Type<int32>(), &pFlow::MPI::int32x3Type__);
		MPI_Type_commit(&pFlow::MPI::int32x3Type__);

	}
#else

#endif

}

void pFlow::processors::finalizeProcessors()
{

#ifdef pFlow_Build_MPI
	if(initProcessorsCelled_ && !isFinalized())
	{
        MPI::TypeFree(&pFlow::MPI::realx3Type__);
        MPI::TypeFree(&pFlow::MPI::realx4Type__);
		MPI::TypeFree(&pFlow::MPI::int32x3Type__);
		CheckMPI(MPI_Finalize(), true);
	}
#else

#endif
}

pFlow::processors::processors()
{

#ifdef pFlow_Build_MPI	
	if(globalParallel() && !isInitialized())
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






