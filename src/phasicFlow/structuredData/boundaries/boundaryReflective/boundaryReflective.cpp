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

#include "boundaryReflective.hpp"
#include "pointFields.hpp"
#include "dictionary.hpp"
#include "Time.hpp"

pFlow::boundaryReflective::boundaryReflective
(
    const dictionary &dict, 
    const plane &bplane, 
    internalPoints &internal, 
    boundaryList &bndrs, 
    uint32 thisIndex
)
:
    boundaryBase
    (
        dict,
        bplane,
        internal,
        bndrs,
        thisIndex
    )
{
    restitution_ = dict.getValOrSet("restitution", restitution_);
    velocityName_ = dict.getValOrSet("velocityName",velocityName_);
    diameterName_ = dict.getValOrSet("diameterName", diameterName_);
}

bool pFlow::boundaryReflective::beforeIteration(
    uint32 iterNum,
    real t,
    real dt)
{
    return true;
}

bool pFlow::boundaryReflective::iterate
(
    uint32 iterNum, 
    real t, 
    real dt
)
{
    return true;
}

bool pFlow::boundaryReflective::afterIteration
(
    uint32 iterNum, 
    real t, 
    real dt
)
{
    if(empty())return true;
    
    uint32 s = size();
	uint32Vector_D inContactFlags("inContactFlags",s+1, s+1, RESERVE()); 
	inContactFlags.fill(0u);
	auto inContactFlagsD = inContactFlags.deviceViewAll();

	auto points = thisPoints();
    
	auto p = boundaryPlane().infPlane();
    const auto &diam = time().lookupObject<realPointField_D>(diameterName_);
	auto diams = diam.BoundaryField(thisBoundaryIndex()).thisField();

	uint32 numInContact = 0;

	Kokkos::parallel_reduce
	(
		"pFlow::boundaryReflective::afterIteration",
		deviceRPolicyStatic(0u,s),
		LAMBDA_HD(uint32 i, uint32& nContactToUpdate)
		{
			if(p.inPositiveDistance(points(i), 0.5*diams(i)))
			{
				inContactFlagsD(i)=1;
				nContactToUpdate++;
			}
		}, 
		numInContact
	);

    // no particle in contact 
	if(numInContact == 0 )
	{
		return true;
	}

    uint32Vector_D inContactList("inContactList", numInContact);
    const auto& inContactListD = inContactList.deviceViewAll();

    exclusiveScan(inContactFlagsD, 0u, s+1, inContactFlagsD, 0u);
    
    Kokkos::parallel_for
    (
        "pFlow::boundaryReflective::afterIteration",
        deviceRPolicyStatic(0, s),
        LAMBDA_HD(uint32 i)
		{
			if(inContactFlagsD(i)!= inContactFlagsD(i+1))
				inContactListD(inContactFlagsD(i)) = points.index(i);
		}
    );
    Kokkos::fence(); 

    const auto& velocity = time().lookupObject<realx3PointField_D>(velocityName_);
    const auto& velocityD = velocity.deviceViewAll();

    Kokkos::parallel_for(
        "pFlow::boundaryReflective::velocityChange",
        deviceRPolicyStatic(0,numInContact),
        LAMBDA_HD(uint32 i)
        {
            auto& vel = velocityD(inContactListD(i));
            real vn = dot(p.normal(), vel);
            
            if(vn < 0)
            {
                realx3 vt = vel - vn*p.normal();
                vel = restitution_*(vt - vn*p.normal());
            }
        }
    );
    Kokkos::fence();
    
    // TODO: notify integration for changes in the velocity 

    return true;
}
