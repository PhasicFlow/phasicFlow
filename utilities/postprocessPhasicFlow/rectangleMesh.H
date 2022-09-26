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

#ifndef __rectangleMesh_H__
#define __rectangleMesh_H__

#include "cells.H"

namespace pFlow
{



class rectangleMesh
:
	public cells<int32>
{
	
public:

	TypeNameNV("rectangleMesh");
	

	INLINE_FUNCTION_HD
	rectangleMesh(){};

	INLINE_FUNCTION_HD
	rectangleMesh(
		const realx3& minP,
		const realx3& maxP,
		int32 nx,
		int32 ny,
		int32 nz)
	:
	cells(
		box(minP, maxP),
		nx, ny, nz)
	{}

	INLINE_FUNCTION_H
	rectangleMesh(const dictionary & dict)
	:
	cells(
		box(
			dict.getVal<realx3>("min"),
			dict.getVal<realx3>("max")),
		dict.getVal<int32>("nx"),
		dict.getVal<int32>("ny"),
		dict.getVal<int32>("nz")
		)
	{}

	INLINE_FUNCTION_HD
	rectangleMesh(const rectangleMesh&) = default;

	INLINE_FUNCTION_HD
	rectangleMesh& operator = (const rectangleMesh&) = default;

	INLINE_FUNCTION_HD
	rectangleMesh(rectangleMesh&&) = default;

	INLINE_FUNCTION_HD
	rectangleMesh& operator = (rectangleMesh&&) = default;

	INLINE_FUNCTION_HD
	~rectangleMesh() = default;


	INLINE_FUNCTION_HD
	int64 size()const
	{
		return this->totalCells();
	}

	INLINE_FUNCTION_HD
	real cellVol()const
	{
		auto [dx,dy,dz] = this->cellSize();
		return dx*dy*dz;
	}

	INLINE_FUNCTION_H
	auto minPoint()const
	{
		return domain().minPoint();
	}

	INLINE_FUNCTION_H
	auto maxPoint()const
	{
		return domain().maxPoint();
	}

	bool read(iIstream& is)
	{
		return true;
	}

	bool write(iOstream& os)const
	{
		return true;
	}

	bool writeToVtk(iOstream& os)const
	{
		os<<"DATASET RECTILINEAR_GRID"<<endl;
		os<<"DIMENSIONS "<<nx()+1<<" "<< ny()+1 << " "<< nz()+1 <<endl;
		
		auto [x, y , z] = this->minPoint();
		auto [dx, dy, dz] = this->cellSize();

		os<<"X_COORDINATES "<< nx()+1 <<" float\n";
		for(int32 i=0; i<nx()+1; i++)
		{
			os<< x<<"\n";
			x+= dx;
		}

		os<<"Y_COORDINATES "<< ny()+1 <<" float\n";
		for(int32 j=0; j<ny()+1; j++)
		{
			os<< y <<"\n";
			y+= dy;
		}

		os<<"Z_COORDINATES "<< nz()+1 <<" float\n";
		for(int32 j=0; j<nz()+1; j++)
		{
			os<< z <<"\n";
			z+= dz;
		}

		os<<"CELL_DATA "<< nx()*ny()*nz()<<endl;

		return true;
	}	
	
};



}


#endif // __rectangleMesh_H__
