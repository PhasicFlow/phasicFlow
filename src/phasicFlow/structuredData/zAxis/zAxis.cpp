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


#include "zAxis.hpp"


pFlow::zAxis::zAxis(const realx3 &p1, const realx3 &p2) :
p1_(p1),
p2_(p2)
{
	n_ = p2-p1;
	auto len = pFlow::length(n_);

	if(len < smallValue )
	{
		fatalExit;
	}
	n_ /= len;

	makeTransMatrix();
}

pFlow::realx3 pFlow::zAxis::transferToZ(const realx3 & p)
{
	real pp[4][1] = {p.x(), p.y(), p.z(), 1.0};
	real pn[4][1];

	MatMul(Trans_z_xz_P1_, pp, pn);
	
	return realx3(
		pn[0][0],
		pn[1][0],
		pn[2][0]
		);

}

pFlow::realx3 pFlow::zAxis::transferBackZ(const realx3 & p)
{
	real pp[4][1] = { p.x(), p.y(), p.z(), 1.0 };
	real pn[4][1];

	MatMul(ITrans_P1_xz_z_, pp, pn);

	return realx3(
		pn[0][0],
		pn[1][0],
		pn[2][0]
		);
}


void pFlow::zAxis::makeTransMatrix()
{
	
	// transfering point p1 to the origin
	real TransP1[4][4] =
	{
		1.0, 0.0, 0.0, -p1_.x(),
		0.0, 1.0, 0.0, -p1_.y(),
		0.0, 0.0, 1.0, -p1_.z(),
		0.0, 0.0, 0.0, 1.0
	};

	// for transformation back
	real ITransP1[4][4] =
	{
		1.0, 0.0, 0.0, p1_.x(),
		0.0, 1.0, 0.0, p1_.y(),
		0.0, 0.0, 1.0, p1_.z(),
		0.0, 0.0, 0.0, 1.0
	};

	
	real u = n_.x();
	real v = n_.y();
	real w = n_.z();
	real u2v2 = sqrt(u*u + v*v);

	//correcting the transformation matrix in the case of coincidence with z - axis
	if ( equal(w,1.0) )
	{
		assignMat(TransP1 , Trans_z_xz_P1_);
		assignMat(ITransP1, ITrans_P1_xz_z_);
		return;
	}

	u2v2 = max(smallValue, u2v2);

	real TransXZ[4][4] =
	{
		u / u2v2, v / u2v2, 0.0, 0.0,
		-v / u2v2, u / u2v2, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	real TransZ[4][4] =
	{
		w, 0.0, -u2v2, 0.0,
		0.0, 1.0, 0.0, 0.0,
		u2v2, 0.0, w, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	real temp[4][4];
	// creat transformation matrix to transfer point from line axis to z-axis
	MatMul(TransXZ, TransP1, temp);
	MatMul(TransZ, temp, Trans_z_xz_P1_);


	real ITransXZ[4][4] =
	{
		u / u2v2, -v / u2v2, 0.0, 0.0,
		+v / u2v2, u / u2v2, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	real ITransZ[4][4] =
	{
		w, 0.0, +u2v2, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-u2v2, 0.0, w, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	// creat transformation matrix to transfer point to from z-axis to line axis
	MatMul(ITransXZ, ITransZ, temp);
	MatMul(ITransP1, temp, ITrans_P1_xz_z_);

}
