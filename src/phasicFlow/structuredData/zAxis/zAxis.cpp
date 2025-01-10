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


pFlow::zAxis::zAxis(const realx3 &p1, const realx3 &p2) 
:
	p1_(p1),
	p2_(p2)
{
	if( equal(p1,p2))
	{
		fatalErrorInFunction<<
		"points are equal "<< p1 <<" "<<p2<<endl;
		fatalExit;
	}
	makeTransMatrix(p2-p1);
}

pFlow::realx3 pFlow::zAxis::transferToZ(const realx3 & p)const
{
	const auto p0 = p1_-p;
	array2D<real,3,1> po {p0.x(), p0.y(), p0.z()}; 
	
	auto tp = MatMul(rotMat_, po);
	
	return realx3(tp(0,0), tp(1,0), tp(2,0));

}

pFlow::realx3 pFlow::zAxis::transferBackZ(const realx3 & p)const
{

	auto rp = MatMul(invRotMat_,array2D<real,3,1>{p.x(), p.y(), p.z()});

	return realx3(rp(0,0)+p1_.x(),rp(1,0)+p1_.y(), rp(2,0)+p1_.z());
}




void pFlow::zAxis::makeTransMatrix(const realx3& vec)
{
	const realx3 Unit = {0,0,1};
	const auto v_unit = normalize(vec);
	auto uvw = cross(v_unit,Unit);

	const auto rcos = dot(v_unit, Unit);
    const auto rsin = uvw.length();

	if (abs(rsin) > smallValue)
        uvw /= rsin;
	const auto& [u, v, w] = uvw;

	rotMat_ = 
	ArrayType({rcos, 0, 0, 0, rcos, 0, 0, 0, rcos})+
	ArrayType({0, -w, v, w, 0, -u, -v, u, 0})+
	(1-rcos)*ArrayType({u*u, u*v, u*w, u*v, v*v, w*v, u*w, v*w, w*w});

	invRotMat_ = transpose(rotMat_);

}  