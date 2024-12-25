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

#ifndef __domainDistribute_hpp__
#define __domainDistribute_hpp__

#include "box.hpp"
#include "Vectors.hpp"
#include "VectorSingles.hpp"
#include "pointStructure.hpp"

namespace pFlow
{

class domainDistribute
{
protected:
	
	// protected members 

	int32 									numDomains_;

	VectorSingle<box,HostSpace> 			extDomains_;

	Vector<VectorSingle<int32,HostSpace>>	particlesInDomains_;
	
	int32Vector_H 							numParInDomain_;

	real maxBoundingBoxSize_;

	real domainExtension_ = 1.0;

	void clcDomains(const std::vector<box>& domains);

public:

	domainDistribute(
		const Vector<box>& domains,
		real maxBoundingBox);

	~domainDistribute()=default;

	domainDistribute(const domainDistribute&)=delete;

	domainDistribute& operator=(const domainDistribute&)=delete;

	int32 numDomains()const
	{
		return numDomains_;
	}
	
	int32 numParInDomain(int32 di)const
	{
		return numParInDomain_[di];
	}

	span<const int32> particlesInDomain(int32 di)const
	{
		return
		span<const int32>(
			particlesInDomains_[di].hostViewAll().data(),
			numParInDomain_[di]
			);
	}

	bool changeDomainsSize(
		real extentFraction, 
		real maxBoundingBoxSize, 
		const std::vector<box>& domains);
	

	//template<typename includeMask>
	bool locateParticles(
		ViewType1D<realx3,HostSpace> points, const pFlagTypeHost& mask);
	
};


} // pFlow::coupling

#endif //__domainDistribute_hpp__