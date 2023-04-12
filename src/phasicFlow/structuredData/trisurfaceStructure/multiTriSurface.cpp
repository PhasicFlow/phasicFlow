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

void pFlow::multiTriSurface::calculateVars()
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
	

}

pFlow::multiTriSurface::multiTriSurface()
:
	triSurface(),
	lastPointIndex_("lastPointIndex", "lastPointIndex"),
	lastVertexIndex_("lastVertexIndex", "lastVertexIndex"),
	surfaceNames_("surfaceNames", "surfaceNames")
{
	calculateVars();
}

bool pFlow::multiTriSurface::addTriSurface
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
	
	auto verVec = vertices_.deviceVectorAll();
	auto areaVec = area_.deviceVectorAll();

	auto newVerVec = newVertices.deviceVectorAll();
	auto newArea = newAreas.deviceVectorAll();

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

}

bool pFlow::multiTriSurface::addTriSurface
(
	const word& name,
	const realx3x3Vector& vertices
)
{
	triSurface newSurf(vertices);

	return addTriSurface(name, newSurf);
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

bool pFlow::multiTriSurface::readMultiTriSurface
(
	iIstream& is
)
{
	if( !readTriSurface(is) )return false;

	// from current position
	if(!lastPointIndex_.read(is, true)) return false;

	if(!lastVertexIndex_.read(is, true) ) return false;

	if( !surfaceNames_.read(is, true)) return false;

	calculateVars();

	return true;
}

bool pFlow::multiTriSurface::writeMultiTriSurface
(
	iOstream& os
)const
{
	if(!writeTriSurface(os)) return false;

	os << lastPointIndex_;
	os << lastVertexIndex_;
	os << surfaceNames_;

	return os.check(FUNCTION_NAME);
}