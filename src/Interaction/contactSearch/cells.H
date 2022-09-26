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

#ifndef __cells_H__
#define __cells_H__


#include "types.H"
#include "box.H"

namespace pFlow
{

template<typename indexType>
class cells
{
public:
	
	using CellType = triple<indexType>;

protected:

	// - domain 
	box 		domain_{realx3(0.0), realx3(1.0)};

	// - cell size 
	realx3 		cellSize_{1,1,1};

	CellType 	numCells_{1,1,1};

	
	// - protected methods
	INLINE_FUNCTION_H
	void calculate()
	{
		numCells_ = (domain_.maxPoint()-domain_.minPoint())/cellSize_ + realx3(1.0);
		numCells_ = max( numCells_ , CellType(static_cast<indexType>(1)) );
	}

public:

	INLINE_FUNCTION_HD
	cells()
	{}

	INLINE_FUNCTION_H
	cells(const box& domain, real cellSize)
	:
		domain_(domain),
		cellSize_(cellSize)
	{
		calculate();
	}

 
	INLINE_FUNCTION_H
	cells(const box& domain, int32 nx, int32 ny, int32 nz)
	:
		domain_(domain),
		cellSize_(
			(domain_.maxPoint() - domain_.minPoint())/realx3(nx, ny,  nz)
			),
		numCells_(nx, ny, nz)
	{}

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
		cellSize_ = cellSize;
		calculate();
	}

	INLINE_FUNCTION_H
	void setCellSize(realx3 cellSize)
	{
		cellSize_ = cellSize;
		calculate();
	}

	INLINE_FUNCTION_HD
	realx3 cellSize()const
	{
		return cellSize_;
	}

	INLINE_FUNCTION_HD
	const CellType& numCells()const
	{
		return numCells_;
	}

	INLINE_FUNCTION_HD
	indexType nx()const
	{
		return numCells_.x();
	}

	INLINE_FUNCTION_HD
	indexType ny()const
	{
		return numCells_.y();
	}

	INLINE_FUNCTION_HD
	indexType nz()const
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

	const auto& domain()const
	{
		return domain_;
	}
	
	INLINE_FUNCTION_HD
	CellType pointIndex(const realx3& p)const
	{
		return CellType( (p - domain_.minPoint())/cellSize_ );
	}

	INLINE_FUNCTION_HD
	bool pointIndexInDomain(const realx3 p, CellType& index)const
	{
		if( !domain_.isInside(p) ) return false;
		
		index = this->pointIndex(p);
		return true;
	}

	INLINE_FUNCTION_HD
	bool inDomain(const realx3& p)const
	{	
		return domain_.isInside(p);
	}

	INLINE_FUNCTION_HD
	bool isInRange(const CellType& cell)const
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
	bool isInRange(indexType i, indexType j, indexType k)const
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
		const CellType& p1,
		const CellType& p2,
		const CellType& p3,
		indexType extent,
		CellType& minP,
		CellType& maxP)const
	{
		minP = min( min( p1, p2), p3)-extent;
		maxP = max( max( p1, p2), p3)+extent;

		minP = bound(minP);
		maxP = bound(maxP);
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
		minP = min(min(p1,p2),p3) - extent*cellSize_ ;
		maxP = max(max(p1,p2),p3) + extent*cellSize_ ;

		minP = bound(minP);
		maxP = bound(maxP);
	}

	INLINE_FUNCTION_HD
	CellType bound(CellType p)const
	{
		return CellType(
			min( numCells_.x()-1, max(0,p.x())),
			min( numCells_.y()-1, max(0,p.y())),
			min( numCells_.z()-1, max(0,p.z()))
			);
	}

	INLINE_FUNCTION_HD
	realx3 bound(realx3 p)const
	{
		return realx3(
			min( domain_.maxPoint().x(), max(domain_.minPoint().x(),p.x())),
			min( domain_.maxPoint().y(), max(domain_.minPoint().y(),p.y())),
			min( domain_.maxPoint().z(), max(domain_.minPoint().z(),p.z()))
			);
	}
};	


}


#endif
