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


#ifndef __sphereTriSurfaceContact_hpp__
#define __sphereTriSurfaceContact_hpp__

#include "triWall.hpp"
#include "pLine.hpp"

namespace pFlow::sphTriInteraction
{

INLINE_FUNCTION_HD
bool pointInPlane(
	const realx3& p1,
	const realx3& p2,
	const realx3& p3,
	const realx3& p )
{

	realx3 p1p = p1 - p;
	realx3 p2p = p2 - p;
	realx3 p3p = p3 - p;

	real p1p2 = dot(p1p, p2p);
	real p2p3 = dot(p2p, p3p);
	real p2p2 = dot(p2p, p2p);
	real p1p3 = dot(p1p, p3p);

	// first condition u.v < 0
	// u.v = [(p1-p)x(p2-p)].[(p2-p)x(p3-p)] = (p1p.p2p)(p2p.p3p) - (p2p.p2p)(p1p.p3p) 
	if (p1p2*p2p3 - p2p2*p1p3 < 0.0) return false;


	// second condition u.w < 0
	// u.w = [(p1-p)x(p2-p)].[(p3-p)x(p1-p)] = (p1p.p3p)(p2p.p1p) - (p2p.p3p)(p1p.p1p)
	real p1p1 = dot(p1p, p1p);
	if (p1p3*p1p2 - p2p3*p1p1 < (0.0)) return false;

	return true;
}

INLINE_FUNCTION_HD
void cramerRule2(real A[2][2], real B[2], real & x1, real &x2)
{
	real det = (A[0][0] * A[1][1] - A[1][0]*A[0][1]);
	x1 = (B[0]*A[1][1] - B[1]*A[0][1]) / det;
	x2 = (A[0][0] * B[1] - A[1][0] * B[0])/ det;
}

INLINE_FUNCTION_HD
bool pointInPlane(
	const realx3& p1,
	const realx3& p2,
	const realx3& p3,
	const realx3 &p,
	int32 &Ln)
{
	realx3 v0 = p2 - p1;
	realx3 v1 = p3 - p1;
	realx3 v2 = p - p1;

	real A[2][2] = { dot(v0, v0), dot(v0, v1), dot(v1, v0), dot(v1, v1) };
	real B[2] = { dot(v0, v2), dot(v1, v2) };
	real nu, w;

	cramerRule2(A, B, nu, w);
	real nuW = nu + w;
		
	
	if (nuW > 1) 
	{
		Ln = 2;	return true;
	}
	else if (nuW >= 0)
	{
		if (nu >= 0 && w >= 0)
		{
			Ln = 0;	return true;
		}
		if (nu > 0 && w < 0)
		{
			Ln = 1;	return true;
		}
		if (nu < 0 && w > 0)
		{
			Ln = 3;	return true;
		}
	}
	else
	{
		Ln = 1;	return true;
	}
	
	return false;
}

INLINE_FUNCTION_HD
bool isSphereInContactActiveSide(
	const realx3& p1,
	const realx3& p2,
	const realx3& p3,
	const realx3& cntr,
	real 	rad,
	real& 	ovrlp,
	realx3& norm,
	realx3& cp)
{
	
	triWall wall(true, p1,p2,p3);

	real dist = wall.normalDistFromWall(cntr);
	
	if(dist < 0.0 )return false;

	ovrlp = rad - dist;

	if (ovrlp > 0)
	{
		realx3 ptOnPlane = wall.nearestPointOnWall(cntr);
		
		if (pointInPlane(p1, p2, p3, ptOnPlane))
		{
			cp = ptOnPlane;
			norm = -wall.n_;
			return true;
		}

		realx3 lnv;
		
		if (pLine(p1,p2).lineSphereCheck(cntr, rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}

		if ( pLine(p2,p3).lineSphereCheck(cntr, rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}

		if ( pLine(p3,p1).lineSphereCheck(cntr, rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}
	}
			
	return false;
}

INLINE_FUNCTION_HD
bool isSphereInContactBothSides(
	const realx3x3& tri,
	const realx3& cntr,
	real 	Rad,
	real& 	ovrlp,
	realx3& norm,
	realx3& cp)
{
	
	triWall wall(true, tri.x_,tri.y_,tri.z_);

	real dist = wall.normalDistFromWall(cntr);
	
	
	ovrlp = Rad - abs(dist);

	if (ovrlp > 0)
	{
		realx3 ptOnPlane = wall.nearestPointOnWall(cntr);
		
		if (pointInPlane(tri.x_,tri.y_,tri.z_,ptOnPlane))
		{
			cp = ptOnPlane;
			
			if(dist >= 0.0)
				norm = -wall.n_;
			else
				norm = wall.n_;
			return true;
		}

		realx3 lnv;
		
		if (pLine(tri.x_, tri.y_).lineSphereCheck(cntr, Rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}

		if ( pLine(tri.y_, tri.z_).lineSphereCheck(cntr, Rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}

		if ( pLine(tri.z_, tri.x_).lineSphereCheck(cntr, Rad, lnv, cp, ovrlp))
		{
			norm = -lnv;
			return true;
		}
	}
			
	return false;
}


} // pFlow::sphTriInteraction


#endif //__sphereTriSurfaceContact_hpp__