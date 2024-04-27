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

#ifndef __regularSimulationDomain_hpp__
#define __regularSimulationDomain_hpp__

#include "simulationDomain.hpp"

namespace pFlow
{

class regularSimulationDomain 
: 
	public simulationDomain
{
private:

	domain thisDomain_;

	uint32 initialNumPoints_ = 0;

	bool   createBoundaryDicts() final;

	bool   setThisDomain() final;

public:

	TypeInfo("simulationDomain<regular>");

	explicit regularSimulationDomain(systemControl& control);

	~regularSimulationDomain() final = default;

	add_vCtor(simulationDomain, regularSimulationDomain, systemControl);

	bool   initialUpdateDomains(span<realx3> pointPos) final;

	uint32 initialNumberInThis() const final;

	uint32 numberToBeImported() const final;

	uint32 numberToBeExported() const final;

	/// @brief Is this domain active?
	/// Active mean, there is particle in it and
	/// boundaries and other entities of simulation domains are valid
	bool domainActive() const final;

	/// @brief return the simulation domain of this processor
	const domain& thisDomain() const final;

	bool initialTransferBlockData(
	  span<char> src,
	  span<char> dst,
	  size_t     sizeOfElement
	) const final;

	/// @brief
	/// @param src
	/// @param dst
	/// @return
	bool initialTransferBlockData(span<realx3> src, span<realx3> dst)
	  const final;

	bool initialTransferBlockData(span<real> src, span<real> dst)
	  const final;

	bool initialTransferBlockData(span<uint32> src, span<uint32> dst)
	  const final;

	bool initialTransferBlockData(span<int32> src, span<int32> dst)
	  const final;

	const dictionary& thisBoundaryDict() const final;

};

}

#endif //__regularSimulationDomain_hpp__
