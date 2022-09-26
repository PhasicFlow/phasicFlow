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

#ifndef __fieldOperations_H__ 
#define __fieldOperations_H__

#include "rectMeshFields.H"
#include "pointFields.H"
#include "pointRectCell.H"
#include "includeMask.H"

namespace pFlow
{


template<typename T>
rectMeshField_H<T> sumOp( const pointField_H<T> field, const pointRectCell& pointToCell)
{
	// create field
	const auto& mesh = pointToCell.mesh();

	rectMeshField_H<T> results(mesh, T(0));

	for(int32 i=0; i<mesh.nx(); i++)
	{
		for(int32 j=0; j<mesh.ny(); j++)
		{
			for(int32 k=0; k<mesh.nz(); k++)
			{
				auto n = pointToCell.Head(i,j,k);	
				T res (0);
				while(n>-1)
				{
					res += field[n];
					n = pointToCell.Next(n);
				}

				results(i,j,k) = res;
			}
		}
	}

	return results;
}

template<typename T, typename incMask>
rectMeshField_H<T> sumMaksOp( const pointField_H<T> field, const pointRectCell& pointToCell, const incMask& mask)
{
	// create field
	const auto& mesh = pointToCell.mesh();

	rectMeshField_H<T> results(mesh, T(0));

	for(int32 i=0; i<mesh.nx(); i++)
	{
		for(int32 j=0; j<mesh.ny(); j++)
		{
			for(int32 k=0; k<mesh.nz(); k++)
			{
				//auto [loop, n] = pointToCell.startLoop(i,j,k);
				auto n = pointToCell.Head(i,j,k);	
				T res (0);
				
				while(n>-1)
				{
					
					if(mask(n))
					{
						res += field[n];
					}

					n = pointToCell.Next(n);
				}
		
				results(i,j,k) = res;
			}
		}
	}

	return results;
}




}


#endif //__fieldOperations_H__

