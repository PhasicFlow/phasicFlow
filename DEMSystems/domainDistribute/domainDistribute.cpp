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

#include "domainDistribute.hpp"



pFlow::coupling::domainDistribute::domainDistribute(
	int32 numDomains, 
	const Vector<box>& domains_,
	real maxBoundingBox)
:
numDomains_(numDomains),
extDomains_("extDomains", numDomains),
particlesInDomains_("particlesInDomains", numDomains),
numParInDomain_("numParInDomain", numDomains, 0),
maxBoundingBoxSize_(maxBoundingBox)
{

	realx3 dl = domainExtension_ * maxBoundingBoxSize_;

	for(int32 i=0; i<numDomains_; i++)
	{
		extDomains_[i] = extendBox(domains_[i], dl);
	}
}

bool pFlow::coupling::domainDistribute::locateParticles(
		ViewType1D<realx3,HostSpace> points, includeMask mask)
	{
		range active = mask.activeRange();
		auto numInDomain = numParInDomain_.deviceVectorAll();
		auto numDomains = numDomains_;

		using policy = Kokkos::RangePolicy<
			DefaultHostExecutionSpace,
			Kokkos::IndexType<int32> >;

		Kokkos::parallel_for("locateParticles",
			policy(active.first, active.second),
			LAMBDA_HD(int32 i){
				if(mask(i))
				{
					for(int32 di=0; di<numDomains; di++)
					{
						if(extDomains_[i].isInside(points[i]))
							Kokkos::atomic_add(&numInDomain[di],1);
					}
				}
			});
		Kokkos::fence();

		
		for(int32 i=0; i<numDomains_; i++)
		{
			particlesInDomains_[i].resize(numParInDomain_[i]);
		}

		numParInDomain_.fill(0);

		auto particlesInDomainsPtr = particlesInDomains_.data();

		Kokkos::parallel_for("locateParticles",
			policy(active.first, active.second),
			LAMBDA_HD(int32 i){
				if(mask(i))
				{
					for(int32 di=0; di<numDomains; di++)
					{
						if(extDomains_[i].isInside(points[i]))
						{

							particlesInDomainsPtr[di][numInDomain[di]] = i;
							Kokkos::atomic_add(&numInDomain[di],1);

						}
					}
				}
			});
		Kokkos::fence();		


		return true;
	}