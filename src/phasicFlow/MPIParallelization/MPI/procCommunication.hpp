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
#ifndef __procCommunication_hpp__
#define __procCommunication_hpp__


#include "procVector.hpp"
#include "localProcessors.hpp"
#include "mpiCommunication.hpp"

namespace pFlow::MPI
{


class procCommunication
{
protected:

	const localProcessors& processors_;

public:	

    procCommunication(const localProcessors& proc);
    	
	~procCommunication()=default;
    
    /// @brief Tell if this processor is master processor in the local
    /// communicator 
    /// @return true if this processor is master  

    inline 
    const auto& processors()const
    {
        return processors_;
    }

    inline
    bool localMaster()const
    {
        return processors_.localMaster();;
    }

    inline
    auto localSize()const
    {
        return processors_.localSize();
    }

    inline
    auto localRank()const
    {
        return processors_.localRank();
    }

    inline
    auto localCommunicator()const
    {
        return processors_.localCommunicator();
    }

    /// @brief return the master number in the local communicator  
    auto localMasterNo()const
    {
        return processors_.localMasterNo();
    }

	/// Send a single val to all processors including itself (local communicator)
	template<typename T>
	std::pair<T,bool> distributeMasterToAll(const T& val)
	{
		
		T retVal = val;
		auto res = CheckMPI(
            Bcast(retVal, localMasterNo(),localCommunicator() ),
            false);
		
		return {retVal, res};
	}

	/// @brief  Send a single value to all processor including master (in local communicator)
	/// @param val value to be sent
	/// @param recvVal recieved value 
	/// @return true if successful and false if fail
	template<typename T>
	bool distributeMasterToAll(const T& val, T& recvVal)
	{
		recvVal = val;
		return CheckMPI(
            Bcast(recvVal, localMasterNo(), localCommunicator()),
            false);
	}

	/// @brief  values in the vector (size is equal to number of 
    // processors in local communicator) to each processor 
	template<typename T>
	std::pair<T,bool> distributeMasterToAll(const procVector<T>& vals)
	{
		T val;	
		auto vec = vals.getSpan();
		auto res = CheckMPI(
            scatter(vec, val, localMasterNo(), localCommunicator()),
            false);
		
		return {val, res};
	}

    /// @brief Each processor in the local communicator calls this funtion with a value 
    /// and the values are distributed among all processors 
	template<typename T>
	std::pair<procVector<T>, bool> collectAllToAll(const T& val)
	{
		procVector<T> allVec(processors_);
		auto vec = allVec.getSpan();
		auto res = CheckMPI(
            allGather(val, vec, localCommunicator()), 
            false);
		return {allVec, res};
	}

    /// @brief Each processor in the local communicator calls this funtion with a value 
    /// and the values are distributed among all processors 
	template<typename T>
	bool collectAllToAll(const T& val, procVector<T>& allVec)
	{
		auto vec = allVec.getSpan();
		return CheckMPI(
            allGather(val, vec, localCommunicator()), 
            false);
	}

    /// @brief Each processor in the local communicator calls this function with a value
    /// and all values are collected in the master processor 
	template<typename T>
	std::pair<procVector<T>,bool> collectAllToMaster(const T& val)
	{
		// only on master processor
		procVector<T> masterVec(processors_, true);
		
		auto masterSpan = masterVec.getSpan();
		auto res = CheckMPI( 
			gather(val,masterSpan, localMasterNo(), localCommunicator()), 
            false);

		return {masterVec, res};

	}

    template<typename T>
	bool collectAllToMaster(const T& val, procVector<T>& masterVec)
	{
		// only on master processor
		auto [vec, res] = collectAllToMaster(val);
        masterVec = vec;
		return res;
	}

}; //procCommunication

} // pFlow::MPI

#endif //__procCommunication_hpp__
