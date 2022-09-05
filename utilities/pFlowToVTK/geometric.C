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


#include "geometric.H"


template<>
bool pFlow::dataToVTK( vtkFile& vtk, const triSurface& surface )
{
	

	auto nP = surface.numPoints();
	auto hPoints = surface.points().hostVector();

	vtk() << "DATASET POLYDATA" << endl;
	vtk() << "POINTS " << nP << " float" << endl;


	for ( auto i=0; i<nP; i++ )
	{
		vtk() << hPoints[i].x()  << " " << hPoints[i].y() << " " << hPoints[i].z() << endl;
		if (!vtk) return false;
	}

	auto nV = surface.numTriangles();
	auto hVertices = surface.vertices().hostVector();

	vtk() << "POLYGONS " << nV << "  " << 4*nV << endl;

	for(auto i=0; i<nV; i++)
	{
		vtk()<< 3 <<" "<< hVertices[i].x() << " " << hVertices[i].y() <<" "<<hVertices[i].z()<<endl;
		if (!vtk) return false;
	}

	return true;
} 

template<>
bool pFlow::dataToVTK( vtkFile& vtk, const multiTriSurface& surface )
{

	auto nP = surface.numPoints();
	auto hPoints = surface.points().hostVector();

	vtk() << "DATASET POLYDATA" << endl;
	vtk() << "POINTS " << nP << " float" << endl;


	for ( auto i=0; i<nP; i++ )
	{
		vtk() << hPoints[i].x()  << " " << hPoints[i].y() << " " << hPoints[i].z() << endl;
		if (!vtk) return false;
	}

	auto nV = surface.numTriangles();
	auto hVertices = surface.vertices().hostVector();

	vtk() << "POLYGONS " << nV << "  " << 4*nV << endl;

	for(auto i=0; i<nV; i++)
	{
		vtk()<< 3 <<" "<< hVertices[i].x() << " " << hVertices[i].y() <<" "<<hVertices[i].z()<<endl;
		if (!vtk) return false;
	}

	return true;

}
