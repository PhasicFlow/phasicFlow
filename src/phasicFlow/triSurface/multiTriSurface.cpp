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


#include "multiTriSurface.hpp"

/*void pFlow::multiTriSurface::calculateVars()
{
	numSurfaces_ = surfaceNames_.size();

	// make sure the host and device are sync
	lastPointIndex_.syncViews();

	surfaceNumPoints_.clear();
	
	int32 last = 0;
	for(auto& pi:lastPointIndex_) 
	{
		surfaceNumPoints_.push_back(pi+1-last);
		last = pi+1;
	}
	
	// update views
	surfaceNumPoints_.syncViews();

	surfaceNumVertices_.clear();
	last = 0;

	lastVertexIndex_.syncViews();
	

	for(auto& vi:lastVertexIndex_)
	{
		surfaceNumVertices_.push_back(vi+1-last);
		last = vi+1;
	}
	surfaceNumVertices_.syncViews();
	
	pointsStartPos_.reallocate(surfaceNumPoints_.capacity());
	pointsStartPos_.clear();
	
	pointsStartPos_.push_back(0);

	ForAll( i, surfaceNumPoints_)
	{
		if(i==0)continue;

		pointsStartPos_.push_back( pointsStartPos_[i-1] + surfaceNumPoints_[i-1] );
	}

	pointsStartPos_.syncViews();
	

	verticesStartPos_.reallocate(surfaceNumVertices_.capacity());
	verticesStartPos_.clear();

	verticesStartPos_.push_back(0);
	ForAll(i, surfaceNumVertices_)
	{
		if(i==0)continue;
		verticesStartPos_.push_back(verticesStartPos_[i-1] + surfaceNumVertices_[i-1]);
	}
	verticesStartPos_.syncViews();
	

}*/

/*pFlow::multiTriSurface::multiTriSurface()
:
	triSurface(),
	lastPointIndex_("lastPointIndex", "lastPointIndex"),
	lastVertexIndex_("lastVertexIndex", "lastVertexIndex"),
	surfaceNames_("surfaceNames", "surfaceNames")
{
	calculateVars();
}*/

/*bool pFlow::multiTriSurface::addTriSurface
(
	const word& name,
	const triSurface& tSurf
)
{

	const auto& newPoints = tSurf.points();
	const auto& newVertices = tSurf.vertices();
	const auto& newAreas = tSurf.area();
	
	// 
	
	
	points_.append(newPoints);
	
	
	// add new vertices to the existing one
	auto vOldSize = vertices_.size();
	auto vNewSize =  vOldSize + newVertices.size();
	vertices_.resize(vNewSize);
	area_.resize(vNewSize);
	
	auto verVec = vertices_.deviceViewAll();
	auto areaVec = area_.deviceViewAll();

	auto newVerVec = newVertices.deviceViewAll();
	auto newArea = newAreas.deviceViewAll();

	auto maxIdx = maxIndex();

	Kokkos::parallel_for(
		"multiTriSurface::addTriSurface",
		newVertices.size(),
		LAMBDA_HD(int32 i){
			verVec[vOldSize+i] = newVerVec[i]+(maxIdx+1);
			areaVec[vOldSize+i] = newArea[i];
		}
		);
	Kokkos::fence();

	if( !check() )
	{
		fatalErrorInFunction<<
		"the indices and number of points do not match. \n";
		return false;
	}

	lastPointIndex_.push_back(points_.size()-1);
	lastPointIndex_.syncViews();
	
	lastVertexIndex_.push_back(vertices_.size()-1);
	lastVertexIndex_.syncViews();

	surfaceNames_.push_back(name);

	calculateVars();

	return true;

}*/

pFlow::multiTriSurface::multiTriSurface
(
	const objectFile &obj, 
	repository *owner
)
:
	triSurface(obj, owner),
	subscriber("multiTriSurface")
{
	if( !IOobject::readObject() )
    {
        fatalErrorInFunction<<
        "Error in reading from file "<<IOobject::path()<<endl;
        fatalExit;
    }
}

pFlow::multiTriSurface::multiTriSurface
(
	const objectFile &objf, 
	repository *owner, 
	const multiTriSurface &surf
)
:
	triSurface(objf, owner, surf),
	subscriber("multiTriSurface"),
	subSurfaces_(surf.subSurfaces_)
{
	
}

bool pFlow::multiTriSurface::appendTriSurface
(
	const word &name, 
	const realx3x3Vector &triangles
)
{
	uint32 start = size();
	uint32 pointStart = numPoints();

	if(!triSurface::appendTriSurface(triangles))
	{
		fatalErrorInFunction;
		return false;
	}
	uint32 end = size();
	uint32 pointEnd = numPoints();
	subSurfaces_.emplace_back(name, start, end, pointStart, pointEnd);

    return true;
}

bool pFlow::multiTriSurface::read(iIstream &is, const IOPattern &iop)
{	
	subSurfaces_.clear();

	if( !is.findKeywordAndVal("subSurfaces", subSurfaces_ ) )
	{
		fatalErrorInFunction<<
		"Error in reading subSurfaces from stream "<< is.name()<<endl;
		return false;
	}

    return triSurface::read(is, iop);
}

bool pFlow::multiTriSurface::write
(
    iOstream &os,
    const IOPattern &iop
) const
{	

	
	if( iop.thisProcWriteData() )
	{
		os.writeWordEntry("subSurfaces", subSurfaces_);
		if(!os.check(FUNCTION_NAME))return false;
	}
	
	return triSurface::write(os,iop);
}

/*pFlow::real3*
	pFlow::multiTriSurface::beginSurfacePoint
(
	unit i
)
{
	if(i== 0) return points_.data();
	if(i>=numSurfaces())return points_.data()+numPoints();
	return points_.data()+lastPointIndex_[i-1]+1;
}

const pFlow::real3*
	pFlow::multiTriSurface::beginSurfacePoint
(
	unit i
)const
{
	if(i== 0) return points_.data();
	if(i>=numSurfaces())return points_.data()+numPoints();
	return points_.data()+lastPointIndex_[i-1]+1;
}

pFlow::real3* 
	pFlow::multiTriSurface::endSurfacePoint
(
	unit i
)
{
	if(i>=numSurfaces())return points_.data()+numPoints();
	return points_.data()+lastPointIndex_[i]+1;
}

const pFlow::real3*
	pFlow::multiTriSurface::endSurfacePoint
(
	unit i
)const
{
	if(i>=numSurfaces())return points_.data()+numPoints();
	return points_.data()+lastPointIndex_[i]+1;
}*/