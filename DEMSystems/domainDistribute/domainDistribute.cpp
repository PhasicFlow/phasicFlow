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


void pFlow::domainDistribute::clcDomains(const std::vector<box>& domains)
{
	realx3 dl = domainExtension_ * maxBoundingBoxSize_;

	for(int32 i=0; i<numDomains_; i++)
	{
		extDomains_[i] = extendBox(domains[i], dl);
	}
}


pFlow::domainDistribute::domainDistribute(
	const Vector<box>& domains,
	real maxBoundingBox)
:
numDomains_(domains.size()),
extDomains_("extDomains", numDomains_),
particlesInDomains_("particlesInDomains", numDomains_),
numParInDomain_("numParInDomain", numDomains_, 0),
maxBoundingBoxSize_(maxBoundingBox)
{

	clcDomains(domains);
}

bool pFlow::domainDistribute::locateParticles(
		ViewType1D<realx3,HostSpace> points, includeMask mask)
{
	
	range activeRange = mask.activeRange();

	for(int32 di=0; di<numDomains_; di++)
	{
		particlesInDomains_[di].clear();
	}

	for(int32 i=activeRange.first; i<activeRange.second; i++)
	{
		if(mask(i))
		{
			for(int32 di=0; di<numDomains_; di++)
			{
				if(extDomains_[di].isInside(points[i]))
				{
					particlesInDomains_[di].push_back(i);
				}
			}
		}
	}

	for(int32 di=0; di<numDomains_; di++)
	{
		numParInDomain_[di] = particlesInDomains_[di].size();
	}

	output<<" numParInDomain_ "<< numParInDomain_<<endl;

	return true;
}

bool pFlow::domainDistribute::changeDomainsSize(
		real extentFraction, 
		real maxBoundingBoxSize, 
		const std::vector<box>& domains)
{
	domainExtension_ = extentFraction;
	maxBoundingBoxSize_ = maxBoundingBoxSize;

	if(domains.size()!= numDomains_)
	{
		fatalErrorInFunction<<"number of new domians differs"<<endl;
		return false;
	}

	clcDomains(domains);
	return true;
}


/*bool pFlow::domainDistribute::locateParticles(
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
}*/