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
#ifndef __processors_H__
#define __processors_H__ 



namespace pFlow
{
	// - Forward
	bool checkMPI
	(
		const char* funcName,
		int 				error, 
		bool 				forceAbort, 
		const char* fileName, 
		int 				lineNumebr
	);
}

// - Macro for automating line and file logs
#define CheckMPI(caller, fAbort)\
   pFlow::checkMPI(#caller, (caller), fAbort, __FILE__, __LINE__);


namespace pFlow
{

/**
 * This class holds the information about the global execution 
 * world and number of processors in communication.
 * 
 */
class processors
{
private:

	/// Is the static member initProcessors is called
	static inline 
	bool initProcessorsCelled_ = false;

	/// Global rank of the current processor
	static inline
	int globalRank_ = 0;

	/// The global size of all processors 
	static inline
	int globalSize_ = 1;

    static inline
    int argc_ = 0;

    static inline 
    char** argv_ = nullptr;

public:

	/// Initialize MPI processors
	static 
	void initProcessors(int argc, char *argv[]);

	/// Finalize MPI processors 
	static
	void finalizeProcessors();

	/// Constructor 
	processors();
	
	/// Destructor 
	~processors();
	
	/// Master processors number (globaly in MPI).
	static inline
	int globalMasterNo()
	{
		return 0;
	}

	/// Is this a parallel MPI run.
	static inline
	bool globalParallel()
	{
		return processors::globalSize()>1;
	}
	
	static inline
	const char* globalRunTypeName()
	{
		if(globalParallel())
		{
			return "MPI";
		}
		else
		{
			return "regular";
		}
	}
	/// Is MPI initialized?
	static
	bool isInitialized();
	
	/// Is MPI finalized?
	static
	bool isFinalized();
	
	/// Is this processor the master processor?
	static inline
	bool globalMaster()
	{
		return processors::globalRank() == processors::globalMasterNo();
	}
	
	/// Global size of processors
	static inline
	int globalSize()
	{
		return globalSize_;
	}
	
	/// Rank of the processor in the global MPI
	static inline
	int globalRank()
	{
		return globalRank_;
	}

    static inline
    int argc()
    {
        return argc_;
    }

    static inline 
    char** argv()
    {
        return argv_;
    }

	/// Abort MPI run or regular run
	static
	void abort(int error);

/*#ifdef pFlow_Build_MPI
    static inline 
    auto worldCommunicator()
    {
      return pFlow::MPI::CommWorld;  
    }
#endif*/

}; //processors


} // pFlow

#endif //__processors_H__