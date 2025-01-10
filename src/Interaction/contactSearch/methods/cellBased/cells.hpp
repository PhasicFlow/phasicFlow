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

#ifndef __cells_hpp__
#define __cells_hpp__


#include "types.hpp"
#include "box.hpp"

namespace pFlow
{

class cells
{
private:

	// - domain 
	box 		domainBox_{realx3(0.0), realx3(1.0)};

	// - cell size 
	real 		celldx_{1};

	int32x3 	numCells_{1,1,1};
	
	// - protected methods
	INLINE_FUNCTION_H
	void calculate()
	{
		numCells_ = (domainBox_.maxPoint()-domainBox_.minPoint())/celldx_ + realx3(1.0);
		numCells_ = max( numCells_ , int32x3(1) );
	}

public:

	INLINE_FUNCTION_HD
	cells() = default;

	INLINE_FUNCTION_H
	cells(const box& domain, real cellSize)
	:
		domainBox_(domain),
		celldx_(cellSize)
	{
		calculate();
	}

	INLINE_FUNCTION_HD
	cells(const cells&) = default;

	INLINE_FUNCTION_HD
	cells& operator = (const cells&) = default;

	INLINE_FUNCTION_HD
	cells(cells &&) = default;

	INLINE_FUNCTION_HD
	cells& operator=(cells&&) = default;

	cells getCells()const
	{
		return *this;
	}

	INLINE_FUNCTION_H
	void setCellSize(real cellSize)
	{
		celldx_ = cellSize;
		calculate();
	}

	INLINE_FUNCTION_HD
	real cellSize()const
	{
		return celldx_;
	}

	INLINE_FUNCTION_HD
	const int32x3& numCells()const
	{
		return numCells_;
	}

	INLINE_FUNCTION_HD
	int32 nx()const
	{
		return numCells_.x();
	}

	INLINE_FUNCTION_HD
	int32 ny()const
	{
		return numCells_.y();
	}

	INLINE_FUNCTION_HD
	int32 nz()const
	{
		return numCells_.z();
	}

	INLINE_FUNCTION_HD
	int64 totalCells()const
	{
		return  static_cast<int64>(numCells_.x())*
				static_cast<int64>(numCells_.y())*
				static_cast<int64>(numCells_.z());
	}

	const auto& domainBox()const
	{
		return domainBox_;
	}
	
	INLINE_FUNCTION_HD
	int32x3 pointIndex(const realx3& p)const
	{
		return int32x3( (p - domainBox_.minPoint())/celldx_ );
	}

	INLINE_FUNCTION_HD
	bool pointIndexInDomain(const realx3 p, int32x3& index)const
	{
		if(!inDomain(p))return false;
		index = this->pointIndex(p);
		return true;
	}

	INLINE_FUNCTION_HD
	bool inDomain(const realx3& p)const
	{	
		return domainBox_.isInside(p);
	}

	INLINE_FUNCTION_HD
	bool inCellRange(const int32x3& cell)const
	{
		if(cell.x()<0)return false;
		if(cell.y()<0)return false;
		if(cell.z()<0)return false;
		if(cell.x()>numCells_.x()-1) return false;
		if(cell.y()>numCells_.y()-1) return false;
		if(cell.z()>numCells_.z()-1) return false;
		return true;
	}

	INLINE_FUNCTION_HD
	bool inCellRange(int32 i, int32 j, int32 k)const
	{
		if(i<0)return false;
		if(j<0)return false;
		if(k<0)return false;
		if(i>numCells_.x()-1) return false;
		if(j>numCells_.y()-1) return false;
		if(k>numCells_.z()-1) return false;
		return true;
	}

	
	INLINE_FUNCTION_HD
	void extendBox(
		const realx3& p1,
		const realx3& p2,
		const realx3& p3, 
		real  extent,
		realx3& minP,
		realx3& maxP)const
	{
		minP = min(min(p1,p2),p3) - extent*celldx_ ;
		maxP = max(max(p1,p2),p3) + extent*celldx_ ;

		minP = bound(minP);
		maxP = bound(maxP);
	}

	INLINE_FUNCTION_HD
	int32x3 bound(int32x3 p)const
	{
		return int32x3(
			min( numCells_.x()-1, max(0,p.x())),
			min( numCells_.y()-1, max(0,p.y())),
			min( numCells_.z()-1, max(0,p.z()))
			);
	}

	INLINE_FUNCTION_HD
	realx3 bound(realx3 p)const
	{
		return realx3(
			min( domainBox_.maxPoint().x(), max(domainBox_.minPoint().x(),p.x())),
			min( domainBox_.maxPoint().y(), max(domainBox_.minPoint().y(),p.y())),
			min( domainBox_.maxPoint().z(), max(domainBox_.minPoint().z(),p.z()))
			);
	}
};	


}


#endif
