#include "rotatingAxis.hpp"
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

INLINE_FUNCTION_HD
pFlow::realx3 pFlow::rotatingAxis::linVelocityPoint(const realx3 &p, const realx3&)const
{
	
	if(!inTimeRange()) return {0,0,0};

	realx3 L = p - projectPoint(p);
	return cross(omega_*unitVector(),L);
}

INLINE_FUNCTION_HD 
pFlow::realx3 pFlow::rotatingAxis::transferPoint(const realx3 p, real dt)const
{
    return  rotate(p, *this, dt);
}

INLINE_FUNCTION_HD
pFlow::realx3 pFlow::rotate(const realx3& p, const rotatingAxis& ax, real dt)
{
	
	if(!ax.inTimeRange()) return p;

	realx3 nv = ax.unitVector();
	real cos_tet = cos(ax.omega()*dt);
	real sin_tet = sin(ax.omega()*dt);
	real u2 = nv.x()*nv.x();
	real v2 = nv.y()*nv.y();
	real w2 = nv.z()*nv.z();
	realx3 lp1 = ax.point1();
	
	// (a(v2+w2) - u( bv + cw - ux - vy - wz)) (1-cos_tet) + x cos_tet + (- cv + bw - wy + vz) sin_tet 
	realx3 res;

	res.x_ = (lp1.x_*(v2 + w2) - (nv.x_*(lp1.y_*nv.y_ + lp1.z_*nv.z_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.x_ * cos_tet +
		(-lp1.z_*nv.y_ + lp1.y_*nv.z_ - nv.z_*p.y_ + nv.y_*p.z_) * sin_tet;


	// ( b(u2+w2) - v( au + cw - ux - vy - wz))(1-cos_tet) + y cos_tet + ( cu - aw + wx - uz ) sin_tet
	res.y_ = (lp1.y_*(u2 + w2) - (nv.y_*(lp1.x_*nv.x_ + lp1.z_*nv.z_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.y_ * cos_tet +
		(lp1.z_*nv.x_ - lp1.x_*nv.z_ + nv.z_*p.x_ - nv.x_*p.z_) * sin_tet;

	// (c(u2+v2) - w( au + bv - ux - vy - wz ))(1-cos_tet) + z cos_tet + (-bu + av - vx + uy) sin_tet
	res.z_ = (lp1.z_*(u2 + v2) - (nv.z_*(lp1.x_*nv.x_ + lp1.y_*nv.y_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.z_ * cos_tet +
		(-lp1.y_*nv.x_ + lp1.x_*nv.y_ - nv.y_*p.x_ + nv.x_*p.y_) * sin_tet;

	return res;	
}

INLINE_FUNCTION_HD
pFlow::realx3 pFlow::rotate(const realx3 &p, const line& ln, real theta)
{

	realx3 nv = ln.unitVector();
	real cos_tet = cos(theta);
	real sin_tet = sin(theta);
	real u2 = nv.x()*nv.x();
	real v2 = nv.y()*nv.y();
	real w2 = nv.z()*nv.z();
	realx3 lp1 = ln.point1();
	
	// (a(v2+w2) - u( bv + cw - ux - vy - wz)) (1-cos_tet) + x cos_tet + (- cv + bw - wy + vz) sin_tet 
	realx3 res;

	res.x_ = (lp1.x_*(v2 + w2) - (nv.x_*(lp1.y_*nv.y_ + lp1.z_*nv.z_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.x_ * cos_tet +
		(-lp1.z_*nv.y_ + lp1.y_*nv.z_ - nv.z_*p.y_ + nv.y_*p.z_) * sin_tet;


	// ( b(u2+w2) - v( au + cw - ux - vy - wz))(1-cos_tet) + y cos_tet + ( cu - aw + wx - uz ) sin_tet
	res.y_ = (lp1.y_*(u2 + w2) - (nv.y_*(lp1.x_*nv.x_ + lp1.z_*nv.z_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.y_ * cos_tet +
		(lp1.z_*nv.x_ - lp1.x_*nv.z_ + nv.z_*p.x_ - nv.x_*p.z_) * sin_tet;

	// (c(u2+v2) - w( au + bv - ux - vy - wz ))(1-cos_tet) + z cos_tet + (-bu + av - vx + uy) sin_tet
	res.z_ = (lp1.z_*(u2 + v2) - (nv.z_*(lp1.x_*nv.x_ + lp1.y_*nv.y_ - nv.x_*p.x_ - nv.y_*p.y_ - nv.z_*p.z_)))*(1 - cos_tet) +
		p.z_ * cos_tet +
		(-lp1.y_*nv.x_ + lp1.x_*nv.y_ - nv.y_*p.x_ + nv.x_*p.y_) * sin_tet;

	return res;
}

INLINE_FUNCTION_HD
void pFlow::rotate(realx3* p, uint32 n, const line& ln, real theta)
{
	realx3 nv = ln.unitVector();
	real cos_tet = cos(theta);
	real sin_tet = sin(theta);
	real u2 = nv.x()*nv.x();
	real v2 = nv.y()*nv.y();
	real w2 = nv.z()*nv.z();
	realx3 lp1 = ln.point1();
	
	// (a(v2+w2) - u( bv + cw - ux - vy - wz)) (1-cos_tet) + x cos_tet + (- cv + bw - wy + vz) sin_tet 
	realx3 res;

	for(uint32 i=0; i<n; i++ )
	{
		res.x_ = (lp1.x_*(v2 + w2) - (nv.x_*(lp1.y_*nv.y_ + lp1.z_*nv.z_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].x_ * cos_tet +
			(-lp1.z_*nv.y_ + lp1.y_*nv.z_ - nv.z_*p[i].y_ + nv.y_*p[i].z_) * sin_tet;


		// ( b(u2+w2) - v( au + cw - ux - vy - wz))(1-cos_tet) + y cos_tet + ( cu - aw + wx - uz ) sin_tet
		res.y_ = (lp1.y_*(u2 + w2) - (nv.y_*(lp1.x_*nv.x_ + lp1.z_*nv.z_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].y_ * cos_tet +
			(lp1.z_*nv.x_ - lp1.x_*nv.z_ + nv.z_*p[i].x_ - nv.x_*p[i].z_) * sin_tet;

		// (c(u2+v2) - w( au + bv - ux - vy - wz ))(1-cos_tet) + z cos_tet + (-bu + av - vx + uy) sin_tet
		res.z_ = (lp1.z_*(u2 + v2) - (nv.z_*(lp1.x_*nv.x_ + lp1.y_*nv.y_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].z_ * cos_tet +
			(-lp1.y_*nv.x_ + lp1.x_*nv.y_ - nv.y_*p[i].x_ + nv.x_*p[i].y_) * sin_tet;	

		p[i] = res;
	}
	
}

INLINE_FUNCTION_HD
void pFlow::rotate(realx3* p, uint32 n, const rotatingAxis& ax, real dt)
{
	if(!ax.inTimeRange()) return;
	
	realx3 nv = ax.unitVector();
	real cos_tet = cos(ax.omega()*dt);
	real sin_tet = sin(ax.omega()*dt);
	real u2 = nv.x()*nv.x();
	real v2 = nv.y()*nv.y();
	real w2 = nv.z()*nv.z();
	realx3 lp1 = ax.point1();
	
	// (a(v2+w2) - u( bv + cw - ux - vy - wz)) (1-cos_tet) + x cos_tet + (- cv + bw - wy + vz) sin_tet 
	realx3 res;

	for(uint32 i=0; i<n; i++ )
	{
		res.x_ = (lp1.x_*(v2 + w2) - (nv.x_*(lp1.y_*nv.y_ + lp1.z_*nv.z_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].x_ * cos_tet +
			(-lp1.z_*nv.y_ + lp1.y_*nv.z_ - nv.z_*p[i].y_ + nv.y_*p[i].z_) * sin_tet;


		// ( b(u2+w2) - v( au + cw - ux - vy - wz))(1-cos_tet) + y cos_tet + ( cu - aw + wx - uz ) sin_tet
		res.y_ = (lp1.y_*(u2 + w2) - (nv.y_*(lp1.x_*nv.x_ + lp1.z_*nv.z_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].y_ * cos_tet +
			(lp1.z_*nv.x_ - lp1.x_*nv.z_ + nv.z_*p[i].x_ - nv.x_*p[i].z_) * sin_tet;

		// (c(u2+v2) - w( au + bv - ux - vy - wz ))(1-cos_tet) + z cos_tet + (-bu + av - vx + uy) sin_tet
		res.z_ = (lp1.z_*(u2 + v2) - (nv.z_*(lp1.x_*nv.x_ + lp1.y_*nv.y_ - nv.x_*p[i].x_ - nv.y_*p[i].y_ - nv.z_*p[i].z_)))*(1 - cos_tet) +
			p[i].z_ * cos_tet +
			(-lp1.y_*nv.x_ + lp1.x_*nv.y_ - nv.y_*p[i].x_ + nv.x_*p[i].y_) * sin_tet;	

		p[i] = res;
	}
	
}