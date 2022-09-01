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


#include "triSurface.H"
#include "error.H"
#include "iOstream.H"
#include "triSurfaceKernels.H"


pFlow::int32  pFlow::triSurface::addTriangle
(
	const realx3x3 & tri,
	realx3Vector& points,
	int32x3Vector& vertices
)
{
	int32x3 newTri;

		// first point
	if ( int32 i = find(points, tri.x()) ; i > -1)
	{ 
		newTri.x() = i; // existing point 
	}
	else
	{ 
		points.push_back(tri.x());  // new point
		newTri.x() = static_cast<int32>(points.size()) - 1;
	}
	
	// second point
	
	if ( int32 j = find(points, tri.y()) ; j > -1)
	{ 
		newTri.y() = j; // existing point 
	}
	else
	{ 
		points.push_back(tri.y());  // new point
		newTri.y() = static_cast<int32>(points.size()) - 1;
	}
	
	// third point 
	
	if ( int32 k = find(points, tri.z()) ; k > -1)
	{ 
		newTri.z() = k; // existing point 
	}
	else
	{ 
		points.push_back(tri.z());  // new point
		newTri.z() = static_cast<int32>(points.size()) - 1;
	}
	
	// adds the new tirple vertices to the list
	vertices.push_back( newTri );

	//output<< " points " <<points << endl;
	//output<< " vertices "<<vertices <<endl;

	return static_cast<int32>(vertices.size()) -1;
}

pFlow::int32 pFlow::triSurface::calcMaxIndex()const
{

	int32 maxIdx = -1;

	if(vertices_.size()>0)
	{
		auto verDeviceVec = vertices_.deviceVectorAll();
		
		Kokkos::parallel_reduce(
			"triSurface::calcMaxIndex",
			vertices_.size(),
			LAMBDA_HD(int32 i, int32& valueToUpdate){
				valueToUpdate = max(valueToUpdate, max(verDeviceVec[i]));
			},
			Kokkos::Max<int32>( maxIdx )
			);
	}

	return maxIdx;
}

bool pFlow::triSurface::check()
{

	auto maxIdx = calcMaxIndex();
	maxIndex_ = maxIdx;
	if( maxIdx >= static_cast<int32>(points_.size())) return false;
	return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
pFlow::triSurface::triSurface()
:
	points_("points"),
	vertices_("vertices"),
	area_("area"),
	maxIndex_(-1)
{}

pFlow::triSurface::triSurface
(
	const realx3Vector& points,
	const int32x3Vector& vertices
)
:
	points_("points", "points", points),
	vertices_("vertices", "vertices", vertices),
	area_("area", "area"),
	maxIndex_(-1)
{
	if( check() )
	{
		fatalErrorInFunction<<
		"the indices and number of points do not match. \n";
		fatalExit;
	}

	area_.reallocate(vertices_.capacity(), vertices_.size());

	pFlow::triSurfaceKernels::calculateArea(points_, vertices_, area_);

}


pFlow::triSurface::triSurface
(
	const realx3x3Vector& triangles
)
:
	points_("points"),
	vertices_("vertices"),
	area_("area")
{

	Vector<realx3> points;
	Vector<int32x3> vertices;
	
	points.clear();
	vertices.clear();

	for( const auto& tr: triangles)
	{
		if(auto i= addTriangle(tr, points, vertices); i < 0 )
		{
			fatalErrorInFunction <<
			"failed to insert a triangle into triSurface. \n";
			fatalExit;
		}
	}

	points_.assign(points);

	vertices_.assign(vertices);

	area_.reallocate(vertices_.capacity(), vertices_.size());

	pFlow::triSurfaceKernels::calculateArea(points_, vertices_, area_);

	if( !check() )
	{
		fatalErrorInFunction<<
		"the indices and number of points do not match. \n";
		fatalExit;
	}
}


bool pFlow::triSurface::readTriSurface
(
	iIstream& is
)
{

	is.fatalCheck(FUNCTION_NAME);

	is >> points_;
	is.fatalCheck(FUNCTION_NAME);

	is >> vertices_;
	is.fatalCheck(FUNCTION_NAME);

	if( !check() )
	{
		ioErrorInFile( is.name(), is.lineNumber() ) <<
		"the indices and number of points do not match. \n";
		return false;
	}

	area_.reallocate(vertices_.capacity(), vertices_.size());

	pFlow::triSurfaceKernels::calculateArea(points_, vertices_, area_);

	return true;
}

bool pFlow::triSurface::writeTriSurface
(
	iOstream& os
)const
{
	
	os << points_;
	os << vertices_;

	return os.check(FUNCTION_NAME);;
}




