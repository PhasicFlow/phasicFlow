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


/*
class name: zAxis
a class for transfering points to a cylinderical coordinates defined by points
p1 and p2
*/

#ifndef __zAxis_hpp__
#define __zAxis_hpp__

#include "types.hpp"

namespace pFlow
{

template <typename T, int32 nRow, int32 nInner, int32 nCol >
void MatMul(T(&A)[nRow][nInner], T(&B)[nInner][nCol], T(&C)[nRow][nCol]);

template <typename T, int32 nRow, int32 nCol >
void assignMat(T(&A)[nRow][nCol], T(&B)[nRow][nCol]);

class zAxis
{
public:
	// constructors 
	zAxis(const realx3 &lp1, const realx3 &lp2);
	
	real length()const
	{
		return pFlow::length(p2_-p1_);
	}

	realx3 transferToZ(const realx3 & p);

	realx3 transferBackZ(const realx3 & p);

private:
	void makeTransMatrix();
protected:

	realx3 p1_;
	realx3 p2_;
	realx3 n_;

	real Trans_z_xz_P1_[4][4];

	real ITrans_P1_xz_z_[4][4];
};



template <typename T, int32 nRow, int32 nInner, int32 nCol >
void MatMul(T(&A)[nRow][nInner], T(&B)[nInner][nCol], T(&C)[nRow][nCol])
{

	for (int32 row = 0; row < nRow; row++)
	{
		for (int32 col = 0; col < nCol; col++)
		{
			T sum = 0;
			for (int inner = 0; inner < nInner; inner++)
			{
				sum += A[row][inner] * B[inner][col];
			}
			C[row][col] = sum;
		}
	}
}

template <typename T, int32 nRow, int32 nCol >
void assignMat(T(&A)[nRow][nCol], T(&B)[nRow][nCol])
{

	for (int32 row = 0; row < nRow; row++)
	{
		for (int32 col = 0; col < nCol; col++)
		{
			B[row][col] = A[row][col];
		}
	}
}


}

#endif

