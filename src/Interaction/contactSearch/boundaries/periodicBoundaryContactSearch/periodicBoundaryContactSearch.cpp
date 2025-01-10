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

#include "periodicBoundaryContactSearch.hpp"
#include "contactSearch.hpp"
#include "particles.hpp"
#include "pointStructure.hpp"
#include "geometry.hpp"


void pFlow::periodicBoundaryContactSearch::setSearchBox()
{
    
    auto db = pStruct().thisDomain().domainBox();
    auto n1 = boundary().mirrorBoundary().boundaryPlane().normal();
    auto l1 = boundary().mirrorBoundary().neighborLength();
    auto n2 = boundary().boundaryPlane().normal();
    auto l2 = boundary().neighborLength();

    realx3 minP = db.minPoint() + (db.maxPoint()-db.minPoint())* n1+(n2*l2);
    realx3 maxP = db.maxPoint() + (n1*l1);

    searchBox_={minP, maxP};
}

pFlow::periodicBoundaryContactSearch::periodicBoundaryContactSearch(
    const dictionary &dict,
    const boundaryBase &boundary,
    const contactSearch &cSearch)
: 
    boundaryContactSearch(dict, boundary, cSearch),
    transferVec_(boundary.mirrorBoundary().displacementVectroToMirror()),
    thisIndex_(thisBoundaryIndex()),
    mirrorIndex_(mirrorBoundaryindex()),
    diameter_(cSearch.Particles().boundingSphere())
{
    
    if(thisIndex_%2==1)
    {
        masterSearch_ = true;
        
        setSearchBox();
        
        real minD;
        real maxD;
        cSearch.Particles().boundingSphereMinMax(minD, maxD);
        
        ppContactSearch_ = makeUnique<ppwBndryContactSearch>(
            searchBox_,
            maxD);

        const auto& geom = cSearch.Geometry();

        pwContactSearch_ = makeUnique<wallBoundaryContactSearch>(
            0.5,
            geom.numPoints(),
            geom.size(),
            geom.points().deviceViewAll(),
            geom.vertices().deviceViewAll(),
            geom.normals().deviceViewAll());
    }
    else
    {
        masterSearch_ = false;
        searchBox_={{0,0,0},{0,0,0}};
    }
}

bool pFlow::periodicBoundaryContactSearch::broadSearch
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
        
        auto thisP = boundary().thisPoints();
        auto thisDiams = diameter_.BoundaryField(thisIndex_).thisField();
        auto mirrorP = mirrorBoundary().thisPoints();
        auto mirrorDiams = diameter_.BoundaryField(mirrorIndex_).thisField();

        ppContactSearch_().broadSearchPP(
            ppPairs, 
            thisP, 
            thisDiams,
            mirrorP,
            mirrorDiams,
            transferVec_);
        
        /*pwContactSearch_().broadSearch(
            pwPairs, 
            ppContactSearch_().searchCells(),
            thisP,
            thisDiams,
            mirrorP,
            mirrorDiams,
            transferVec_,
            ppContactSearch_().sizeRatio());*/

            //output<<t<<"  boundary pp size "<< ppPairs.size()<<endl;
            //output<<t<<"  boundary pw size "<< pwPairs.size()<<endl;
        
        return true;

    }else
    {
        return true;
    }
}
