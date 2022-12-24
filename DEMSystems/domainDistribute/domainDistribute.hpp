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

namespace pFlow::coupling
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

	int32 lastUpdateIter_ = 0;

	int32 updateInterval_ = 1;

	real domainExtension_ = 1.0;

	using includeMask = typename pointStructure::activePointsHost;

public:

	domainDistribute(
		int32 numDomains, 
		const Vector<box>& domains_,
		real maxBoundinBox);

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

	//template<typename includeMask>
	bool locateParticles(
		ViewType1D<realx3,HostSpace> points, includeMask mask);
	
};


} // pFlow::coupling

#endif //__domainDistribute_hpp__