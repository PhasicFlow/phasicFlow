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

#ifndef __rectangleMesh_hpp__
#define __rectangleMesh_hpp__

#include "types.hpp"
#include "error.hpp"
#include "box.hpp"
#include "IOobject.hpp"

class repository;

namespace pFlow
{

class rectangleMesh
:
	public IOobject
{
private:

	box 		meshBox_;

	int32x3 	numCells_;

	realx3 		dx_;

public:

	TypeInfo("rectangleMesh");

	rectangleMesh(
		const box& mshBox,
		int32 nx,
		int32 ny,
		int32 nz,
		repository* rep);
	
	rectangleMesh(const dictionary & dict, repository* rep);

	
	rectangleMesh(const rectangleMesh&) = default;

	
	rectangleMesh& operator = (const rectangleMesh&) = default;

	rectangleMesh(rectangleMesh&&) = default;

	
	rectangleMesh& operator = (rectangleMesh&&) = default;

	~rectangleMesh() override = default;

	int64 size()const
	{
		return numCells_.x_*numCells_.y_*numCells_.z_;
	}

	int32 nx()const
	{
		return numCells_.x();
	}

	int32 ny()const
	{
		return numCells_.y();
	}

	int32 nz()const
	{
		return numCells_.z();
	}

	real cellVol()const
	{
		return dx_.x_*dx_.y_*dx_.z_;
	}
	
	realx3 minPoint()const
	{
		return meshBox_.minPoint();
	}
	
	realx3 maxPoint()const
	{
		return meshBox_.maxPoint();
	}

	inline
	bool isInsideIndex(const realx3 p, int32x3 & ind )const
	{
		if(meshBox_.isInside(p))
		{
			ind = (p - meshBox_.minPoint())/dx_ ;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool write(iOstream& is, const IOPattern& iop)const override
	{
		notImplementedFunction;
		return true;
	}

	bool read(iIstream& is, const IOPattern& iop) override
	{
		notImplementedFunction;
		return true;
	}

	bool writeToVtk(iOstream& os)const
	{
		os<<"DATASET RECTILINEAR_GRID"<<endl;
		os<<"DIMENSIONS "<<nx()+1<<" "<< ny()+1 << " "<< nz()+1 <<endl;
		
		auto [x, y , z] = this->minPoint();
		auto [dx, dy, dz] = dx_;

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


#endif // __rectangleMesh_hpp__
