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

#ifndef __fieldOperations_hpp__ 
#define __fieldOperations_hpp__

#include "rectMeshFields.hpp"
#include "pointFields.hpp"
#include "pointRectCell.hpp"
#include "includeMask.hpp"

namespace pFlow
{


template<typename T>
uniquePtr<rectMeshField_H<T>> sumOp( pointField_H<T>& field, pointRectCell& pointToCell)
{
	// create field
	auto& mesh = pointToCell.mesh();
	auto iterator = pointToCell.getCellIterator();
	auto f = field.deviceView();

	auto resultsPtr = makeUnique<rectMeshField_H<T>>(mesh, T(0));
	auto& results = resultsPtr();
	for(int32 i=0; i<mesh.nx(); i++)
	{
		for(int32 j=0; j<mesh.ny(); j++)
		{
			for(int32 k=0; k<mesh.nz(); k++)
			{
				uint32 n = iterator.start(i,j,k);	
				T res (0);
				while(n != cellMapper::NoPos)
				{
					res += f[n];
					n = iterator.getNext(n);
				}

				results(i,j,k) = res;
			}
		}
	}

	return resultsPtr;
}

template<typename T, typename incMask>
uniquePtr<rectMeshField_H<T>> sumMaksOp( pointField_H<T>& field, pointRectCell& pointToCell, const incMask& mask)
{
	// create field
	auto& mesh = pointToCell.mesh();
	auto iterator = pointToCell.getCellIterator();
	auto f = field.deviceView();

	auto resultsPtr = makeUnique<rectMeshField_H<T>>(mesh, T(0));
	auto& results = resultsPtr();

	for(int32 i=0; i<mesh.nx(); i++)
	{
		for(int32 j=0; j<mesh.ny(); j++)
		{
			for(int32 k=0; k<mesh.nz(); k++)
			{
				//auto [loop, n] = pointToCell.startLoop(i,j,k);
				uint32 n = iterator.start(i,j,k);	
				T res (0);
				
				while(n!= cellMapper::NoPos)
				{
					
					if(mask(n))
					{
						res += f[n];
					}

					n = iterator.getNext(n);
				}
		
				results(i,j,k) = res;
			}
		}
	}

	return resultsPtr;
}




}


#endif //__fieldOperations_hpp__

