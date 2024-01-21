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
#ifndef __MPISimulationDomain_hpp__
#define __MPISimulationDomain_hpp__

#include "simulationDomain.hpp"
#include "partitioning.hpp"
#include "procVector.hpp"
#include "procCommunication.hpp"

namespace pFlow
{

class MPISimulationDomain
:
    public simulationDomain
{
protected:
    MPI::procCommunication      communication_;

    MPI::procVector<domain>      subDomains_;

    uniquePtr<partitioning> domainPartition_ = nullptr;

    uint32 initialNumPoints_ = 0;

    bool createBoundaryDicts() override;

	bool setThisDomain() override;

    std::vector<int> 
    findPlaneNeighbors()const;

public:

    TypeInfo("simulationDomain<MPI>");

    MPISimulationDomain(systemControl& control);

	virtual
	~MPISimulationDomain()=default;

    add_vCtor
	(
		simulationDomain,
		MPISimulationDomain,
		systemControl
	);

	const dictionary& thisBoundaryDict()const override;
    
    /// @brief 
    /// @param pointPos 
    /// @return 
    bool initialUpdateDomains(span<realx3> pointPos)override;
    

    /// @brief 
    /// @return 
    uint32 initialNumberInThis()const override;

    bool initialThisDomainActive()const override;

    bool initialTransferBlockData(
		span<char> src, 
		span<char> dst,
		size_t sizeOfElement)const override;
    
    bool initialTransferBlockData(
        span<realx3> src,
        span<realx3> dst) const override;
	
    bool initialTransferBlockData(
        span<real> src,
        span<real> dst) const override;
    
    bool initialTransferBlockData(
        span<uint32> src,
        span<uint32> dst) const override;
    
    bool initialTransferBlockData(
        span<int32> src,
        span<int32> dst) const override;
    

    /*bool updateDomains(
		span<realx3> pointPos,
        pFlagTypeHost flags) override;*/


	uint32 numberToBeImported()const override;
    
    uint32 numberToBeExported()const override;
	
	bool requiresDataTransfer() const override;
    

};

}

#endif