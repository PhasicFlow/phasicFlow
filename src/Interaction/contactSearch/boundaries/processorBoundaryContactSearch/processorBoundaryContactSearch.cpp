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

#include "processorBoundaryContactSearch.hpp"
#include "contactSearch.hpp"
#include "particles.hpp"
//#include "pointStructure.hpp"
//#include "geometry.hpp"


void pFlow::processorBoundaryContactSearch::setSearchBox()
{
       
    auto l = boundary().neighborLength();
    auto n = boundary().boundaryPlane().normal();
    auto pp1 = boundary().boundaryPlane().parallelPlane(l);
    auto pp2 = boundary().boundaryPlane().parallelPlane(-l);

    realx3 minP1 = min(min(min(pp1.p1(), pp1.p2()), pp1.p3()), pp1.p4());
    realx3 maxP1 = max(max(max(pp1.p1(), pp1.p2()), pp1.p3()), pp1.p4());

    realx3 minP2 = min(min(min(pp2.p1(), pp2.p2()), pp2.p3()), pp2.p4());
    realx3 maxP2 = max(max(max(pp2.p1(), pp2.p2()), pp2.p3()), pp2.p4());

    auto minP = min(minP1, minP2) - l*(realx3(1.0)-abs(n));
    auto maxP = max(maxP1, maxP2) + l*(realx3(1.0)-abs(n));
    
    searchBox_={minP, maxP};
}

pFlow::processorBoundaryContactSearch::processorBoundaryContactSearch(
    const dictionary &dict,
    const boundaryBase &boundary,
    const contactSearch &cSearch)
: 
    boundaryContactSearch(dict, boundary, cSearch),
    diameter_(cSearch.Particles().boundingSphere()),
    masterSearch_(this->isBoundaryMaster()),
    sizeRatio_(dict.getVal<real>("sizeRatio"))
{
    
    if(masterSearch_)
    {        
        setSearchBox();
        
        real minD;
        real maxD;
        cSearch.Particles().boundingSphereMinMax(minD, maxD);
        
        ppContactSearch_ = makeUnique<twoPartContactSearch>(
            searchBox_,
            maxD,
            sizeRatio_);
    }
    else
    {
        searchBox_={{0,0,0},{0,0,0}};
    }
}

bool pFlow::processorBoundaryContactSearch::broadSearch
(
    uint32 iter, 
    real t, 
    real dt, 
    csPairContainerType &ppPairs, 
    csPairContainerType &pwPairs, 
    bool force
)
{
    if(masterSearch_)
    {
        const auto thisPoints = boundary().thisPoints();
        const auto& neighborProcPoints = boundary().neighborProcPoints();
        const auto& bDiams = diameter_.BoundaryField(thisBoundaryIndex());
        const auto thisDiams = bDiams.thisField();
        const auto& neighborProcDiams = bDiams.neighborProcField();
        
        ppContactSearch_().broadSearchPP(
            ppPairs, 
            thisPoints, 
            thisDiams,
            neighborProcPoints,
            neighborProcDiams,
            boundaryName()
        );
        //pOutput<<"ppSize "<< ppPairs.size()<<endl;
        return true;

    }else
    {
        return true;
    }
}
