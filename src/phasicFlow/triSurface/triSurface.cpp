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


#include "triSurface.hpp"
#include "Vectors.hpp"
#include "triangleFunctions.hpp"
#include "error.hpp"
#include "iOstream.hpp"

//#include "triSurfaceKernels.hpp"

namespace pFlow
{
bool convertToTriSurfaceComponents
(
	uint32 basePointIndex,
	span<realx3x3> tris, 
	realx3Vector& points,
	uint32x3Vector& vertices,
	realVector& area,
	realx3Vector& normal
)
{
	auto nt = tris.size();

	points.clear();
	points.reserve(nt);
	
	vertices.clear();
	vertices.resize(nt);

	area.clear();
	area.resize(nt);

	normal.clear();
	normal.resize(nt);

	for(auto i=0; i<nt; i++)
	{
		uint32x3 newTri;
		const auto& tri = tris[i];
		if(!pFlow::triangle::valid(tri.comp1(),tri.comp2(), tri.comp3()))
		{
			fatalErrorInFunction<<
			"Invalid triangle "<< tri << endl;
			return false;
		}

		if(auto np = find(points, tri.comp1()); np<0 )
		{
			points.push_back(tri.comp1());  // new point
			newTri.comp1() = static_cast<uint32>( basePointIndex + points.size() - 1);
		}
		else
		{
			newTri.comp1() = basePointIndex+static_cast<uint32>(np);
		}

		if(auto np = find(points, tri.comp2()); np<0 )
		{
			points.push_back(tri.comp2());  // new point
			newTri.comp2() = static_cast<uint32>( basePointIndex + points.size() - 1);
		}
		else
		{
			newTri.comp2() = basePointIndex+static_cast<uint32>(np);
		}

		if(auto np = find(points, tri.comp3()); np<0 )
		{
			points.push_back(tri.comp3());  // new point
			newTri.comp3() = static_cast<uint32>( basePointIndex + points.size() - 1);
		}
		else
		{
			newTri.comp3() = basePointIndex+static_cast<uint32>(np);
		}

		vertices[i] = newTri;
		normal[i] = triangle::normal(tri.comp1(), tri.comp2(), tri.comp3());
		area[i] = triangle::surface(tri.comp1(), tri.comp2(), tri.comp3());
	}
	
	return true;
}

}


pFlow::triSurface::triSurface
(
	const objectFile &obj, 
	repository *owner
)
:
	IOobject
	(
		obj,
		IOPattern::AllProcessorsSimilar,
		owner
	),
	points_("points", "points"),
	vertices_("vertices", "vertices"),
	area_("area", "area"),
	normals_("normals","normals")
{
}

pFlow::triSurface::triSurface
(
	const objectFile &objf,
	repository* owner, 
	const triSurface &surf
)
:
	IOobject
	(
		objectFile
		(
			objf.name(),
			objf.localPath(),
			objectFile::READ_NEVER,
			objf.wFlag()
		),
		IOPattern::AllProcessorsSimilar,
		owner
	),
	points_(surf.points_),
	vertices_(surf.vertices_),
	area_(surf.area_),
	normals_(surf.normals_)
{}

pFlow::triSurface::triSurface(
    const realx3x3Field_H &triangles,
    repository *owner)
    : IOobject(
          objectFile(
              triangles.name(),
              "",
              IOobject::READ_NEVER,
              IOobject::WRITE_ALWAYS),
          IOPattern::AllProcessorsSimilar,
          owner),
      points_("points", "points"),
      vertices_("vertices", "vertices"),
      area_("area", "area"),
      normals_("normals", "normals")
{
	if( !appendTriSurface(triangles) )
	{
		fatalExit;
	}

}

bool pFlow::triSurface::appendTriSurface
(
	const realx3x3Field_H &triangles
)
{
	uint32 basePointIndex = numPoints();

	auto triData = triangles.getSpan();

	realx3Vector points("points");
	uint32x3Vector vertices("vertices");
	realVector area("area");
	realx3Vector normal("normal");

	if( !convertToTriSurfaceComponents(
		basePointIndex, 
		triData,
		points,
		vertices,
		area,
		normal))
	{
		fatalErrorInFunction<<
		"Error in constructing triSuface from raw data "<<triangles.name()<<endl;
		return false;
	}

	points_.append(points);
	vertices_.append(vertices);
	area_.append(area);
	normals_.append(normal);

    return true;
}

bool pFlow::triSurface::appendTriSurface(const realx3x3Vector &triangles)
{
    uint32 basePointIndex = numPoints();

	auto triData = triangles.getSpan();

	realx3Vector points("points");
	uint32x3Vector vertices("vertices");
	realVector area("area");
	realx3Vector normal("normal");

	if( !convertToTriSurfaceComponents(
		basePointIndex, 
		triData,
		points,
		vertices,
		area,
		normal))
	{
		fatalErrorInFunction<<
		"Error in constructing triSuface from raw data "<<triangles.name()<<endl;
		return false;
	}

	points_.append(points);
	vertices_.append(vertices);
	area_.append(area);
	normals_.append(normal);

    return true;
}

bool pFlow::triSurface::read(iIstream &is, const IOPattern &iop)
{
	points_.clear();
	if(!points_.read(is, iop))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		" when reading field "<< points_.name()<<endl;
		return false;
	}

	vertices_.clear();
	if(!vertices_.read(is, iop))
	{
		ioErrorInFile(is.name(), is.lineNumber())<<
		" when reading field "<< vertices_.name()<<endl;
		return false;
	}
	
	WARNING<<"You should calculate area and normal after reading "<<END_WARNING;

    return true;
}

bool pFlow::triSurface::write(iOstream &os, const IOPattern &iop) const
{
	if( !points_.write(os, iop))
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"when writing field "<<points_.name()<<endl;
		return false;
	}

	if( !vertices_.write(os,iop) )
	{
		ioErrorInFile(os.name(), os.lineNumber())<<
		"when writing field "<<vertices_.name()<<endl;
		return false;
	}

    return true;
}
