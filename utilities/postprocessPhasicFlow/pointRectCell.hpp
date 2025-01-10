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

#ifndef __pointRectCell_hpp__
#define __pointRectCell_hpp__

#include "cellMapper.hpp"
#include "rectMeshFields.hpp"
#include "pointStructure.hpp"

namespace pFlow
{


class pointRectCell
{
public:

	using mapType = cellMapper;

	//using memory_space = typename mapType::memory_space;

protected:

	repository&				processedRepository_;

	rectangleMesh 			mesh_;

	const pointStructure& 	pStruct_;

	hostViewType1D<realx3> 	pointPosition_;

	mapType 				map_;

	int32RectMeshField_H 	nPointInCell_;

public:

	pointRectCell( 
		const   dictionary& 	dictMesh,
		const 	pointStructure& pStruct,
		repository& rep)
	:
		processedRepository_(rep),
		mesh_
		(
			dictMesh,
			&rep
		),
		pStruct_(pStruct),
		pointPosition_(pStruct_.pointPosition().hostViewAll()),
		map_
		( 
			mesh_,
			pointPosition_,
			pStruct_.activePointsMaskHost()
		),
		nPointInCell_(mesh_,"nPointInCell", 0)
	{

		mapPOints();
	}

	auto& mesh()
	{
		return mesh_;
	}

	const auto& mesh()const
	{
		return mesh_;
	}
	
	auto& processedRepository()
	{
		return processedRepository_;
	}

	void mapPOints()
	{
		auto points = pStruct_.pointPositionHost();
		auto activeMask  = pStruct_.activePointsMaskHost();

		map_.build(points, activeMask);
		
		auto iterator = map_.getCellIterator();
	
		for(int32 i=0; i<mesh_.nx(); i++)
		{
			for(int32 j=0; j<mesh_.ny(); j++)
			{
				for(int32 k=0; k<mesh_.nz(); k++)
				{

					int32 res = 0;
					uint32 n = iterator.start(i,j,k);
					while( n!= cellMapper::NoPos)
					{
						res+=1;
						n = iterator.getNext(n);
					}
					nPointInCell_(i,j,k) = res;

				}
			}
		}

	}

	auto getCellIterator()const
	{
		return map_.getCellIterator();
	}

	int32 nPointInCell(int32 i, int32 j, int32 k)const 
	{
		return nPointInCell_(i,j,k);
	}

	//auto
};

}

#endif // __pointRectCell_hpp__ 
