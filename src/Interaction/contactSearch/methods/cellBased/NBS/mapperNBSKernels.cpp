

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

#include "mapperNBSKernels.hpp"

void pFlow::mapperNBSKernels::findPointExtends
(
    const deviceViewType1D<realx3>& points,
    const pFlagTypeDevice& flags,
    realx3& minPoint,
    realx3& maxPoint
)
{
    if(flags.numActive() == 0)
    {
        minPoint = {0,0,0};
        maxPoint = {0,0,0};
        return;
    }

    real minX;
    real minY;
    real minZ;
    real maxX;
    real maxY;
    real maxZ;
    auto aRange = flags.activeRange();
    Kokkos::parallel_reduce(
        "pFlow::mapperNBSKernels::findPointExtends",
        deviceRPolicyStatic(aRange.start(), aRange.end()),
        LAMBDA_HD(
            uint32 i, 
            real& minXUpdate, 
            real& minYUpdate, 
            real& minZUpdate,
            real& maxXUpdate,
            real& maxYUpdate,
            real& maxZUpdate)
        {
            if(flags(i))
            {
                auto p = points(i);
                minXUpdate = min(p.x(), minXUpdate);
                minYUpdate = min(p.y(), minYUpdate);
                minZUpdate = min(p.z(), minZUpdate);
                maxXUpdate = max(p.x(), maxXUpdate);
                maxYUpdate = max(p.y(), maxYUpdate);
                maxZUpdate = max(p.z(), maxZUpdate);
            }

        },
        Kokkos::Min<real>(minX),
        Kokkos::Min<real>(minY), 
        Kokkos::Min<real>(minZ),
        Kokkos::Max<real>(maxX),
        Kokkos::Max<real>(maxY),
        Kokkos::Max<real>(maxZ)
    );

    minPoint = {minX, minY, minZ};
    maxPoint = {maxX, maxY, maxZ};
}

bool pFlow::mapperNBSKernels::buildListsReduce
(
    const cells &searchCell, 
    const deviceViewType3D<uint32> &head, 
    const deviceViewType1D<uint32> &next, 
    const deviceViewType1D<realx3> &points, 
    const pFlagTypeDevice &flags
)
{
    uint32 numOut = 0;
    auto aRange = flags.activeRange();

    if(flags.isAllActive())
    {
        Kokkos::parallel_reduce
        (
            "pFlow::mapperNBSKernels::buildListsReduce",
            deviceRPolicyStatic(aRange.start(), aRange.end()),
            LAMBDA_HD(uint32 i, uint32& valToUpdate)
            {	
                int32x3 ind;
                if( searchCell.pointIndexInDomain(points[i], ind) )
                {
                    uint32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
                    next[i] = old;	
                }
                else
                {
                    valToUpdate++;
                }
            },
            numOut
        );
    }
    else
    {
        Kokkos::parallel_reduce
        (
            "pFlow::mapperNBSKernels::buildListsReduce",
            deviceRPolicyStatic(aRange.start(), aRange.end()),
            LAMBDA_HD(uint32 i, uint32& valToUpdate)
            {	
                int32x3 ind;
                if( flags(i) )
                {
                    if( searchCell.pointIndexInDomain(points[i], ind) )
                    {
                        uint32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
                        next[i] = old;	
                    }
                    else
                    {
                        valToUpdate++;
                    }
                }
            },
            numOut
        );
    }

    return numOut == 0u ;
}

bool pFlow::mapperNBSKernels::buildLists
(
    const cells &searchCell, 
    const deviceViewType3D<uint32> &head, 
    const deviceViewType1D<uint32> &next, 
    const deviceViewType1D<realx3> &points, 
    const pFlagTypeDevice &flags
)
{
    auto aRange = flags.activeRange();
    auto pp = points;
    if(flags.isAllActive() )
    { 
        Kokkos::parallel_for
        (
            "pFlow::mapperNBSKernels::buildLists",
            deviceRPolicyStatic(aRange.start(), aRange.end()),
            LAMBDA_HD(uint32 i)
            {               
                auto ind = searchCell.pointIndex(pp[i]);	
                uint32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
                next[i] = old;
            }
        );
        Kokkos::fence();    
    }
    else
    {
       Kokkos::parallel_for
        (
            "pFlow::mapperNBSKernels::buildLists",
            deviceRPolicyStatic(aRange.start(), aRange.end()),
            LAMBDA_HD(uint32 i)
            {
                if( flags(i) )
                {
                    auto ind = searchCell.pointIndex(points[i]);	
                    uint32 old = Kokkos::atomic_exchange(&head(ind.x(), ind.y(), ind.z()), i);
                    next[i] = old;
                }
            }
        );
        Kokkos::fence();
    }

    return true;
}
