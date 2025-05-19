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

#include "partitioning.hpp"
#include "procCommunication.hpp"
#include "procVector.hpp"
#include "simulationDomain.hpp"

namespace pFlow::MPI
{

class MPISimulationDomain : public simulationDomain
{
private:

	/// a processor communcator for simulation domain 
	procCommunication  communication_;

	/// sub-domain (thisDomain_ for all processors)
	procVector<domain> subDomainsAll_;

	/// number of points in all processors 
	procVector<uint32> numPointsAll_;

	/// partitioning object 
	uniquePtr<partitioning> domainPartitioning_ = nullptr;

	/// the acutal limits of the simulation domain in this processor 
	domain                  thisDomain_;

	uint32                  initialNumPoints_ = 0;

	bool                    thisDomainActive_ = false;

	bool                    createBoundaryDicts() final;

	bool                    setThisDomain() final;

	std::vector<int>        findPlaneNeighbors() const;

public:

	TypeInfo("simulationDomain<MPI>");

	explicit MPISimulationDomain(systemControl& control, real maxBSphere);

	~MPISimulationDomain() final = default;

	add_vCtor
    (
        simulationDomain, 
        MPISimulationDomain, 
        systemControl
    );

	/// @brief
	/// @param pointPos
	/// @return
	bool initialUpdateDomains(span<realx3> pointPos) final;

	/// @brief
	/// @return
	uint32 initialNumberInThis() const final;

	bool  initialTransferBlockData(
			span<char> src,
			span<char> dst,
			size_t     sizeOfElement
			) const final;

	bool initialTransferBlockData(span<realx3> src, span<realx3> dst)
	  const final;

	bool initialTransferBlockData(span<real> src, span<real> dst)
	  const final;

	bool initialTransferBlockData(span<uint32> src, span<uint32> dst)
	  const final;

	bool initialTransferBlockData(span<int32> src, span<int32> dst)
	  const final;

	uint32 numberToBeImported() const final;

	uint32 numberToBeExported() const final;

	/// @brief Is this domain active?
	/// Active mean, there is particle in it and
	/// boundaries and other entities of simulation domains are valid
	bool   domainActive() const final;

	const domain& thisDomain()const final;
};

} // namespace pFlow::MPI

#endif // 